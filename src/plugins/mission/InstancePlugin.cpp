#include "p_rsdk/plugins/mission/InstancePlugin.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/event/MissionEvents.hpp"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <vector>
#include <mutex>

namespace rsdk::mission
{
    using TaskMap = std::unordered_map< std::string, std::unique_ptr<MissionTask> >;

    class InstancePlugin::Impl
    {
        friend class MissionTask;
    public:
        Impl(InstancePlugin* owner)
        {
            _owner = owner;
            // set default callback
            _state_changed_cb = std::bind(&Impl::default_state_changed_cb, this, std::placeholders::_1);
        }

        void default_state_changed_cb(InstanceState state)
        {
            _system->info(
                "[instance] Instance Name :" 
                + _id + " State From "
                + std::to_string(static_cast<uint16_t>(_last_state))
                + " To "
                + std::to_string(static_cast<uint16_t>(_state))
            );
        }

        InstancePlugin*                             _owner;
        std::string                                 _id;                // instance id/name
        std::unique_ptr<MissionTask>                _main_task;         // main task
        TaskMap                                     _sub_task_map;      // sub task
        std::vector<std::unique_ptr<MissionTask>>   _end_sub_task_list; // ended sub task list
        std::function<void (InstanceState)>         _state_changed_cb;  // callback function when state changed
        InstanceState                               _last_state;        // last state
        InstanceState                               _state;             // current state
        std::shared_ptr<RobotSystem>                _system;            // system
        std::mutex                                  _state_mutex;       // mutex for state

        /**
         * @brief   对终态进行判断(FAILED和FINISHED)
         * 
         * @return InstanceState 
         */
        InstanceState real_state()
        {
            if(_state == InstanceState::FAILED && _sub_task_map.size())
            {
                return InstanceState::FAILED_WITH_SUBTASK;
            }
            else if (_state == InstanceState::FINISHED && _sub_task_map.size())
            {
                return InstanceState::FINISHED_WITH_SUBTASK;
            }
            return _state;
        }

        /**
        *          WAITTING                --(instance start)                                >> STARTING
        *          STARTING                --(success)                                       >> EXECUTING
        *          STARTING                --(failed)                                        >> FAILED(END STATE)
        *          EXECUTING               --(failed)                                        >> FAILED(END STATE)
        *          EXECUTING               --(success)                                       >> FINISHED(END)
        */

        /**
         * @brief main task 启动处理
         * 
         * @param rst 
         */
        void mainTaskStartHandle(MissionTask* task, StageRst rst)
        {
            using namespace rsdk::event::mission;

            MissionInfo mission_info;
            mission_info.instance_name  = _id;

            auto event = rsdk::event::REventPtr();
            // main task 成功开始
            
            if(rst.type == StageRstType::SUCCESS)
            {
                event = std::make_shared<MissionStartedEvent>(mission_info);
                _state = InstanceState::EXECUTING;
            }
            else
            {
                event = std::make_shared<MissionStartFailedEvent>(mission_info);
                _state = InstanceState::FAILED;
            }
            _system->postEvent(_owner, event);
        }

        /**
         * @brief   sub task 启动处理
         *          如果启动失败，将其移动到失败Task列表中
         * 
         * @param rst 
         */
        void subtaskStartHandle(MissionTask* task, StageRst rst)
        {
            if(rst.type != StageRstType::SUCCESS)
            {
                _end_sub_task_list.push_back( 
                    std::move(_sub_task_map[task->taskName()]) 
                );
                _sub_task_map.erase(task->taskName());
            }
        }

        void mainTaskExecutingHandle(MissionTask* task, StageRst rst)
        {
            using namespace rsdk::event::mission;
            auto event = rsdk::event::REventPtr();

            MissionInfo mission_info;
            mission_info.instance_name  = _id;

            if( rst.type == StageRstType::SUCCESS or 
                rst.type == StageRstType::INTERRUPTTED)
            {
                event = std::make_shared<MissionFinishedEvent>(mission_info);
                _state = InstanceState::FINISHED;
            }
            if(rst.type == StageRstType::INTERRUPTTED)
            {
                mission_info.is_interrupted = true;
            }
            else
            {
                event = std::make_shared<MissionFailedEvent>(mission_info);
                _state = InstanceState::FAILED;
            }
            _system->postEvent(_owner, event);
        }

