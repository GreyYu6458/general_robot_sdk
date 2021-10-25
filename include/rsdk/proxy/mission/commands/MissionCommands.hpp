#ifndef _BASIC_MISSION_COMMAND_HPP_
#define _BASIC_MISSION_COMMAND_HPP_
#include <string>

namespace rsdk::mission
{
    struct CMDExecutedRst
    {
        bool        is_success;
        std::string detail;
    };

    struct BasicMissionCommand
    {
        virtual void execute(CMDExecutedRst& rst) = 0;
    };
}

#endif