#pragma once

#include "REvent.hpp"

EventField(
    EventCategory::PAYLOAD,
    rsdk::event::payload,
    PayloadEvent,
    /* enums */
    CAMERA_TAKEN_PHOTO,
    CAMERA_SAVED_PHOTO,
    START_RECORD,
    STOP_RECORD
)