        void subtaskExecutingHandle(MissionTask* task, StageRst rst)
        {
            if(rst.type != StageRstType::SUCCESS)
            {
                _end_sub_task_list.push_back( 
                    std::move(_sub_task_map[task->taskName()]) 
                );
                _sub_task_map.erase(task->taskName());
            }
        }

        bool __run_task(std::unique_ptr<MissionTask> task)
        {
            const std::string& name = task->taskName();
            if(_sub_task_map.count(name))
            {
                return false;
            }
            _sub_task_map[name] = std::move(task);
            _sub_task_map[name]->execute(this->_owner);
            return true;
        }
    };

    InstancePlugin::InstancePlugin(const std::shared_ptr<RobotSystem>& system)
    : BasePlugin(system)
    {
        _impl = new Impl(this);
        
        // create id by uuid
        boost::uuids::uuid a_uuid   = boost::uuids::random_generator()();
        _impl->_id                  = boost::uuids::to_string(a_uuid);
        _impl->_system              = system;
        _impl->_state               = InstanceState::WAITTING;
    }

    void InstancePlugin::setMainTask(std::unique_ptr<MainMissionTask> task)
    {
        _impl->_main_task = std::move(task);
    }

    InstancePlugin::~InstancePlugin()
    {
        delete _impl;
    }

    void InstancePlugin::startMainTask()
    {
        _impl->_main_task->execute(this);
    }

    void InstancePlugin::setStateChangedCallback(const std::function<void (InstanceState)>& cb)
    {
        _impl->_state_changed_cb = 
        [this, cb](InstanceState state)
        {
            this->_impl->default_state_changed_cb(state);
            cb(state);
        };
    }

    // TODO 和 OnExecutingStageFinished() 太过于接近，想办法抽象出来
    void InstancePlugin::OnStartStageFinished(MissionTask* task, StageRst rst)
    {
        std::lock_guard<std::mutex> lck(_impl->_state_mutex);
        // 记录状态
        _impl->_last_state = _impl->_state;

        if(task->isMain()){
            _impl->mainTaskStartHandle(task, rst);
        }else{
            _impl->subtaskStartHandle(task, rst);
        }

        _impl->_state = _impl->real_state();

        // 如果状态改变, 调用回调
        if(_impl->_last_state != _impl->_state && _impl->_state_changed_cb)
        {
            _impl->_state_changed_cb(_impl->_state);
        }
    }

    void InstancePlugin::OnExecutingStageFinished(MissionTask* task, StageRst rst)
    {
        std::lock_guard<std::mutex> lck(_impl->_state_mutex);
        // 记录状态
        _impl->_last_state = _impl->_state;
        // 交给状态转移函数处理
        // TODO 封装状态机
        if(task->isMain()){
            _impl->mainTaskExecutingHandle(task, rst);
        }else{
            _impl->subtaskExecutingHandle(task, rst);
        }

        _impl->_state = _impl->real_state();

        // 如果状态改变, 调用回调
        if(_impl->_last_state != _impl->_state && _impl->_state_changed_cb)
        {
            _impl->_state_changed_cb(_impl->_state);
        }
    }

    void InstancePlugin::setId(const std::string& id)
    {
        _impl->_id = id;
    }

    const std::string& InstancePlugin::id()
    {
        return _impl->_id;
    }

    std::unique_ptr<MissionTask>& InstancePlugin::mainTask()
    {
        return _impl->_main_task;
    }

    /**
     * @brief 返回当前状态
     * 
     * @return InstanceState 
     */
    InstanceState InstancePlugin::state()
    {
        return _impl->_state;
    }

    /**
     * @brief 运行子任务，如果同名子任务在运行，则返回RunSubtaskRst::CONFLICT
     */
    InstancePlugin::RunSubtaskRst InstancePlugin::runSubtask(std::unique_ptr<SubMissionTask> task)
    {
        if(is_end_state( _impl->real_state() ))
            return RunSubtaskRst::MISSION_HAS_FINISHED;

        return _impl->__run_task(std::move(task)) ?
            RunSubtaskRst::SUCCESS : RunSubtaskRst::CONFLICT;
    }

    bool InstancePlugin::revent(::rsdk::event::REventParam event)
    {
        return RObject::revent(event);
    }
}