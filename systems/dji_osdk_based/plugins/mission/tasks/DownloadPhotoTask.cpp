#include "DownloadPhotoTask.hpp"
#include "plugins/mission/DJIWPMInstance.hpp"
#include "../exif.h"
#include <dji_vehicle.hpp>
#include <unordered_set>
#include <future>
#include <stdio.h>
#include <sstream>
#include <iomanip>
#include <cmath>

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

        if( !_owner->sharedInfo() )
        {
            rst.detail = "Shared Info Not Set";
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
     * @brief 匹配照片，如果失败返回uint32_t max
     * 
     * @param photo_path 
     * @return uint32_t 
     */
    uint32_t photo_match(const std::string& photo_path)
    {
        FILE* fp = fopen(photo_path.c_str(), "rb");
        if(!fp)
        {
            instance->system()->error("could not open photo file:" + photo_path);
            return UINT32_MAX;
        }
        fseek(fp, 0, SEEK_END);
        unsigned long fsize = ftell(fp);
        rewind(fp);
        unsigned char* buf = new unsigned char[fsize];
        if(fread(buf, 1, fsize, fp) != fsize)
        {
            instance->system()->error("could not alloc memory for photo:"+ photo_path);
            delete[] buf;
            return UINT32_MAX;
        }
        fclose(fp);

        easyexif::EXIFInfo result;
        int code = result.parseFrom(buf, fsize);
        delete[] buf;
        if(code)
        {
            instance->system()->error("Error parsing EXIF: code " + std::to_string(code));
            return UINT32_MAX;
        }

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
            " BIAS:"        + std::to_string(shared_info->photo_bias_time)
        );

        return item_index;
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
            // 不是照片格式
            if(file_ptr->type != DJIFileType::JPG)
            {
                instance->system()->info(file_ptr->name + "is not photo format, pass");
                continue;
            }

            /********************* 开始下载 *********************/
            instance->system()->info("Start Downloading File :" + file_ptr->name);
            std::promise<bool> _file_download_promise;

            auto future = _file_download_promise.get_future();
            rsdk::event::mission::SavedPhotoInfo info;
            bool download_rst{false};

            FileDownloadBlock download_block{this, _file_download_promise};

            {
                /*
                std::lock_guard<std::mutex>  lck(instance->system()->DJIAPIMutex());
                dji_vehicle->cameraManager->setModeSync(
                    DJI::OSDK::PAYLOAD_INDEX_0,
                    CameraModule::WorkMode::PLAYBACK,
                    1
                );
                */
                info.file_path = save_path + file_ptr->name;
                instance->system()->info("Start Downloading" + info.file_path);
                auto ret = dji_vehicle->cameraManager->startReqFileData(
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
                instance->system()->info("Stop Downloading" + info.file_path);
            }
            /********************* 下载结束 *********************/
            if(download_rst)
            {
                // match the photo with item index
                auto index  = photo_match(info.file_path);

                if(index == UINT32_MAX)
                {
                    instance->system()->warning("can not match photo:" + file_ptr->name);
                    continue;
                }
                instance->system()->warning(file_ptr->name + " matched item index:" + std::to_string(index));
                info.item_index = index;

                // post event
                instance->system()->postEvent(
                    instance,
                    std::make_shared<
                        rsdk::event::mission::WPMSavedPhotoEvent
                    >(info)
                );
                instance->system()->info(
                    "Download File :" + file_ptr->name + " Success"
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
