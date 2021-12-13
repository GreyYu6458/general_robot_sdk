#pragma once

namespace rsdk::mission::waypoint
{
    enum class MavMissionItems
    {
        NAV_WAYPOINT                = 16,
        NAV_LOITER_UNLIM            = 17,
        NAV_LOITER_TURNS            = 18,
        NAV_LOITER_TIME             = 19,
        NAV_RETURN_TO_LAUNCH        = 20,
        NAV_LAND                    = 21,
        NAV_TAKEOFF                 = 22,
        NAV_LAND_LOCAL              = 23,
        NAV_TAKEOFF_LOCAL           = 24,
        NAV_FOLLOW                  = 25,
        NAV_CONTINUE_AND_CHANGE_ALT = 30,
        IMAGE_START_CAPTURE         = 2000,
        IMAGE_STOP_CAPTURE          = 2001
    };


}