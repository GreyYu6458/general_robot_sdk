#ifndef _MAV_BASED_VEHICLE_SYSTEM_HPP_
#define _MAV_BASED_VEHICLE_SYSTEM_HPP_
#include "rsdk/system/RobotSystem.hpp"

class MavBasedVehicleSystem : public rsdk::RobotSystem
{
public:
    MavBasedVehicleSystem();

    ~MavBasedVehicleSystem();

    bool isLink() override;

    // 设备的生产商信息
    const std::string &manufacturer() override;

    // 设备序列号，或者其他唯一绑定飞机的字符序列
    const std::string &uniqueCode() override;

    // 机器人的名字
    const std::string &robotName() override;

protected:
    bool tryLink(const rsdk::SystemConfig& config) override;

private:
    class Impl;

    Impl* _impl;
};


#endif