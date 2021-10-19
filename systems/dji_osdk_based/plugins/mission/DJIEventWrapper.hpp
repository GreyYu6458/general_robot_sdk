#ifndef _DJI_EVENT_HANDLER_HPP_
#define _DJI_EVENT_HANDLER_HPP_

class DJIWPExecutor;
class DJIVehicleSystem;

/**
 * @brief   这个类负责注册大疆的任务回调，并将大疆的事件封装成框架的MissionEvent。
 *          封装后的MissionEvent将作为 DJIWPExecutor::onEvent()的参数
 */
class DJIEventWrapper
{
public:
    DJIEventWrapper(DJIVehicleSystem* const, DJIWPExecutor* executor);

    ~DJIEventWrapper();

    void startListeningDJILowLayerEvent();

    DJIWPExecutor* const executor();

    DJIVehicleSystem* const system();

private:
    class Impl;
    Impl* _impl;
};


#endif
