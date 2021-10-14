#ifndef _ROBOT_MISSION_HPP_
#define _ROBOT_MISSION_HPP_
#include "BaseMission.hpp"
#include <memory>

namespace rsdk::mission
{
    class RobotMission : public BaseMission
    {
    public:
        RobotMission();

        virtual ~RobotMission();

    private:
        class Impl;
        Impl* impl;
    };
}

#endif