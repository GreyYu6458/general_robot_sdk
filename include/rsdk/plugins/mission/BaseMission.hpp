#ifndef _MISSION_HPP_
#define _MISSION_HPP_
#include <string>

namespace rsdk::mission
{
    enum class MissionStatus
    {
        Executing,
        Background,
        Finished
    };

    struct ExecuteRst
    {
        bool is_success;
        std::string detail;
    };

    class BaseMission
    {
    public:
        virtual void start() = 0;

        /**
         * @ brief 通知任务管理器
        */
        virtual void notify() = 0;

        virtual ~BaseMission() = 0;
    };
}

#endif