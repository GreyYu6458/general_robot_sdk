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
            if (task_thread.joinable())
                task_thread.join();
        }

        bool                is_main;
        TaskObject          task_object;
        TaskFinishedCb      on_finished_cb;
        std::string         task_name;
        std::thread         task_thread;
    };

    MissionTask::MissionTask()
    {
        _impl = new Impl();
        _impl->task_name = "unknow";
        _impl->is_main   = false;
    }

    MissionTask::MissionTask(
        const std::string &_task_name,
        bool is_main)
        : _impl(new Impl())
    {
        _impl->task_name    = _task_name;
        _impl->is_main      = is_main;
    }
    
    MissionTask::MissionTask(const MissionTask& other)
    {
        _impl = new Impl();
        _impl->is_main          = other._impl->is_main;
        _impl->task_name        = other._impl->task_name;
        _impl->task_object      = other._impl->task_object;
    }

    MissionTask::MissionTask(MissionTask&& other)
    {
        _impl = new Impl();
        _impl->is_main          = other._impl->is_main;
        _impl->task_name        = std::move(other._impl->task_name);
        _impl->task_object      = std::move(other._impl->task_object);
        _impl->on_finished_cb   = std::move(other._impl->on_finished_cb);
        _impl->task_thread      = std::move(other._impl->task_thread);
    }

    MissionTask& MissionTask::operator=(const MissionTask& other)
    {
        _impl = new Impl();
        _impl->is_main          = other._impl->is_main;
        _impl->task_name        = other._impl->task_name;
        _impl->task_object      = other._impl->task_object;

        return *this;
    }

    MissionTask& MissionTask::operator=(const MissionTask&& other)
    {
        _impl->is_main          = other._impl->is_main;
        _impl->task_name        = std::move(other._impl->task_name);
        _impl->task_object      = std::move(other._impl->task_object);
        _impl->on_finished_cb   = std::move(other._impl->on_finished_cb);
        _impl->task_thread      = std::move(other._impl->task_thread);
        return *this;
    }

    MissionTask::~MissionTask()
    {
        delete _impl;
    }

    void MissionTask::setTask(const TaskObject&  _func)
    {
        _impl->task_object = _func;
    }

    bool MissionTask::emptyTask()
    {
        return _impl->task_object == nullptr;
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
    void MissionTask::execute(const TaskFinishedCb& cb)
    {
        this->_impl->on_finished_cb = cb;
        std::thread(
            [this]()
            {
                // launch task
                auto rst = this->_impl->task_object();

                // invoke finished callback if enable
                if (this->_impl->on_finished_cb)
                {
                    this->_impl->on_finished_cb(this, rst);
                }
            }
        ).swap(_impl->task_thread);
    }
}