#include "DownloadPhotoTask.hpp"
#include "plugins/mission/DJIWPMInstance.hpp"
#include "plugins/mission/DJIDelegateMemory.hpp"
#include "../exif.h"
#include "p_rsdk/tools/math/haversine.h"
#include <dji_vehicle.hpp>
#include <unordered_set>
#include <future>
#include <stdio.h>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <regex>
#include <limits>

// DJI_20211014173902_0050_WIDE.jpg
static std::regex photo_name_pettern("DJI_(.*?)_(.*?)_(.*?).jpg");

class DJIDownloadPhotoTask::Impl
{
public:
    Impl(DJIDownloadPhotoTask* owner, DJIWPMInstance* instance)
    : instance(instance)
    {
        _owner = owner;
    }

    rsdk::mission::StageRst startSyncFiles()
    {
        rsdk::mission::StageRst rst;

        if( !_owner->delegateMemory() )
        {
            rst.detail = "delegate memory Not Set";
            rst.type = rsdk::mission::StageRstType::FAILED;
            return rst;
        }

        if( _owner->mediaDownloadPath().empty() )
        {
            rst.detail = "Media Path Not Set";
            rst.type = rsdk::mission::StageRstType::FAILED;
            return rst;
        }

        if(!instance->system()->cameraManager().isMainCameraEnable())
        {
            rst.detail = "DJI Camera Not Enable.";
            rst.type = rsdk::mission::StageRstType::FAILED;
            return rst;
        }

        // wait photo totally recorded
        std::this_thread::sleep_for(std::chrono::seconds(2));

        auto sync_rst = instance->system()->cameraManager().updateFilesSetSync();
        
        if(!sync_rst.second)
        {
            rst.detail = "Sync Files In Camera Failed.";
            rst.type = rsdk::mission::StageRstType::FAILED;
            return rst;
        }

        files_ready_to_download = std::move(sync_rst.first);

        rst.detail = "Sync Files In Camera Successfully.";
        rst.type = rsdk::mission::StageRstType::SUCCESS;
        return rst;
    }

    struct FileDownloadBlock
    {
        Impl*                            _impl;
        std::promise<bool>&              _success_promise;
    };

    /**
     * @brief   如果没对下载任务加锁，promise会被触发多次。
     *          然后OSDK会崩掉，然后程序也会崩掉，是不是很刺激
     * 
     * @param ret_code 
     * @param userData 
     */
    static void fileDownloadReponse(E_OsdkStat ret_code, void* userData)
    {
        FileDownloadBlock* block = (FileDownloadBlock*)(userData);
        try
        {
            block->_success_promise.set_value(ret_code == ErrorCode::SysCommonErr::Success);
        }
        catch(const std::exception& e)
        {
            block->_impl->instance->system()->error(e.what());
        }
    }

    /**
     * @brief 从exif库里抄的
     * 
     * @param photo_path 
     * @param info 
     * @return true 
     * @return false 
     */
    bool loadExif(const std::string& photo_path, easyexif::EXIFInfo& info)
    {
        FILE* fp = fopen(photo_path.c_str(), "rb");
        if(!fp)
        {
            instance->system()->error("could not open photo file:" + photo_path);
            return false;
        }
        fseek(fp, 0, SEEK_END);
        unsigned long fsize = ftell(fp);
        rewind(fp);
        unsigned char* buf = new unsigned char[fsize];
        if(fread(buf, 1, fsize, fp) != fsize)
        {
            instance->system()->error("could not alloc memory for photo:"+ photo_path);
            delete[] buf;
            return false;
        }
        fclose(fp);
        int code = info.parseFrom(buf, fsize);
        delete[] buf;
        if(code)
        {
            instance->system()->error("Error parsing EXIF: code " + std::to_string(code));
            return false;
        }
        return true;
    }

