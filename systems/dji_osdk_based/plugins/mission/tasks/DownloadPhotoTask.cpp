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
    Impl(DJIWPMInstance* instance)
    : instance(instance)
    {

    }

    rsdk::mission::StageRst startSyncFiles()
    {
        // wait photo totally recorded
        std::this_thread::sleep_for(std::chrono::seconds(2));

        auto sync_rst = instance->system()->cameraManager().updateFilesSetSync();
        
        rsdk::mission::StageRst rst;

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

        block->_success_promise.set_value(ret_code == ErrorCode::SysCommonErr::Success);
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

        uint64_t time_stamp = mktime(&t) * 1e6;
        uint64_t min = UINT64_MAX;
        uint64_t event_time = UINT64_MAX;
        uint32_t item_index = UINT32_MAX;

        // START MATCH
        for(const auto event : instance->sharedInfo().photo_time_item_index_list)
        {
            auto diff_time = event.first - time_stamp;
            diff_time = diff_time < 0 ? -diff_time : diff_time;
            if(min > diff_time)
            {
                min = diff_time;
                event_time = event.first;
                item_index = event.second;
            }
        }

        instance->system()->info(photo_path + " DateTime :" + result.DateTime);
        instance->system()->info(photo_path + " UNIXTIME :" + std::to_string(time_stamp));
        instance->system()->info(photo_path + " EVENTTIME:" + std::to_string(event_time));

        return item_index;
    }

    rsdk::mission::StageRst downloadFiles()
    {
        const auto& save_path = instance->mediaRootPath();

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
                std::lock_guard<std::mutex>  lck(instance->system()->DJIAPIMutex());
                info.file_path = save_path + file_ptr->name;
                dji_vehicle->cameraManager->startReqFileData(
                    DJI::OSDK::PAYLOAD_INDEX_0,
                    file_ptr->index,
                    info.file_path,
                    &Impl::fileDownloadReponse,
                    (void*)&download_block
                );
                download_rst = future.get();
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
                else
                {
                    instance->system()->warning(file_ptr->name + "matched item index:" + std::to_string(index));
                }

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
                instance->system()->info(
                    "Download File :" + file_ptr->name + " Failed"
                );
            }
        }

        rst.detail = "media download success";
        rst.type = rsdk::mission::StageRstType::SUCCESS;
        return rst;
    }

    std::mutex                          _file_download_mutex;
    std::condition_variable             _file_download_cv;
    std::vector<const DJIMediaFile *>   files_ready_to_download;
    DJIWPMInstance*                     instance;
};

DJIDownloadPhotoTask::DJIDownloadPhotoTask(DJIWPMInstance* instance):
    rsdk::mission::SubMissionTask("DJI Download Photo Task"),
    _impl(new Impl(instance))
{
    
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
    return _impl->downloadFiles();
}