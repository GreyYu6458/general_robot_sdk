#pragma once
#include "rsdk/proxy/StateDelegation.hpp"
#include <mavsdk/plugins/mission_raw/mission_raw.h>

/**
 * @brief 这是一个非公开的头，不要直接在其他头文件里包含这个头文件
 *        除非另一个也是非公开的头文件
 */

using MavMissionItemList = std::vector<mavsdk::MissionRaw::MissionItem>;

struct MavMissionDelegateMemory : public rsdk::DelegateMemory
{
    MavMissionItemList item_list;
};
