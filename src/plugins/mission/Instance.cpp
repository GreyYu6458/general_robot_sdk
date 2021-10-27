#include "p_rsdk/plugins/mission/Instance.hpp"
#include "p_rsdk/plugins/mission/events/TaskFinishedEvent.hpp"
#include "p_rsdk/plugins/mission/events/TaskStartedEvent.hpp"
#include "p_rsdk/plugins/mission/events/AllTaskFinishedEvent.hpp"
#include "rsdk/system/RobotSystem.hpp"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <unordered_map>

namespace rsdk::mission
{
    using TaskMap = std::unordered_map< std::string, std::unique_ptr<MissionTask> >;

    class Instance::Impl
    {
    public:
        Impl(Instance* owner)
        {
            _owner = owner;  
        }

        Instance*                       _owner;
        std::string                     _id;
        std::string                     _main_task_name;
        TaskMap                         _task_map;
        std::shared_ptr<RobotSystem>    _system;
        std::unique_ptr<Description>    _description;

        void taskCallback(const MissionTask* task_ptr, const TaskExecutionRst& rst)
        {
            const auto& task_name = task_ptr->taskName();
            bool is_main = task_ptr->isMain();
            auto event = ::rsdk::event::REventPtr();
            switch(rst.rst)
            {
                case TaskExecutionRstType::START_FAILED:
                {
                    event = std::make_shared<::rsdk::mission::TaskStartedEvent>(
                        task_name,
                        is_main,
                        false,
                        rst.detail
                    );
                    _system->info("[task] :" + task_name + " start failed");
                    break;
                }
                case TaskExecutionRstType::TASK_INTERRUPTTED:
                {
                    event = std::make_shared<::rsdk::mission::TaskFinishedEvent>(
                        task_name,
                        is_main,
                        false,
                        rst.detail
                    );
                    _system->info("[task] :" + task_name + " interrupted");
                    break;
                }
                case TaskExecutionRstType::SUCCESS:
                {
                    event = std::make_shared<::rsdk::mission::TaskFinishedEvent>(
                        task_name,
                        is_main,
                        true,
                        rst.detail
                    );
                    _system->info("[task] :" + task_name + " start success");
                    break;
                }
            }
            // send task finished event
            if(event)
            {
                _system->postEvent(_owner, event);
            }
        }
        

        bool __add_task(const std::string& name,  std::unique_ptr<MissionTask> task)
        {
            if(!_task_map.count(name))
            {
                _task_map[name] = std::move(task);
                return true;
            }
            return false;
        }

        void __run_task(const std::string& name)
        {
            _task_map[name]->execute(
                std::bind( 
                    &Impl::taskCallback, 
                    this,
                    std::placeholders::_1, 
                    std::placeholders::_2
                )
            );
        }
    };

    Instance::Instance(const std::shared_ptr<RobotSystem>& system)
    {
        _impl = new Impl(this);
        
        // create id by uuid
        boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
        _impl->_id              = boost::uuids::to_string(a_uuid);
        _impl->_system          = system;
    }

    Instance::Instance(const std::string& id, const std::shared_ptr<RobotSystem>& system)
    {
        _impl = new Impl(this);
        _impl->_id              = id;
        _impl->_system          = system;
    }

    void Instance::setMainTask( const MainMissionTask& task)
    {
        _impl->_main_task_name  = task.taskName();
        _impl->__add_task(task.taskName(), std::make_unique<MainMissionTask>(task));
    }

    void Instance::setMainTask( const std::string& task_name, const TaskObject& taskobj)
    {
        auto task = std::make_unique<SubMissionTask>(task_name);
        task->setTask(taskobj);
        _impl->__add_task(task_name, std::move(task));
    }

    Instance::~Instance()
    {
        delete _impl;
    }

    void Instance::startMainTask()
    {
        _impl->__run_task(_impl->_main_task_name);
    }

    const std::string& Instance::id()
    {
        return _impl->_id;
    }

    Instance::AddTaskRst Instance::runSubtask(const SubMissionTask& task)
    {
        if(!_impl->__add_task(task.taskName(), std::make_unique<SubMissionTask>(task)))
            return AddTaskRst::CONFLICT;

        _impl->__run_task(task.taskName());
        return AddTaskRst::SUCCESS;
    }

    /**
     * @brief 重载的
     * 
     * @param task_name 
     * @param is_main 
     */
    Instance::AddTaskRst Instance::runSubtask(const std::string& task_name, const TaskObject& obj)
    {
        auto task = std::make_unique<SubMissionTask>(task_name);
        task->setTask(obj);
        if(_impl->__add_task(task_name, std::move(task)))
            return AddTaskRst::CONFLICT;

        _impl->__run_task(task_name);
        return AddTaskRst::SUCCESS;
    }
}