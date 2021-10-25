#include "p_rsdk/plugins/mission/waypoint/WPMContext.hpp"
#include "WPMControllerPlugin.hpp"

namespace rsdk::mission::waypoint
{
    class WPMContext::Impl
    {
        friend class WPMContext;
    public:
        Impl(const std::shared_ptr<WPMission>& desc)
        : _desc(desc)
        {

        }

    private:
        uint16_t total_wp{0};
        uint16_t current_wp{0};

        std::shared_ptr<WPMission> _desc;
    };

    WPMContext::WPMContext(WPMControllerPlugin* executor,const std::shared_ptr<WPMission>& desc)
    : MissionContext(executor), _impl(new Impl(desc))
    {
        
    }

    WPMContext::~WPMContext()
    {
        delete _impl;
    }

    const std::shared_ptr<WPMission>& WPMContext::getWPMission()
    {
        return _impl->_desc;
    }

    uint16_t WPMContext::totalWaypoint()
    {
        return _impl->total_wp;
    }

    void WPMContext::setTotalWaypoint(uint16_t count)
    {
        _impl->total_wp;
    }

    uint16_t WPMContext::currentWaypointNumber()
    {
        return _impl->current_wp;
    }

    void WPMContext::setCurrentWaypointNumber(uint16_t count)
    {
        _impl->current_wp = count;
    }
}