    /*
    uint32_t compareByTime(const std::string& photo_path, const easyexif::EXIFInfo& result)
    {
        // GET TIME STAMP FROM PHOTO
        std::tm t{};
        std::istringstream ss(result.DateTime);
        ss >> std::get_time(&t, "%Y:%m:%d %H:%M:%S");
        if(ss.fail())
        {
            instance->system()->error("Error parsing DateTime");
            return UINT32_MAX;
        }

        int64_t  diff_time;
        int64_t  time_stamp = mktime(&t) * 1e6;
        uint64_t min = UINT64_MAX;
        uint64_t event_time = UINT64_MAX;
        uint32_t item_index = UINT32_MAX;

        auto shared_info = _owner->sharedInfo();

        // START MATCH
        for(const auto event : shared_info->photo_time_item_index_list)
        {
            diff_time = event.first - time_stamp;
            if(shared_info->get_first_photo)
            {
                diff_time = diff_time - shared_info->photo_bias_time;
            }
            diff_time = diff_time < 0 ? -diff_time : diff_time;  // abs
            if(min > diff_time)
            {
                min = diff_time;
                event_time = event.first;
                item_index = event.second;
            }
        }

        if(!shared_info->get_first_photo)
        {
            shared_info->photo_bias_time = min;
            shared_info->get_first_photo = true;
        }

        instance->system()->info(
            photo_path      + 
            " DateTime :"   + result.DateTime +
            " UNIXTIME :"   + std::to_string(time_stamp) + 
            " EVENTTIME:"   + std::to_string(event_time) +
            " DIFFERENCE:"  + std::to_string(min) + 
            " FIRST_BIAS:"  + std::to_string(shared_info->photo_bias_time)
        );

        return item_index;
    }
    */
    // TODO 目前会匹配所有的航点(虽然大概也慢不到哪去，但考虑到性能较差的硬件)，之后可以考虑引入滑动窗口法，让算法只匹配附近的航点
    uint32_t compareByLocation(const std::string& photo_path, const easyexif::EXIFInfo& result)
    {
        auto& photo_location    = result.GeoLocation;
        auto& delegate_memory   = instance->currentDelegateMemory();
        
        double min_distance     = std::numeric_limits<double>::max();
        uint32_t item_index     = std::numeric_limits<uint32_t>::max();
        for(const auto& position_index_pair : delegate_memory->dji_photo_point)
        {
            auto&  position     = position_index_pair.first;
            double lat_long_dis = haversine(
                position.latitude, position.longitude, 
                photo_location.Latitude, photo_location.Longitude
            );
            double height_dis   = (photo_location.Altitude - delegate_memory->takeoff_altitude) - position.altitude;
            double cdistance    = std::sqrt( lat_long_dis * lat_long_dis + height_dis * height_dis );
            
            instance->system()->trace(
                "PAIR WAPOINT LOCATION PHOTO LOCATION LAT:"     + 
                std::to_string(position.latitude)               + 
                " LON:" + std::to_string(position.longitude)    +
                " ALT:" + std::to_string(position.altitude)     +
                " DIS:" + std::to_string(cdistance)
            );
            
            if(cdistance < min_distance)
            {
                min_distance    = cdistance;
                item_index      = position_index_pair.second;
            }
        }

        instance->system()->warning(
            photo_path                                                          + 
            " PHOTO LOCATION LAT:" + std::to_string(photo_location.Latitude)    + 
            " LON:" + std::to_string(photo_location.Longitude)                  + 
            " ALT:" + std::to_string(photo_location.Altitude)                   +
            " INDEX :"      + std::to_string(item_index)                        +
            " DIFFERENCE:"  + std::to_string(min_distance)
        );

        // 距离差距太大，匹配失败
        if(min_distance > 2.5)
        {
            instance->system()->error(
                "Match Minimum Distance Bigger Than 2.5 Meters Which is " + 
                std::to_string(min_distance) + " [" + photo_path + "]"
            );
            return UINT32_MAX;
        }

        return item_index;
    }

    /**
     * @brief 匹配照片，如果失败返回uint32_t max
     * 
     * @param photo_path 
     * @return uint32_t 
     */
    uint32_t photo_match(const std::string& photo_path)
    {
        easyexif::EXIFInfo result;
        if(!loadExif(photo_path, result))
        {
            return UINT32_MAX;
        }

        return compareByLocation(photo_path, result);
    }

