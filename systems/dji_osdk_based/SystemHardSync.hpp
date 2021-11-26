#pragma once 

class DJIVehicleSystem;

class SystemHardSync
{
public:
    explicit SystemHardSync(DJIVehicleSystem*);

    ~SystemHardSync();

    void startSync();

private:
    class Impl;
    Impl* _impl;
};

