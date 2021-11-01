#pragma once
#include "DJIWPMission.hpp"

struct DJIMissionSharedInfo
{
    DJIWPMission    dji_wp_mission;             // DJI 航线任务

    uint32_t        total_wp;                   // 总共航点

    uint32_t        current_repeated_times;     // 目前的重复次数

    uint32_t        total_repeated_times;       // 总共重复次数

    DJIMissionSharedInfo() = default;

    DJIMissionSharedInfo(const DJIMissionSharedInfo&) = delete;

    DJIMissionSharedInfo& operator=(const DJIMissionSharedInfo&) = delete;
};