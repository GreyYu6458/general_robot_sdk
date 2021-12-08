#ifndef _DJI_VEHICLE_SYSTEM_HPP_
#define _DJI_VEHICLE_SYSTEM_HPP_
#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/message/sensor_msg/msg_coordinate.h"
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
    /**
     * @brief Construct a new DJIVehicleSystem object
     * 
     */
    DJIVehicleSystem();

    /**
     * @brief Destroy the DJIVehicleSystem object
     * 
     */
    ~DJIVehicleSystem();

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
     * @brief 机器人的名字
     * 
     * @return const std::string& 
     */
    const std::string &robotName() override;

    /**
     * @brief 获取相机管理器
     * 
     * @return DJICameraManager& 
     */
    DJICameraManager &cameraManager();

    /**
     * @brief 大疆API MUTEX
     * 
     * @return std::mutex& 
     */
    std::mutex& DJIAPIMutex();

    /**
     * @brief 返回大疆底层指针
     * 
     * @return DJIVehiclePtr 
     */
    DJIVehiclePtr vehicle();

    /**
     * @brief 返回大疆飞机类型
     * 
     * @return DJIVehicleModels 
     */
    DJIVehicleModels model();

    /**
     * @brief 
     * 
     */
    const sensor_msg::Coordinate& lastUpdatePosition();

    /**
     * @brief 
     * 
     */
    void uploadPosition(const sensor_msg::Coordinate&);

protected:
    /**
     * @brief 尝试链接
     * 
     * @param config 
     * @return true 
     * @return false 
     */
    bool tryLink(const rsdk::SystemConfig& config) override;

    /**
     * @brief overload base class
     * 
     * @return std::shared_ptr<DJIVehicleSystem> 
     */
    std::shared_ptr<DJIVehicleSystem> shared_from_this();

private:

    void _regist_osdk_info();

    class SystemImpl;
    SystemImpl *_impl;
};

#endif
