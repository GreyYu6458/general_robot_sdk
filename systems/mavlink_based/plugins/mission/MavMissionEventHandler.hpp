#pragma once

class MavMissionInstance;

class MavMissionEventHandler
{
public:
    explicit MavMissionEventHandler(MavMissionInstance*);

    virtual ~MavMissionEventHandler();

private:
    class Impl;
    Impl* _impl;
};
