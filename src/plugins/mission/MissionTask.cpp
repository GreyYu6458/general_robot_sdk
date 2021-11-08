#include "p_rsdk/plugins/mission/MissionTask.hpp"
#include "p_rsdk/plugins/mission/TaskListener.hpp"
#include "p_rsdk/plugins/mission/InstancePlugin.hpp"
#include <thread>
#include <atomic>

namespace rsdk::mission
{
    class MissionTask::Impl
    {
    public:
        ~Impl()
        {
            if (task_thread.joinable())
            {
                task_thread.join();
            }
        }

        bool                    is_main;
        bool                    is_running{false};
        TaskListener*           listener{nullptr};
        std::string             task_name;
        std::thread             task_thread;
        rsdk::mission::StageRst start_stage_rst{StageRstType::UNEXECUTE, "UNEXECUTE"};
        rsdk::mission::StageRst executing_stage_rst{StageRstType::UNEXECUTE, "UNEXECUTE"};
    };

    MissionTask::MissionTask(
        const std::string &_task_name,
        bool is_main)
        : _impl(new Impl())
    {
        _impl->task_name    = _task_name;
        _impl->is_main      = is_main;
    }

    bool MissionTask::isRunning() const
    {
        return _impl->is_running;
    }
    
    MissionTask::~MissionTask()
    {
        delete _impl;
    }

    const std::string& MissionTask::taskName() const
    {
        return _impl->task_name;
    }

    bool MissionTask::isMain() const
    {
        return _impl->is_main;
    }

    /**
     * @brief 
     * 
     */
    void MissionTask::execute(TaskListener* listerner)
    {
        if(_impl->is_running)
            return; // TODO 返回正在运行警告

        else if(!_impl->is_running && _impl->task_thread.joinable())
        {
            _impl->task_thread.join();
        }

        _impl->is_running = true;
        this->_impl->listener = listerner;
        std::thread(
            [this]()
            {
                _impl->start_stage_rst = this->start_stage();

                if(this->_impl->listener)
                    this->_impl->listener->OnStartStageFinished(this, _impl->start_stage_rst);

                if(_impl->start_stage_rst.type != StageRstType::SUCCESS)
                {
                    _impl->is_running = false;
                    return;
                }

                _impl->executing_stage_rst = this->executing_stage();

                if(this->_impl->listener)
                    this->_impl->listener->OnExecutingStageFinished(this, _impl->executing_stage_rst);

                _impl->is_running = false;
            }
        ).swap(_impl->task_thread);
    }
}