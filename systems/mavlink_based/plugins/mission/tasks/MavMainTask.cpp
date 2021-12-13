#include "MavMainTask.hpp"
#include "MavBasedVehicleSystem.hpp"
#include "../MavMissionInstance.hpp"
#include "../MavMissionDelegateMemory.hpp"
#include <mavsdk/plugins/mission/mission.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <thread>
#include <atomic>

class MavWPMMainTask::Impl
{
public:
    Impl(MavWPMMainTask* owner, MavMissionInstance *instance)
    {
        auto mav_system = std::static_pointer_cast<MavBasedVehicleSystem>(instance->system());
        _mav_telemetry = new mavsdk::Telemetry(mav_system->mavsdkLowLayerSystem());
        _instance   = instance;
        _owner      = owner;
        _context_memory = _instance->currentDelegateMemory();
    }

    ~Impl()
    {
        if(_query_finished_thread.joinable())
        {
            _query_finished_quit = true;
            _query_finished_thread.join();
        }
        delete _mav_telemetry;
    }

    void startLaunch(rsdk::mission::StageRst& ret)
    {
        std::stringstream _string_stream;
        // upload mission
        auto mission_rst = _instance->mavsdk_mission_raw_handle().upload_mission(
                _context_memory->item_list
        );

        if(mission_rst != mavsdk::MissionRaw::Result::Success)
        {
            _string_stream << mission_rst;
            ret.detail  = _string_stream.str();
            ret.type    = rsdk::mission::StageRstType::FAILED;
            return;
        }

        mission_rst = _instance->mavsdk_mission_raw_handle().start_mission();
        if(mission_rst != mavsdk::MissionRaw::Result::Success)
        {
            _string_stream << mission_rst;
            ret.detail  = _string_stream.str();
            ret.type    = rsdk::mission::StageRstType::FAILED;
            return;
        }

        addFinishListener();

        ret.type            = rsdk::mission::StageRstType::SUCCESS;
    }

    void addFinishListener()
    {
        _query_finished_quit = false;
        std::thread(
            [this]()
            {
                // waitting for vehicle armed
                while(_mav_telemetry->armed() && !_query_finished_quit)
                {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                rsdk::mission::StageRst rst;
                rst.type    = rsdk::mission::StageRstType::SUCCESS;
                rst.detail  = "Mission Success Finished";
                _owner->notifyMissionFinish(rst);
            }
        ).swap(_query_finished_thread);
    }

    void waitForMissionFinished()
    {
        std::unique_lock<std::mutex> ulck(_wait_finished_mutex);

        while(_stage_rst.type == rsdk::mission::StageRstType::UNEXECUTE)
        {
            _wait_finished_cv.wait(ulck);
        }
    }

    MavMissionInstance*                         _instance;
    MavWPMMainTask*                             _owner;
    mavsdk::Telemetry*                          _mav_telemetry;
    std::thread                                 _query_finished_thread;
    std::atomic<bool>                           _query_finished_quit{false};
    std::shared_ptr<MavMissionDelegateMemory>   _context_memory;
    std::mutex                                  _wait_finished_mutex;
    std::condition_variable                     _wait_finished_cv;
    rsdk::mission::StageRst                     _stage_rst{rsdk::mission::StageRstType::UNEXECUTE};
};

MavWPMMainTask::MavWPMMainTask(MavMissionInstance *instance)
:rsdk::mission::MainMissionTask("MAVBASED Vehicle Waypoint Mission")
{
    _impl = new Impl(this, instance);
}

MavWPMMainTask::~MavWPMMainTask()
{
    delete _impl;
}

void MavWPMMainTask::notifyMissionFinish(const rsdk::mission::StageRst &rst)
{
    {
        std::lock_guard<std::mutex> lck(_impl->_wait_finished_mutex);
        _impl->_stage_rst = rst;
    }

    _impl->_wait_finished_cv.notify_all();
}

rsdk::mission::StageRst MavWPMMainTask::start_stage()
{
    rsdk::mission::StageRst rst;

    _impl->startLaunch(rst);

    return rst;
}

rsdk::mission::StageRst MavWPMMainTask::executing_stage()
{
    _impl->waitForMissionFinished();

    return _impl->_stage_rst;
}
