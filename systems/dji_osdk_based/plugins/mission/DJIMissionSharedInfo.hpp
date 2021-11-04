#pragma once
#include "DJIWPMission.hpp"
#include <unordered_map>

struct DJIMissionSharedInfo
{
    DJIWPMission                                dji_wp_mission;                 // DJI 航线任务

    uint32_t                                    total_wp{0};                   // 总共航点

    uint32_t                                    current_repeated_times{0};     // 目前的重复次数

    uint32_t                                    total_repeated_times{0};       // 总共重复次数

    std::vector<std::pair<int64_t, uint32_t>>   photo_time_item_index_list;     // 照片事件时间和item_index的表

    DJIMissionSharedInfo() = default;

    DJIMissionSharedInfo(const DJIMissionSharedInfo&) = delete;

    DJIMissionSharedInfo& operator=(const DJIMissionSharedInfo&) = delete;
};
