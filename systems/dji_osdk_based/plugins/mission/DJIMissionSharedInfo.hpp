#pragma once
#include "DJIWPMission.hpp"
#include <unordered_map>

struct DJIMissionSharedInfo
{
    // DJI 航线任务
    DJIWPMission                                dji_wp_mission;

    // 总共航点
    uint32_t                                    total_wp{0};     

    // 目前的重复次数
    uint32_t                                    current_repeated_times{0};

    // 总共重复次数
    uint32_t                                    total_repeated_times{0};

    void init()
    {
        dji_wp_mission.clear();
        total_wp = 0;
        current_repeated_times = 0;
        total_repeated_times = 0;
    }

    DJIMissionSharedInfo() = default;

    DJIMissionSharedInfo(const DJIMissionSharedInfo&) = delete;

    DJIMissionSharedInfo& operator=(const DJIMissionSharedInfo&) = delete;
};
