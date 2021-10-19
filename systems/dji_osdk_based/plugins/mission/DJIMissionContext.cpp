#include "DJIMissionContext.hpp"
#include "DJIWPMission.hpp"

class DJIMissionContext::Impl
{
    friend class DJIMissionContext;
public:
    Impl(const std::shared_ptr<DJIWPMission>& desc)
    : dji_wp_mission(desc){

    }

private:
    std::shared_ptr<DJIWPMission> dji_wp_mission;
};

DJIMissionContext::DJIMissionContext(const std::shared_ptr<DJIWPMission>& desc)
: rmfw::WPMContext(desc->rawMission()), _impl(new Impl(desc))
{

}

DJIMissionContext::~DJIMissionContext()
{
    delete _impl;
}

const std::shared_ptr<DJIWPMission>& DJIMissionContext::djiWPMission()
{
    return _impl->dji_wp_mission;
}