#ifndef _DJI_MISSION_CONTEXT_HPP_
#define _DJI_MISSION_CONTEXT_HPP_
#include "p_rsdk/interfaces/mission/flight/waypoint/WPMContext.hpp"

namespace rmfw = rsdk::mission::flight::waypoint;

class DJIWPMission;

class DJIMissionContext : public rmfw::WPMContext
{
    friend class DJIMissionExecutor;
public:
    DJIMissionContext(const std::shared_ptr<DJIWPMission>&);

    virtual ~DJIMissionContext();

    /**
     * @brief 
     * 
     * @return const std::shared_ptr<DJIWPMission>& 
     */
    const std::shared_ptr<DJIWPMission>& djiWPMission();

    /**
     * @brief   是否已经完成了制定的所有航线次数(DJI 航线特别功能)
     *          可以通过该方式判断是否是由遥控器引起的任务结束
     * @return true 
     * @return false 
     */
    bool hasFinishedAllCount();

    /**
     * @brief 设置完成的次数 DJI特有?
     */
    void setFinishedCount(size_t count);

    /**
     * @brief 设置需要完成的次数，见大疆OSDK参数
     * 
     */
    void setAllRepeatTimes(size_t count);

private:
    class Impl;
    Impl* _impl;
};


#endif
