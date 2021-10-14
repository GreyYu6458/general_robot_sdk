#ifndef _REVENT_HPP_
#define _REVENT_HPP_

namespace rsdk::event
{
    enum REventType
    {
        MissionEvent,
        SensorEvent,
    };

    class REvent
    {
    public:
        virtual ~REvent(){}

        void accept();

        void ignore();

        bool isAccepted() const;

        void setAccepted();

        REventType type() const;

    private:
        class REventImpl;
        REventImpl* _impl;
    };
}

#endif
