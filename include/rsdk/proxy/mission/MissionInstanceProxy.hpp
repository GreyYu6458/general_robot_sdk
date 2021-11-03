#pragma once

#include <string>
#include "rsdk/proxy/BaseProxy.hpp"

namespace rsdk::mission
{
    class MissionInstance : public BaseProxy
    {
    public:
        MissionInstance(
            const std::shared_ptr<RobotSystem>&, 
            const std::shared_ptr<BasePlugin>&
        );

        void startMission();

        void setId(const std::string&);

        bool resetState();

        const std::string& id();
    };
} // namespace rsdk::mission

