#pragma once

#include <string>

namespace rsdk::mission
{
    struct CMDExecRst
    {
        bool is_success;
        std::string detail;
    };

    struct BaseInstance
    {
        virtual void start(CMDExecRst&) = 0;
    }; 

    struct ControllableInstance : public BaseInstance
    {
        virtual void stop( CMDExecRst&  ) = 0;

        virtual void pause( CMDExecRst&  ) = 0;

        virtual void resume( CMDExecRst&  ) = 0;
    };  
} // namespace rsdk::mission
