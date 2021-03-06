#pragma once
#include "p_rsdk/plugins/StateDelegation.hpp"
#include "rsdk/message/sensor_msg/msg_coordinate.h"
#include "DJIWPMission.hpp"
#include <vector>
#include <utility>

using CoordinateIndexPair = std::pair<sensor_msg::Coordinate, uint32_t>;

/**
 * @brief DelegateMemory 是将航线生命周期延续到Proxy生命周期的方式
 * 
 */
struct DJIDelegateMemory : rsdk::DelegateMemory
{
    // 转换后的任务
    DJIWPMission                        dji_mission;

    // 需要拍照的航点
    std::vector<CoordinateIndexPair>    dji_photo_point;

    // action id 去重
    std::vector<bool>                   action_id_filter;

    // 已下载照片数
    uint32_t                            photo_download_number{0};

    // 起飞点海拔高度
    double                              takeoff_altitude{0};

    // 总共航点
    uint32_t                            total_wp{0};

    // 目前的重复次数
    uint32_t                            current_repeated_times{0};

    // 总共重复次数
    uint32_t                            total_repeated_times{0};
};
