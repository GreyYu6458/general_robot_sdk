#include "DJICameraManager.hpp"
#include "DJIVehicleSystem.hpp"
#include <dji_vehicle.hpp>
#include <dji_camera.hpp>
#include <future>

class DJICameraManager::Impl
{
public:
    Impl(DJIVehicleSystem* system)
    : _system(system)
    {

    }

    struct FileSyncBlock
    {
        Impl*                            _impl;
        std::promise<bool>&              _success_promise;
        std::vector<const DJIMediaFile*> _new_file_list;
    };

    static void cameraFileReponse(E_OsdkStat ret_code, const FilePackage file_list, void* userData)
    {
        FileSyncBlock* file_sync_block = (FileSyncBlock*)userData;

        if(ret_code != ErrorCode::SysCommonErr::Success)
        {
            // 同步失败
            file_sync_block->_success_promise.set_value(false);
        }
        else
        {
            // 同步成功
            for (const auto &media_file : file_list.media)
            {
                DJIMediaFile wrapper_file{ 
                    .name  = media_file.fileName, 
                    .index = media_file.fileIndex,
                    .type  = media_file.fileType == MediaFileType::JPEG ? DJIFileType::JPG : DJIFileType::OTHERS
                };
                // 没记录该文件
                if(!file_sync_block->_impl->_camera_files_set.count(wrapper_file))
                {
                    auto rst = file_sync_block->_impl->_camera_files_set.insert(wrapper_file);
                    auto __a = &(*rst.first);
                    file_sync_block->_new_file_list.push_back(__a);
                }
            }
            file_sync_block->_success_promise.set_value(true);
        }
    }

    bool initization()
    {
        ErrorCode::ErrorCodeType ret;
        ret = _system->vehicle()->cameraManager->initCameraModule(PAYLOAD_INDEX_0,"dji_camera_index_0");
        if (ret != ErrorCode::SysCommonErr::Success) 
        {
            return false;
        }
        else
        {
            return syncCameraFile();
        }
    }

    bool syncCameraFile()
    {
        if(!_system->vehicle()->isM300())
            return false;

        return updateFilesSetSync().second;
    }

    DJICameraManager::SyncRst updateFilesSetSync()
    {
        std::lock_guard<std::mutex> lck(_system->DJIAPIMutex());

        _system->info("[system]:M300 Detected, Start Camera File Synchronizating");
        _system->info("[system]:Start obtaining download right");
        
        auto ret = _system->vehicle()->cameraManager->obtainDownloadRightSync(
            PAYLOAD_INDEX_0,
            true,
            10
        );

        if(ret != ErrorCode::SysCommonErr::Success)
        {
            _system->error("[system]:Obtaining download right failed!");
        }
        else
        {
            _system->info("[system]:Obtaining download right success");
        }

        _system->info("[system]:Start Setting camera mode");
        ret = _system->vehicle()->cameraManager->setModeSync(
            PAYLOAD_INDEX_0,
            CameraModule::WorkMode::PLAYBACK,
            4
        );
        if(ret != ErrorCode::SysCommonErr::Success)
        {
            _system->warning("[system]:Setting camera mode failed!");
        }
        else
        {
            _system->info("[system]:Setting camera mode success");
        }

        std::promise<bool> _file_sync_promise;

        auto future = _file_sync_promise.get_future();

        FileSyncBlock sync_block{this, _file_sync_promise};
        _system->warning("[system]:Start Downloading File Descriptions In Camera");
        _system->vehicle()->cameraManager->startReqFileList(
            PAYLOAD_INDEX_0,
            &Impl::cameraFileReponse,
            &sync_block
        );

        return { sync_block._new_file_list, future.get() };
    }

    DJIVehicleSystem*       _system;
    DJIMediaSet             _camera_files_set;
    bool                    _is_enable{false};
};


DJICameraManager::DJICameraManager(DJIVehicleSystem* system)
: _impl(new Impl(system))
{

}

DJICameraManager::~DJICameraManager()
{
    delete _impl;
}

bool DJICameraManager::initization()
{
    _impl->_is_enable = _impl->initization();
    return _impl->_is_enable;
}

// 云台相机是否挂载
bool DJICameraManager::isMainCameraEnable()
{
    return _impl->_is_enable;
}

// 云台相机中文件名称的集合
DJIMediaSet& DJICameraManager::cameraFileSet()
{
    return _impl->_camera_files_set;
}

DJICameraManager::SyncRst DJICameraManager::updateFilesSetSync()
{
    return _impl->updateFilesSetSync();
}
