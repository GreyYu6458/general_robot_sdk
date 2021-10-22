#ifndef _BASE_MISSION_HPP_
#define _BASE_MISSION_HPP_

namespace rsdk::mission
{
    class MissionDescription
    {
    public:
        MissionDescription();

        virtual ~MissionDescription();

    private:
        class Impl;
        Impl* _impl;
    };
}

#endif