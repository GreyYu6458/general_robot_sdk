#ifndef _DJI_ACTION_EVENT_HPP_
#define _DJI_ACTION_EVENT_HPP_
#include <cstdint>

enum DJIActionEventEnum
{
    TakenPhoto,
    StartRecordVideo,
    StopRecordVideo,
    Paused,
    Resumed,
    Unknown,
    ActionEventCount
};

struct DJIActionEvent
{
    DJIActionEventEnum type;
    size_t adjoint_wp;  // 原始航点中，伴随的航点(cmd type == 16)
    size_t item_index;  // 原始航点中，本身的序号
};

#endif