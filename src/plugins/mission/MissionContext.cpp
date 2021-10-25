#include "p_rsdk/plugins/mission/MissionContext.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include "p_rsdk/plugins/mission/BaseMIssionControllerPlugin.hpp"

#include "p_rsdk/plugins/mission/events/TaskFinishedEvent.hpp"
#include "p_rsdk/plugins/mission/events/TaskStartedEvent.hpp"
#include "p_rsdk/plugins/mission/events/AllTaskFinishedEvent.hpp"

#include <unordered_map>
#include <mutex>

namespace rsdk::mission
{
    class MissionContext::Impl
    {
        friend class MissionContext;
    public:
        Impl()
        {}

        BaseMIssionControllerPlugin* executor;
        std::unordered_map<std::string, std::unique_ptr<MissionTask>>   task_map;
        MissionState                                                    current_state{MissionState::Executing};
        std::mutex                                                      add_task_mutex;
    };

    MissionContext::MissionContext(BaseMIssionControllerPlugin* executor)
    : _impl(new Impl())
    {
        _impl->executor = executor;
    }

    MissionContext::~MissionContext()
    {
        delete _impl;
    }

    MissionState MissionContext::state()
    {
        return _impl->current_state;
    }

    void MissionContext::setState(MissionState state)
    {
        _impl->current_state = state;
    }

    /**
     * @brief 是否含有指定名称的Task
     * 
     * @param task_name 
     * @return true 
     * @return false 
     */
    bool MissionContext::hasTask(const std::string& task_name)
    {
        return _impl->task_map.count(task_name) != 0;
    }

    /**
     * @brief 查询指定名称的Task是否完成
     * 
     * @param task_name 
     * @return true 
     * @return false 
     */
    bool MissionContext::isTaskDone(const std::string& task_name)
    {
        if(  _impl->task_map.count(task_name) != 0 )
        {
            return _impl->task_map[task_name]->isDone();
        }
        return false;
    }

    /**
     * @brief 添加一个Task，通过Task来管理任务。如果Task重名，则会出现以下几种情况:
     *        1.不存在重复名称，添加正常
     *        2.名称重复，但原Task已经执行完毕，则新的任务会覆盖已完成的任务
     *        3.名称重复，但原Task还在执行，则添加失败
     * 
     * @param task 
     * @return true 
     * @return false 
     */
    MissionContext::AddTaskRst MissionContext::addTask(std::unique_ptr<MissionTask> task)
    {
        // 不存在重复名称，添加正常,返回true
        if(_impl->task_map.count(task->taskName()) == 0)
        {
            _add_task(task);
            return MissionContext::AddTaskRst::SUCCESS;
        }
        // MainTask只能被添加一次
        else if(_impl->task_map[task->taskName()]->isMain())
        {
            return MissionContext::AddTaskRst::MAIN_TASK_ONLY_ADD_ONCE;
        }
        // 名称重复，但原Task已经执行完毕，则新的任务会覆盖已完成的任务,返回true
        else if(_impl->task_map[task->taskName()]->isDone())
        {
            _add_task(task);
            return MissionContext::AddTaskRst::SUCCESS;
        }
        // 名称重复，但原Task还在执行，则添加失败,返回false
        else
        {
            return MissionContext::AddTaskRst::SAME_NAME_TASK_RUNNING;
        }
    }

    void MissionContext::_add_task(std::unique_ptr<MissionTask>& task)
    {
        const std::string task_name = task->taskName();
        bool is_main = task->isMain();

        task->onFinished(
            [this, is_main](const std::string& name, const TaskExectionRst& rst)
            {
                // send task finished event
                _impl->executor->system()->postEvent(
                    _impl->executor,
                    std::make_shared<::rsdk::mission::TaskFinishedEvent>
                    (
                        name,
                        is_main,
                        rst.is_success,
                        rst.detail
                    )
                );

                // earse from map
                {
                    std::lock_guard<std::mutex> lck(_impl->add_task_mutex);
                    _impl->task_map.erase(name);
                }
                
                // task has last
                if(_impl->task_map.size())
                    return;

                // all finished
                _impl->executor->system()->postEvent(
                    _impl->executor,
                    std::make_shared<::rsdk::mission::AllTaskFinishedEvent>()
                );
            }
        );

        {
            std::lock_guard<std::mutex> lck(_impl->add_task_mutex);
            _impl->task_map[task_name] = std::move(task);
            _impl->task_map[task_name]->start();
            _impl->executor->system()->postEvent(
                _impl->executor,
                std::make_shared<::rsdk::mission::TaskStartedEvent>
                (
                    task_name,
                    is_main
                )
            );
        }
    }
} // namespace rsdk::mission
