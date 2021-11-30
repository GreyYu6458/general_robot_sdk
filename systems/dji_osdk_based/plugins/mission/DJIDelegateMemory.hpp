#pragma once
#include "rsdk/proxy/StateDelegation.hpp"
#include "rsdk/message/sensor_msg/msg_coordinate.h"
#include "DJIWPMission.hpp"
#include <vector>
#include <utility>

using CoordinateIndexPair = std::pair<sensor_msg::Coordinate, uint32_t>;

struct DJIDelegateMemory : rsdk::DelegateMemory
{
    // 转换后的任务
    DJIWPMission                        dji_mission;

    // 需要拍照的航点
    std::vector<CoordinateIndexPair>    dji_photo_point;

    // 起飞点海拔高度
    double                              takeoff_altitude{0};

    // 总共航点
    uint32_t                            total_wp{0};

    // 目前的重复次数
    uint32_t                            current_repeated_times{0};

    // 总共重复次数
    uint32_t                            total_repeated_times{0};
};