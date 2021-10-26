#include "DJIVehicleSystem.hpp"
#include "dji_linux_helper/DJIInitializationTool.hpp"

#include <dji_vehicle.hpp>
#include <dji_linker.hpp>
#include <dji_camera_manager.hpp>
#include <rsdk/system/SystemLinkMethods.hpp>

#include <boost/format.hpp>
#include <future>

#include "plugins/telemetry/DJIBattery.hpp"
#include "plugins/telemetry/DJIAvoid.hpp"
#include "plugins/telemetry/DJIFlightStatus.hpp"
#include "plugins/telemetry/DJIAttitude.hpp"
#include "plugins/telemetry/DJIGNSS.hpp"
#include "plugins/telemetry/DJIGNSSUncertain.hpp"
#include "plugins/telemetry/DJIGPSTime.hpp"
#include "plugins/mission/DJIWPMController.hpp"
#include "plugins/camera/DJIVideoStream.hpp"

using DJILinker = DJI::OSDK::Linker;

static const uint32_t   default_dji_app_id = 1100225;
static const char*      default_dji_app_key = "b25a76692794311fae2187a6c369ae012f18f46c1535f4e2d48e3ce26f841346";
static const uint32_t   default_dji_app_version = 1;

class DJIVehicleSystem::SystemImpl
{
    friend class DJIVehicleSystem;

#define REGIST_PLUGIN(dji_plugin_name, base_plugin_name) \
    _owner->dji_regist_plugin<base_plugin_name> \
        ( \
            std::make_shared<dji_plugin_name> \
            ( \
                _owner->shared_from_this() \
            ) \
        )

private:
    void registALLPlugin()
    {
        REGIST_PLUGIN(DJIAttitude,          rsdk::telemetry::AttitudePlugin);
        REGIST_PLUGIN(DJIAvoid,             rsdk::telemetry::AvoidancePlugin);
        REGIST_PLUGIN(DJIBatteryWrapper,    rsdk::telemetry::BatteryPlugin);
        REGIST_PLUGIN(DJIGNSSReceiver,      rsdk::telemetry::GNSSReceiverPlugin);
        REGIST_PLUGIN(DJIGNSSUncertain,     rsdk::telemetry::GNSSUncertainInfoPlugin);
        REGIST_PLUGIN(DJIFlightStatus,      rsdk::telemetry::FlyingRobotStatusPlugin);
        REGIST_PLUGIN(DJIWPMController,     rsdk::mission::waypoint::WPMControllerPlugin);
        REGIST_PLUGIN(DJIVideoStream,       rsdk::camera::VideoStreamPlugin);
    }

    SystemImpl(DJIVehicleSystem* owner)
    : _owner(owner)
    {

    }

    DJIVehicleSystem* _owner;

public:

    struct FileSyncBlock
    {
        SystemImpl* system_impl;
        std::promise<bool>& _success_promise;
    };

    static void cameraFileReponse(E_OsdkStat ret_code, const FilePackage file_list, void* userData)
    {
        FileSyncBlock* impl = (FileSyncBlock*)userData;
        
        if(ret_code != ErrorCode::SysCommonErr::Success)
        {
            impl->_success_promise.set_value(false);
        } 
        else
        {
            for (const auto &media_file : file_list.media)
            {
                impl->system_impl->_camera_files_set.insert(media_file.fileName);
            }
            impl->_success_promise.set_value(true);
        } 
    }

    bool cameraInitization()
    {
        /*
        enum PayloadIndexType {
          PAYLOAD_INDEX_0 = 0x00,
          PAYLOAD_INDEX_1 = 0x01,
          PAYLOAD_INDEX_2 = 0x02,
          PAYLOAD_INDEX_CNT = 0x03,
          PAYLOAD_INDEX_INVALID = 0x03,
        };
        */
        ErrorCode::ErrorCodeType ret;
        ret = _dji_vehicle->cameraManager->initCameraModule(PAYLOAD_INDEX_0,"dji_camera_index_0");
        if (ret != ErrorCode::SysCommonErr::Success) 
        {
            _owner->warning(
                "Init Camera module DJI CAMERA INDEX 0 failed. Camera Related Function Will Disable"
            );
            return false;
        }
        else
        {
            return syncCameraFile();
        }
    }

