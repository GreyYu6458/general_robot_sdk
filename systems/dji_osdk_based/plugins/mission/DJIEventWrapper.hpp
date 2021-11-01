#ifndef _DJI_EVENT_HANDLER_HPP_
#define _DJI_EVENT_HANDLER_HPP_

class DJIWPMInstance;
class DJIVehicleSystem;

class DJIEventWrapper
{
public:
    DJIEventWrapper(DJIWPMInstance* instance);

    ~DJIEventWrapper();

    void startListeningDJILowLayerEvent();

    DJIWPMInstance* instance();

private:
    class Impl;
    Impl* _impl;
};

#endif
