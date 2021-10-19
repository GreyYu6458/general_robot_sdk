#ifndef _MISSION_CONTEXT_HPP_
#define _MISSION_CONTEXT_HPP_
#include <memory>
#include <functional>
#include "MissionDescription.hpp"

namespace rsdk::mission
{
    class BaseExecutor;
    class MissionDescription;

    enum class MissionState
    {
        Executing,
        Background,
        Finish,
        STATE_COUNT
    };

    class MissionContext
    {
        friend class BaseExecutor;

    public:
        MissionContext(const std::shared_ptr<MissionDescription>& description);

        virtual ~MissionContext();

        MissionState state();

        const std::shared_ptr<MissionDescription>& description() const;

    protected:
        void setState(MissionState state);

    private:
        class Impl;
        Impl* _impl;
    };
}

#endif