    /**
     * @brief 读取存储在DJI 相机中的文件(仅限M300)
     * 
     * @return true 
     * @return false 
     */
    bool syncCameraFile()
    {
        if(!_dji_vehicle->isM300())
            return false;

        _owner->info("[system]:M300 Detected, Start Camera File Synchronizating");
        _owner->info("[system]:Start obtaining download right");
        auto ret = _dji_vehicle->cameraManager->obtainDownloadRightSync(
            PAYLOAD_INDEX_0,
            true,
            10
        );
        if(ret != ErrorCode::SysCommonErr::Success)
        {
            _owner->warning("[system]:Obtaining download right failed!");
        }
        else
        {
            _owner->warning("[system]:Obtaining download right success");
        }

        _owner->info("[system]:Start Setting camera mode");
        ret = _dji_vehicle->cameraManager->setModeSync(
            PAYLOAD_INDEX_0,
            CameraModule::WorkMode::PLAYBACK,
            4
        );
        if(ret != ErrorCode::SysCommonErr::Success)
        {
            _owner->warning("[system]:Setting camera mode failed!");
        }
        else
        {
            _owner->warning("[system]:Setting camera mode success");
        }

        std::promise<bool> _file_sync_promise;

        auto future = _file_sync_promise.get_future();

        FileSyncBlock sync_block{this, _file_sync_promise};
        _owner->warning("[system]:Start Downloading File Descriptions In Camera");
        _dji_vehicle->cameraManager->startReqFileList(
            PAYLOAD_INDEX_0,
            &SystemImpl::cameraFileReponse,
            &sync_block
        );

        return future.get();
    }

    // impl of link method
    bool link(const rsdk::SystemConfig &config)
    {
        static boost::format formater("file:%s,line:%d,content:%s");
        _owner->_regist_osdk_info(); // get DJI Low layer message
        auto linker = new (std::nothrow) Linker();

        if (!linker)
        {
            _owner->error( (formater % __FILE__ % __LINE__ % "Malloc For DJI Linker Failed!").str());
            return false;
        }
        
        if (!linker->init())
        {
            _owner->error( (formater % __FILE__ % __LINE__ % "DJI Linker Init Failed!").str());
            return false;
        }

        _dji_linker = std::unique_ptr<DJILinker>(linker);

        auto usb_port_opt = config.getPameter<rsdk::SerialMethod>("usb");
        auto acm_port_opt = config.getPameter<rsdk::SerialMethod>("acm");

        if (!usb_port_opt.has_value())
        {   
            _owner->error( (formater % __FILE__ % __LINE__ % "config value is empty, system config has no correct key : \"usb\"").str());
            return false;
        }

        if (!acm_port_opt.has_value())
        {
            _owner->error( (formater % __FILE__ % __LINE__ % "config value is empty, system config has no correct key : \"acm\"").str());
            return false;
        }

        auto usb_port = *usb_port_opt;
        auto acm_port = *acm_port_opt;

        if (!_dji_linker->addUartChannel(usb_port.dev_path.c_str(), usb_port.baudrate, FC_UART_CHANNEL_ID))
        {
            _owner->error( (formater % __FILE__ % __LINE__ % "DJI VEHICLE ADD ACM channel Failed!").str());
            return false;
        }

        if (!_dji_linker->addUartChannel(acm_port.dev_path.c_str(), acm_port.baudrate, USB_ACM_CHANNEL_ID))
        {
            _owner->error( (formater % __FILE__ % __LINE__ % "DJI VEHICLE ADD USB channel Failed!").str());
            return false;
        }

        DJIVehicle::ActivateData activateData;
        activateData.ID = default_dji_app_id;
        char temp[65];
        std::strcpy(temp, default_dji_app_key);
        activateData.encKey = temp;
        activateData.version = default_dji_app_version;

        _dji_vehicle = std::make_shared<DJI::OSDK::Vehicle>(_dji_linker.get());

        if (!_dji_vehicle)
        {
            _owner->error( (formater % __FILE__ % __LINE__ % "Vehicle create failed").str());
            return false;
        }

        /*DJI::OSDK::ACK::ErrorCode ack = */
        _dji_vehicle->activate(&activateData, 5 /*max wait 5 seconds*/);

        if (!_dji_vehicle->isM300())
        {
            _dji_vehicle->setUSBFlightOn(true);
        }

        setVehicleModelType();

        DJI::OSDK::ACK::ErrorCode subscribe_status;
        subscribe_status = _dji_vehicle->subscribe->verify(1);
        if(DJI::OSDK::ACK::getError(subscribe_status) != DJI::OSDK::ACK::SUCCESS)
        {
            _owner->error( (formater % __FILE__ % __LINE__ % "DJI VERIFY ERROR").str());
            return false;
        }
        _unique_code = _dji_vehicle->getHwSerialNum();
        _owner->info("DJI Vehicle Link Success");

        return true;
    }

private:

