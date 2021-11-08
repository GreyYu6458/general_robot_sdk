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

    // 是否获得了第一个照片
    bool                                        get_first_photo{false};

    // 照片的偏移时间,这个是根据第一个照片得出的
    int64_t                                     photo_bias_time{0};

    // 照片事件时间和item_index的表
    std::vector<std::pair<int64_t, uint32_t>>   photo_time_item_index_list;

    DJIMissionSharedInfo() = default;

    DJIMissionSharedInfo(const DJIMissionSharedInfo&) = delete;

    DJIMissionSharedInfo& operator=(const DJIMissionSharedInfo&) = delete;
};
