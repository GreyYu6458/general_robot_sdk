/**
 * @brief 这是一个非公开的头，不要直接在其他头文件里包含这个头文件
 *        除非另一个也是非公开的头文件
 */

#pragma once
#include "rsdk/proxy/StateDelegation.hpp"
#include "tasks/MavMainTask.hpp"

struct MavMissionDelegateMemory : public rsdk::DelegateMemory
{
    MavMissionItemList item_list;
};