    void setVehicleModelType()
    {
        if(_dji_vehicle->isM100())
        {
            _model = DJIVehicleModels::MODEL_M100;
        }
        else if(_dji_vehicle->isM210V2()) 
        {
            _model = DJIVehicleModels::MODEL_M210;
        }
        else if(_dji_vehicle->isM300())
        {
            _model = DJIVehicleModels::MODEL_M300;
        }
    }

    std::shared_ptr<DJIVehicle>         _dji_vehicle;
    std::unique_ptr<DJILinker>          _dji_linker;
    std::unique_ptr<DJIGPSTime>         _gps_time_sysc_plugin;
    bool                                _camera_is_enable{false};
    std::unordered_set<std::string>     _camera_files_set;
    std::mutex                          _dji_api_mutex;

    DJIVehicleModels _model{DJIVehicleModels::UNKNOWN};

    rsdk::SystemConfig _config;

    std::string _manufacturer;
    std::string _unique_code;

    bool _is_linked;
};

DJIVehicleSystem::DJIVehicleSystem()
    : _impl(new DJIVehicleSystem::SystemImpl(this))
{
    initDJIOsdkHal();
}

DJIVehicleSystem::~DJIVehicleSystem()
{
    delete _impl;
}

bool DJIVehicleSystem::tryLink(const rsdk::SystemConfig &config)
{
    _impl->_config = config;

    auto rst = _impl->link(config);

    if(rst == false)
        return rst;

    // start camera file sync
    _impl->_camera_is_enable = _impl->cameraInitization();
    if(_impl->_camera_is_enable)
    {
        info(
            "Synchronizating Files In Camera Success, Total " 
            + std::to_string(_impl->_camera_files_set.size())
            + " Files In Camera"
        );
    }
    else
    {
        warning(
            "Synchronizating Files In Camera Failed! Some Function Will Be Disable!"
        );
    }

    // start time sync
    _impl->_gps_time_sysc_plugin = 
        std::make_unique<DJIGPSTime>(
            shared_from_this()
        );
    _impl->_gps_time_sysc_plugin->setFreqency(5);
    _impl->_gps_time_sysc_plugin->exec();
    _impl->registALLPlugin();

    return rst;
}

bool DJIVehicleSystem::isMainCameraEnable()
{
    return _impl->_camera_is_enable;
}

std::mutex& DJIVehicleSystem::DJIAPIMutex()
{
    return _impl->_dji_api_mutex;
}

// 云台相机中文件名称的集合
std::unordered_set<std::string>& DJIVehicleSystem::camera_file_set()
{
    return _impl->_camera_files_set;
}

std::shared_ptr<DJIVehicleSystem> DJIVehicleSystem::shared_from_this()
{
    return std::static_pointer_cast<DJIVehicleSystem>( this->RobotSystem::shared_from_this() );
}

// 飞机是否连接
bool DJIVehicleSystem::isLink()
{
    return _impl->_dji_linker->isUartPlugged() && _impl->_dji_linker->isUSBPlugged();
}

// 设备的生产商信息
const std::string &DJIVehicleSystem::manufacturer()
{
    static const std::string _v = "DJI";
    return _v;
}

// 设备序列号，或者其他唯一绑定飞机的字符序列
const std::string &DJIVehicleSystem::uniqueCode()
{
    return _impl->_unique_code;
}

// 机器人的名字
const std::string &DJIVehicleSystem::robotName()
{
    return _impl->_unique_code;
}

DJIVehicleModels DJIVehicleSystem::model()
{
    return _impl->_model;
}

DJIVehiclePtr DJIVehicleSystem::vehicle()
{
    return _impl->_dji_vehicle;
}
