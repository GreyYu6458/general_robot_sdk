#include "DJIWPMission.hpp"
#include <vector>
#include <unordered_map>

class DJIWPMission::Impl
{
public:
    std::vector<DJI::OSDK::WaypointV2>          _dji_wps;
    std::vector<DJI::OSDK::DJIWaypointV2Action> _dji_actions;
    std::unordered_map<size_t, DJIActionEvent>  _action_map;
    // 航点序号和item_index的表
    std::vector<uint32_t>                       _wp_index_seq_list;
    bool                                        _is_valid{true};
    bool                                        _autoReturnHome{false};

    void clear()
    {
        _dji_wps.clear();
        _wp_index_seq_list.clear();
        _dji_actions.clear();
        _action_map.clear();
    }
};

/******************************* public method *******************************/

DJIWPMission::DJIWPMission()
{
    _impl = new Impl();
}

DJIWPMission::DJIWPMission(const DJIWPMission& other)
{
    _impl = new Impl(*other._impl);
}

DJIWPMission::DJIWPMission(DJIWPMission&& other) noexcept
{
    _impl = new Impl(std::move(*other._impl));
}

DJIWPMission& DJIWPMission::operator=(const DJIWPMission& other)
{
    *this->_impl = *other._impl;
    return *this;
}

DJIWPMission& DJIWPMission::operator=(DJIWPMission&& other)
{
    *this->_impl = std::move(*other._impl);
    return *this;
}

DJIWPMission::~DJIWPMission()
{
    delete _impl;
}

bool DJIWPMission::autoReturnHome()
{
    return _impl->_autoReturnHome;
}

std::vector< DJI::OSDK::WaypointV2 >& DJIWPMission::djiWayPoints()
{
    return _impl->_dji_wps;
}

std::vector< DJI::OSDK::DJIWaypointV2Action>& DJIWPMission::djiActions()
{
    return _impl->_dji_actions;
}

void DJIWPMission::clear()
{
    _impl->clear();
}

bool DJIWPMission::eventType(size_t action_id, DJIActionEvent& dji_action_event)
{
    return _impl->_action_map.count(action_id) != 0 &&
        (dji_action_event = _impl->_action_map[action_id], true);
}

bool DJIWPMission::wpIndex2Sequence(uint32_t wp_index, uint32_t& sequence)
{
    return _impl->_wp_index_seq_list.size() > wp_index &&
        (sequence = _impl->_wp_index_seq_list[wp_index], true);
}

bool DJIWPMission::isValid()
{
    return _impl->_is_valid;
}

std::vector<uint32_t>& DJIWPMission::wpIndexSeqList()
{
    return _impl->_wp_index_seq_list;
}

void DJIWPMission::addActionEvent(size_t index, const DJIActionEvent& event)
{
    _impl->_action_map[index] = event;
}

void DJIWPMission::setAutoReturn(bool b)
{
    _impl->_autoReturnHome = b;
}

void DJIWPMission::setIsVaild(bool b)
{
    _impl->_is_valid = b;
}