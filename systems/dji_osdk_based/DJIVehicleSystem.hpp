#ifndef _DJI_VEHICLE_SYSTEM_HPP_
#define _DJI_VEHICLE_SYSTEM_HPP_
#include "rsdk/system/RobotSystem.hpp"
#include "DJICameraManager.hpp"
#include <unordered_set>
#include <mutex>

namespace DJI::OSDK
{
    class Vehicle;
}

enum class DJIVehicleModels : int8_t
{
    MODEL_M100 = 1 << 0,
    MODEL_M210 = 1 << 1,
    MODEL_M300 = 1 << 2,
    MODEL_ALL = MODEL_M100 | MODEL_M210 | MODEL_M300,
    UNKNOWN
};

using DJIVehicle = DJI::OSDK::Vehicle;
using DJIVehiclePtr = std::shared_ptr<DJIVehicle>;

class DJIPluginBase;

class DJIVehicleSystem : public rsdk::RobotSystem
{
public:
    DJIVehicleSystem();

    ~DJIVehicleSystem();

    // 飞机是否连接
    bool isLink() override;

    // 设备的生产商信息
    const std::string &manufacturer() override;

    // 设备序列号，或者其他唯一绑定飞机的字符序列
    const std::string &uniqueCode() override;

    // 机器人的名字
    const std::string &robotName() override;

    // 获取相机管理器
    DJICameraManager &cameraManager();

    // 大疆API MUTEX
    std::mutex& DJIAPIMutex();

    // 返回大疆底层指针
    DJIVehiclePtr vehicle();

    // 返回大疆类型
    DJIVehicleModels model();

protected:
    bool tryLink(const rsdk::SystemConfig& config) override;

    /**
     * @brief overload base class
     * 
     * @return std::shared_ptr<DJIVehicleSystem> 
     */
    std::shared_ptr<DJIVehicleSystem> shared_from_this();

private:
    template<class T, class G>
    bool dji_regist_plugin(const std::shared_ptr<G>& impl)
    {
        static_assert(
            std::is_base_of<DJIPluginBase, G>::value, 
            "input type is not the base of DJIPlugin" 
        );
        // not support this model of dji

        int8_t vehicle_model        = static_cast<int8_t>(model());
        int8_t plugin_model         = static_cast<int8_t>(impl->supportModel());
        int8_t is_support_plugin    = vehicle_model & plugin_model;

        if(!is_support_plugin)
        {
            publishInfo<rsdk::SystemInfoLevel::WARNING>( 
                "not support:\t" + std::string(typeid(G).name()) 
            );   
        }
        else if (registInterfaceImplToMap<T>(impl))
        {
            publishInfo<rsdk::SystemInfoLevel::INFO>( 
                "regist    :\t" + std::string(typeid(G).name())
            );
            return true;
        }
        return false;
    }

    void _regist_osdk_info();

    class SystemImpl;
    SystemImpl *_impl;
};

#endif
