#ifndef _MAV_BASED_VEHICLE_SYSTEM_HPP_
#define _MAV_BASED_VEHICLE_SYSTEM_HPP_
#include "rsdk/system/RobotSystem.hpp"

namespace mavsdk
{
    class System;
    class Telemetry;
}

class MavBasedVehicleSystem : public rsdk::RobotSystem
{
public:
    /**
     * @brief Construct a new Mav Based Vehicle System object
     * 
     */
    MavBasedVehicleSystem();

    /**
     * @brief Destroy the Mav Based Vehicle System object
     * 
     */
    ~MavBasedVehicleSystem() override;

    /**
     * @brief 设备的生产商信息
     * 
     * @return const std::string& 
     */
    const std::string &manufacturer() override;

    /**
     * @brief 设备序列号，或者其他唯一绑定飞机的字符序列
     * 
     * @return const std::string& 
     */
    const std::string &uniqueCode() override;

    /**
     * @brief 获得机器人的名字
     * 
     * @return const std::string& 
     */
    const std::string &robotName() override;

    /**
     * @brief 获得mavsdk::system的指针
     * 
     * @return std::shared_ptr<mavsdk::System> 
     */
    std::shared_ptr<mavsdk::System> mavsdkLowLayerSystem();

protected:
    bool tryLink(const rsdk::SystemConfig& config) override;

    /**
    * @brief overload base class
    *
    * @return std::shared_ptr<DJIVehicleSystem>
    */
    std::shared_ptr<MavBasedVehicleSystem> shared_from_this();

private:
    class Impl;
    Impl* _impl;
};


#endif