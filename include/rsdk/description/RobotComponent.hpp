#ifndef _ROBOT_DESCRIPTION_HPP_
#define _ROBOT_DESCRIPTION_HPP_
#include <string>
#include <unordered_map>
#include "Material.hpp"

namespace rsdk::description
{
    struct RobotComponent
    {
        std::string                                             name;

        Material                                                material;

        std::unordered_map<std::string, RobotComponent>         linkedCmpnt;
    };
}

#endif