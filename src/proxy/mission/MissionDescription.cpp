#include "rsdk/proxy/mission/MissionDescription.hpp"

namespace rsdk::mission
{
    class MissionDescription::Impl
    {

    };


    MissionDescription::MissionDescription()
    : _impl(new Impl())
    {

    };

    MissionDescription::~MissionDescription()
    {
        delete _impl;
    }
}