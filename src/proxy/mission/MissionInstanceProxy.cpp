#include "rsdk/proxy/mission/MissionInstanceProxy.hpp"
#include "p_rsdk/plugins/mission/InstancePlugin.hpp"
#include "p_rsdk/plugins/mission/MissionTask.hpp"
#include "rsdk/system/RobotSystem.hpp"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <unordered_map>
#include <mutex>

namespace rsdk::mission
{
    using TaskMap = std::unordered_map< std::string, std::unique_ptr<MissionTask> >;

    class MissionInstanceProxy::Impl
    {
    public:
        Impl(MissionInstanceProxy* owner)
        {
            _owner  = owner;
            _system = _owner->system();
            // set default callback
            _state_changed_cb = std::bind(&Impl::default_state_changed_cb, this, std::placeholders::_1);
        }

        void default_state_changed_cb(InstanceState state)
        {
            _system->info(
                "Instance Name :"
                + _id + " State From "
                + std::to_string(static_cast<uint16_t>(_last_state))
                + " To "
                + std::to_string(static_cast<uint16_t>(state))
            );
        }

        MissionInstanceProxy*                       _owner;
        std::string                                 _id;
        std::unique_ptr<MainMissionTask>            _main_task;
        TaskMap                                     _sub_task_map;
        std::vector<std::unique_ptr<MissionTask>>   _end_sub_task_list;
        InstanceState                               _last_state;
        InstanceState                               _state;
        std::shared_ptr<RobotSystem>                _system;
        std::function<void (InstanceState)>         _state_changed_cb;
        std::mutex                                  _state_mutex;
        std::mutex                                  _task_map_mutex;

        /**
         * @brief   对终态进行判断(FAILED和FINISHED)
         * 
         * @return InstanceState 
         */
        InstanceState real_state()
        {
            std::lock_guard<std::mutex> lck(_task_map_mutex);
            if(_state == InstanceState::FAILED && _sub_task_map.size())
            {
                return InstanceState::FAILED_WITH_SUBTASK;
            }
            else if (_state == InstanceState::FINISHED && _sub_task_map.size())
            {
                return InstanceState::FINISHED_WITH_SUBTASK;
            }
            else if(_state == InstanceState::FINISHED && !_sub_task_map.size())
            {
                return InstanceState::FINISHED;
            }
            return _state;
        }

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
            mission_info.detail = rst.detail;

            auto event = rsdk::event::REventPtr();
            // main task 成功开始
            
            if(rst.type == StageRstType::SUCCESS)
            {
                event = std::make_shared<MissionStartedEvent>(mission_info);
                _state = InstanceState::EXECUTING;
                _owner->system()->info("Main Task Start Success:" + task->taskName());
            }
            else
            {
                event = std::make_shared<MissionStartFailedEvent>(mission_info);
                _state = InstanceState::FAILED;
                _owner->system()->info("Main Task Start Failed:" + task->taskName());
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
                std::lock_guard<std::mutex> lck(_task_map_mutex);
                _owner->system()->info("Subtask Start Failed:" + task->taskName() + " Retail:" + rst.detail);
                _end_sub_task_list.push_back( 
                    std::move(_sub_task_map[task->taskName()]) 
                );
                _sub_task_map.erase(task->taskName());
            }
            else
            {
                _owner->system()->info("Subtask Start Success:" + task->taskName());
            }
        }

        /**
         * @brief 主任务运行完毕处理函数
         * 
         * @param task 
         * @param rst 
         */
        void mainTaskExecutingHandle(MissionTask* task, StageRst rst)
        {
            using namespace rsdk::event::mission;
            auto event = rsdk::event::REventPtr();

            MissionInfo mission_info;
            mission_info.instance_name  = _id;
            mission_info.detail = rst.detail;

            if( rst.type == StageRstType::SUCCESS or 
                rst.type == StageRstType::INTERRUPTTED)
            {
                _state = InstanceState::FINISHED;
                mission_info.is_interrupted = (rst.type == StageRstType::INTERRUPTTED);
                event = std::make_shared<MissionFinishedEvent>(mission_info);
                _owner->system()->info("MainTask Executing Success/Interrupted:" + task->taskName());
            }
            else
            {
                _state = InstanceState::FAILED;
                event = std::make_shared<MissionFailedEvent>(mission_info);
                _owner->system()->info("MainTask Executing Failed:" + task->taskName());
            }
            _system->postEvent(_owner, event);
        }