    rsdk::mission::StageRst downloadFiles(const std::string& path)
    {
        const auto& save_path = path;

        instance->system()->info("total " + std::to_string(files_ready_to_download.size()) + " new files in camera");
        rsdk::mission::StageRst rst;
        if(save_path.empty())
        {
            rst.detail = "media save path not set";
            rst.type = rsdk::mission::StageRstType::SUCCESS;
            return rst;
        }

        auto dji_vehicle = instance->system()->vehicle();

        for(auto file_ptr: files_ready_to_download)
        {
            std::lock_guard<std::mutex>  lck(instance->system()->DJIAPIMutex());
            // 不是照片格式
            if(file_ptr->type != DJIFileType::JPG)
            {
                instance->system()->info(file_ptr->name + "is not photo format, pass");
                continue;
            }

            /********************* 开始下载 *********************/
            instance->system()->info("Start Downloading File :" + file_ptr->name);
            rsdk::event::mission::SavedPhotoInfo info;
            bool download_rst{false};
                
            dji_vehicle->cameraManager->setModeSync(
                DJI::OSDK::PAYLOAD_INDEX_0,
                CameraModule::WorkMode::PLAYBACK,
                1
            );
            auto ret = instance->system()->vehicle()->cameraManager->obtainDownloadRightSync(
                PAYLOAD_INDEX_0,
                true,
                10
            );
            if(ret != ErrorCode::SysCommonErr::Success)
            {
                instance->system()->error("[system]:Obtaining download right failed!");
            }
            else
            {
                instance->system()->info("[system]:Obtaining download right success");
            }

            std::promise<bool> _file_download_promise;
            auto future = _file_download_promise.get_future();
            FileDownloadBlock download_block{this, _file_download_promise};

            info.file_path = save_path + file_ptr->name;
            instance->system()->info("Start Downloading" + info.file_path);
            ret = dji_vehicle->cameraManager->startReqFileData(
                DJI::OSDK::PAYLOAD_INDEX_0,
                file_ptr->index,
                info.file_path,
                &Impl::fileDownloadReponse,
                (void*)&download_block
            );
                
            auto status = future.wait_for(std::chrono::seconds(10));
            if(status == std::future_status::timeout)
            {
                _file_download_promise.set_value(10);
                std::this_thread::sleep_for(std::chrono::seconds(3));
            }

            download_rst = future.get();
            instance->system()->info("Finish Downloading" + info.file_path);
            /********************* 下载结束 *********************/
            if(download_rst)
            {
                instance->system()->info(
                    "Download File Success, Current Download File Count :" + 
                    std::to_string(++instance->currentDelegateMemory()->photo_download_number)
                );
                // match the photo with item index
                auto index  = photo_match(info.file_path);

                if(index == UINT32_MAX)
                {
                    instance->system()->error(
                        "Can Not Match Photo:" + 
                        file_ptr->name
                    );
                    continue;
                }
                instance->system()->warning(
                    file_ptr->name + 
                    " matched item index:" + 
                    std::to_string(index)
                );
                info.item_index = index;

                // post event
                instance->system()->postEvent(
                    instance,
                    make_event<rsdk::event::mission::WPMSavedPhotoEvent>(info)
                );
                instance->system()->info(
                    "Match Photo With Waypoint :" + file_ptr->name + " Success"
                );
            }
            else
            {
                instance->system()->error(
                    "Download File :" + file_ptr->name + " Failed!"
                );
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        rst.detail = "media download success";
        rst.type = rsdk::mission::StageRstType::SUCCESS;
        return rst;
    }

    std::mutex                          _file_download_mutex;
    DJIDownloadPhotoTask*               _owner;
    std::condition_variable             _file_download_cv;
    std::vector<const DJIMediaFile *>   files_ready_to_download;
    DJIWPMInstance*                     instance;
};

DJIDownloadPhotoTask::DJIDownloadPhotoTask(DJIWPMInstance* instance)
{
    _impl = new Impl(this, instance);
}

DJIDownloadPhotoTask::~DJIDownloadPhotoTask()
{
    delete _impl;
}

/**
 * @brief 启动阶段, 同步文件
 * 
 * @return true 
 * @return false 
 */
rsdk::mission::StageRst DJIDownloadPhotoTask::start_stage()
{
    return _impl->startSyncFiles();   
}

/**
 * @brief 执行阶段, 下载文件
 * 
 * @return true 
 * @return false 
 */
rsdk::mission::StageRst DJIDownloadPhotoTask::executing_stage()
{
    return _impl->downloadFiles(mediaDownloadPath());
}


void DJIDownloadPhotoTask::notifyMissionFinish(const rsdk::mission::StageRst& rst)
{

}
