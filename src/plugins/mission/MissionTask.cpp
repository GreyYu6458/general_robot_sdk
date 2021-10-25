#include "p_rsdk/plugins/mission/MissionTask.hpp"
#include <thread>
#include <atomic>

namespace rsdk::mission
{
    class MissionTask::Impl
    {
    public:
        ~Impl()
        {
            if(task_thread.joinable())
                task_thread.join();
        }

        bool               is_main;
        TaskObject         task_object;
        TaskFinishedCb     on_finished_cb;
        std::string        task_name;
        std::thread        task_thread;
        std::atomic<bool>  is_finished{false};
    };

    MissionTask::MissionTask(
        const std::string& _task_name, 
        const TaskObject& _func,
        bool is_main
    )
    : _impl(new Impl())
    {
        _impl->task_name = _task_name;
        _impl->task_object = _func;
        _impl->is_main     = is_main;
    }

    MissionTask::~MissionTask()
    {
        delete _impl;
    }

    std::string MissionTask::taskName() const
    {
        return _impl->task_name;
    }

    bool MissionTask::isDone() const
    {
        return _impl->is_finished;
    }

    bool MissionTask::isMain() const
    {
        return _impl->is_main;
    }

    /**
     * @brief 
     * 
     */
    void MissionTask::start()
    {
        std::thread(
            [this]()
            {
                // launch task
                auto rst = this->_impl->task_object();

                // invoke finished callback if enable
                if(this->_impl->on_finished_cb)
                {
                    this->_impl->on_finished_cb(this->_impl->task_name, rst);
                }

                // set finished flag
                this->_impl->is_finished = true;
            }
        ).swap(_impl->task_thread);
    }

    void MissionTask::onFinished(const TaskFinishedCb& on_finish)
    {
        _impl->on_finished_cb = on_finish;
    }
}