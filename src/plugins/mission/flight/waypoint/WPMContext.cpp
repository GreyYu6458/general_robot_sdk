#include "rsdk/plugins/mission/flight/waypoint/WPMContext.hpp"

namespace rsdk::mission::flight::waypoint
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
        std::shared_ptr<WPMission> _desc;
    };

    WPMContext::WPMContext(const std::shared_ptr<WPMission>& desc)
    :   ::rsdk::mission::MissionContext(desc), 
        _impl(new Impl(desc))
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
}