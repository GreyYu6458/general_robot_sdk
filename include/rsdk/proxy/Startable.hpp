#ifndef _STARTABLE_HPP_
#define _STARTABLE_HPP_

namespace rsdk
{
    struct Startable
    {
        virtual bool start() = 0;

        virtual bool isStarted() = 0;
    };
}

#endif
