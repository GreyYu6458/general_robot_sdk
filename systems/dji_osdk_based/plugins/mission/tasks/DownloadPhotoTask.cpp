#include "DownloadPhotoTask.hpp"
#include "plugins/mission/DJIWPMInstance.hpp"
#include <dji_vehicle.hpp>
#include <unordered_set>
#include <future>

class DJIDownloadPhotoTask::Impl
{
public:
    Impl(DJIWPMInstance* instance)
    : instance(instance)
    {

    }

    rsdk::mission::StageRst startSyncFiles()
    {
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
        rst.type = rsdk::mission::StageRstType::FAILED;
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

    rsdk::mission::StageRst downloadFiles()
    {
        const auto& save_path = instance->mediaRootPath();

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
            std::promise<bool> _file_download_promise;

            auto future = _file_download_promise.get_future();

            FileDownloadBlock download_block{this, _file_download_promise};

            std::lock_guard<std::mutex>  lck(instance->system()->DJIAPIMutex());

            rsdk::event::mission::SavedPhotoInfo info;
            info.file_path = save_path + file_ptr->name;

            dji_vehicle->cameraManager->startReqFileData(
                DJI::OSDK::PAYLOAD_INDEX_0,
                file_ptr->index,
                info.file_path,
                &Impl::fileDownloadReponse,
                (void*)&download_block
            );

            if(future.get())
            {
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
