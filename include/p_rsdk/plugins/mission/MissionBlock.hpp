#ifndef _MISSION_BLOCK_HPP_
#define _MISSION_BLOCK_HPP_
#include "p_rsdk/plugins/BasePlugin.hpp"

namespace rsdk
{
    class RobotSystem;
}

namespace rsdk::mission
{
    enum class MissionCMD
    {
        START,
        PAUSE,
        RESUME,
        STOP
    };

    enum class CMDExecuteRstType
    {
        SUCCESS,
        NOT_SUPPORT,
        FAILED
    };

    struct CMDExecuteRst
    {
        CMDExecuteRstType   type;
        std::string&        detail;
    };

    /**
     * @brief 任务实体,这是被Controller控制的对象
     */
    class MissionBlock : public BasePlugin
    {
    public:
        MissionBlock(const std::shared_ptr<RobotSystem>&);

        virtual ~MissionBlock();

        virtual int main_task() = 0;

        void exectue(MissionCMD, CMDExecuteRst&);

    protected:
        bool revent( ::rsdk::event::REventParam ) override;

    private:
        class Impl;
        Impl* _impl;
    };
}


#endif