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
    size_t finished_times{0};
    size_t total_times{1};
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

bool DJIMissionContext::hasFinishedAllCount()
{
    return _impl->finished_times == _impl->total_times;
}

void DJIMissionContext::setFinishedCount(size_t times)
{
    _impl->finished_times = times;
}

void DJIMissionContext::setAllRepeatTimes(size_t times)
{   
    _impl->total_times = times;
}
