#include "p_rsdk/plugins/mission/InstancePlugin.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/event/MissionEvents.hpp"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <unordered_map>

namespace rsdk::mission
{
    using TaskMap = std::unordered_map< std::string, std::unique_ptr<MissionTask> >;

    class InstancePlugin::Impl
    {
    public:
        Impl(InstancePlugin* owner)
        {
            _owner = owner;  
        }

        InstancePlugin*                 _owner;
        std::string                     _id;
        std::string                     _main_task_name;
        TaskMap                         _task_map;
        std::shared_ptr<RobotSystem>    _system;

        void taskCallback(const MissionTask* task_ptr, const TaskExecutionRst& rst)
        {
            using namespace rsdk::event::mission;

            TaskInfo task_info;
            task_info.mission_info.instance_name     = this->_id;
            task_info.detail                         = std::move(rst.detail);
            task_info.execute_result                 = rst.rst_type;
            task_info.task_name                      = task_ptr->taskName();
            task_info.is_main_task                   = task_ptr->isMain();

            _system->postEvent(
                _owner, 
                std::make_shared<TaskEvent>(std::move(task_info)) 
            );

            // all task done
            if(!_task_map.size())
            {
                typename MissionFinished::payload_type mission_info;
                mission_info.instance_name = this->_id;
                _system->postEvent(
                    _owner, 
                    std::make_shared<MissionFinished>(std::move(mission_info)) 
                );
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

    InstancePlugin::InstancePlugin(const std::shared_ptr<RobotSystem>& system)
    : BasePlugin(system)
    {
        _impl = new Impl(this);
        
        // create id by uuid
        boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
        _impl->_id              = boost::uuids::to_string(a_uuid);
        _impl->_system          = system;
    }

    InstancePlugin::InstancePlugin(const std::string& id, const std::shared_ptr<RobotSystem>& system)
    : BasePlugin(system)
    {
        _impl = new Impl(this);
        _impl->_id              = id;
        _impl->_system          = system;
    }

    void InstancePlugin::setMainTask( const MainMissionTask& task)
    {
        _impl->_main_task_name  = task.taskName();
        _impl->__add_task(task.taskName(), std::make_unique<MainMissionTask>(task));
    }

    void InstancePlugin::setMainTask( const std::string& task_name, const TaskObject& taskobj)
    {
        auto task = std::make_unique<SubMissionTask>(task_name);
        task->setTask(taskobj);
        _impl->__add_task(task_name, std::move(task));
    }

    InstancePlugin::~InstancePlugin()
    {
        delete _impl;
    }

    void InstancePlugin::startMainTask()
    {
        _impl->__run_task(_impl->_main_task_name);
    }

    const std::string& InstancePlugin::id()
    {
        return _impl->_id;
    }

    InstancePlugin::RunSubtaskRst InstancePlugin::runSubtask(const SubMissionTask& task)
    {
        if(!_impl->__add_task(task.taskName(), std::make_unique<SubMissionTask>(task)))
            return RunSubtaskRst::CONFLICT;

        _impl->__run_task(task.taskName());
        return RunSubtaskRst::SUCCESS;
    }

    /**
     * @brief 
     * 
     * @param task_name 
     * @param is_main 
     */
    InstancePlugin::RunSubtaskRst InstancePlugin::runSubtask(const std::string& task_name, const TaskObject& obj)
    {
        auto task = std::make_unique<SubMissionTask>(task_name);
        task->setTask(obj);
        
        if(_impl->__add_task(task_name, std::move(task)))
            return RunSubtaskRst::CONFLICT;

        _impl->__run_task(task_name);
        return RunSubtaskRst::SUCCESS;
    }
}