        /**
         * @brief   子任务处理完毕处理函数
         *          不管subtask有没有成功，都将其移除
         * 
         * @param task 
         * @param rst 
         */
        void subtaskExecutingHandle(MissionTask* task, StageRst rst)
        {
            using namespace rsdk::event::mission;

            {
                std::lock_guard<std::mutex> lck(_task_map_mutex);
                _end_sub_task_list.push_back( 
                    std::move(_sub_task_map[task->taskName()]) 
                );
                _sub_task_map.erase(task->taskName());
            }
            _owner->system()->info("Subtask Executing Over:" + task->taskName());
        }

        /**
         * @brief 
         * 
         * @param task 
         * @return true 
         * @return false 
         */
        bool __run_task(std::unique_ptr<MissionTask> task)
        {
            std::lock_guard<std::mutex> lck(_task_map_mutex);
            const std::string& name = task->taskName();
            if(_sub_task_map.count(name) && _sub_task_map[name]->isRunning())
            {
                return false;
            }
            _owner->system()->info("Start Run Task:" + task->taskName());
            _sub_task_map[name] = std::move(task);
            _sub_task_map[name]->execute(this->_owner);
            return true;
        }
    };

    MissionInstanceProxy::MissionInstanceProxy(
        const std::shared_ptr<RobotSystem>& system, 
        const std::shared_ptr<BasePlugin>& plugin
    ) : BaseProxy(system, plugin)
    {
        _impl = new Impl(this);

        boost::uuids::uuid a_uuid   = boost::uuids::random_generator()();
        _impl->_id                  = boost::uuids::to_string(a_uuid);
        _impl->_system              = system;
        _impl->_state               = InstanceState::WAITTING;
    }

    MissionInstanceProxy::~MissionInstanceProxy()
    {
        delete _impl;
    }

    void MissionInstanceProxy::startMission()
    {
        _impl->_main_task = plugin<InstancePlugin>()->getMainTask();
        _impl->_main_task->execute(this);
    }

    void MissionInstanceProxy::OnStartStageFinished(MissionTask* task, StageRst rst)
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

    /**
     * @brief Task Executing 阶段结束后，会调用此函数
     * 
     * @param task 
     * @param rst 
     */
    void MissionInstanceProxy::OnExecutingStageFinished(MissionTask* task, StageRst rst)
    {
        handleTaskFinished(task, rst);

        std::lock_guard<std::mutex> lck(_impl->_state_mutex);
        // 记录状态
        _impl->_last_state = _impl->_state;
        std::cout << static_cast<uint32_t>(_impl->_state) << std::endl;
        // 交给状态转移函数处理
        // TODO 封装状态机
        if(task->isMain()){
            _impl->mainTaskExecutingHandle(task, rst);
        }else{
            _impl->subtaskExecutingHandle(task, rst);
        }
        std::cout << static_cast<uint32_t>(_impl->_state) << std::endl;

        _impl->_state = _impl->real_state();

        // 如果状态改变, 调用回调
        if(_impl->_last_state != _impl->_state && _impl->_state_changed_cb)
        {
            _impl->_state_changed_cb(_impl->_state);
        }
    }

    void MissionInstanceProxy::setId(const std::string& id)
    {
        _impl->_id = id;
    }

    const std::string& MissionInstanceProxy::id()
    {
        return _impl->_id;
    }

    InstanceState MissionInstanceProxy::state()
    {
        return _impl->_state;
    }

    void MissionInstanceProxy::setStateChangedCallback(const std::function<void (InstanceState)>& cb)
    {
        _impl->_state_changed_cb =
        [this, cb](InstanceState state)
        {
            this->_impl->default_state_changed_cb(state);
            cb(state);
        };
    }

    bool MissionInstanceProxy::runSubTask(std::unique_ptr<SubMissionTask> task)
    {
        return _impl->__run_task(std::move(task));
    }

    bool MissionInstanceProxy::hasSubTask(const std::string& task_name)
    {
        std::lock_guard<std::mutex> lck(_impl->_task_map_mutex);
        return _impl->_sub_task_map.count(task_name);
    }

    std::unique_ptr<MainMissionTask>& MissionInstanceProxy::mainTask()
    {
        return _impl->_main_task;
    }
}