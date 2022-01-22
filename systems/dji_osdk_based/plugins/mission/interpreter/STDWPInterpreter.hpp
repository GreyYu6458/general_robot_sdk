#pragma once
#include "plugins/mission/DJIWPMission.hpp"
#include "InterpretException.hpp"
#include <dji_waypoint_v2.hpp>
#include <dji_waypoint_v2_action.hpp>
#include <dji_mission_type.hpp>

namespace rmw = rsdk::mission::waypoint;

class DJIWPMInstance;
/**
 * @brief standard waypoint interpreter
 */
class STDWPInterpreter
{
public:
    /**
     * @brief Construct a new STDWPInterpreter object
     * 
     * @param instance 
     */
    explicit STDWPInterpreter(DJIWPMInstance* instance);

    /**
     * @brief Destroy the STDWPInterpreter object
     * 
     */
    ~STDWPInterpreter();

    /**
     * @brief 解析入口，将标准航点转化成大疆的航点 
     *        rsdk::mission::waypoint::WaypointItems 2 DJIWPMission
     * 
     * @return InterpretException 
     */
    InterpretException interpret(const rmw::WaypointItems&, DJIWPMission&);

    /**
     * @brief 处理不同类型命令的模板函数
     * 
     * @tparam mw::MavMissionItems 
     * @return InterpretException 
     */
    template<rmw::MavMissionItems> InterpretException 
    _convert_item(const rmw::WaypointItems&, DJIWPMission&);

    /**
     * @brief 构建一个到达触发器
     * 
     * @param waypoint_index 
     * @return DJIWaypointV2Trigger 
     */
    static DJI::OSDK::DJIWaypointV2Trigger    createReachedTrigger(uint16_t waypoint_index);
    
    /**
     * @brief 构建关联触发器
     * 
     */
    static DJI::OSDK::DJIWaypointV2Trigger    createAssociateTrigger(float wait_time /* unit:s */, uint16_t associate_action_index);

    /**
     * @brief 构建一个飞机控制器
     * 
     * @param isStartFlying 
     * @return DJIWaypointV2Actuator 
     */
    static DJI::OSDK::DJIWaypointV2Actuator   createAircraftControlActuator(bool isStartFlying);

    /**
     * @brief 构建一个相机控制器
     * 
     * @return DJIWaypointV2Actuator 
     */
    static DJI::OSDK::DJIWaypointV2Actuator   createPhotoActuator();

protected:

    struct WaitTimeInfo
    {
        // 停止的点
        uint32_t    pause_item_index;
        
        // 等待时间
        float       wait_time;
    };

    /**
     * @brief 解析上下文
     */
    struct Context 
    {
        // 当前解析到的航点
        size_t                          current_index;

        // 是否需要等待
        bool                            should_wait_time;
        
        // 等待的信息
        WaitTimeInfo                    wait_time_info;

        // 原始航点中,上一个航点(cmd type == 16)的序号
        const rmw::WPMItem*             last_wp;

        // 解析中，上一个大疆的航点
        const DJI::OSDK::WaypointV2*    last_dji_wp{nullptr};

        void reset()
        {
            current_index = 0;
            should_wait_time = false;
            wait_time_info.pause_item_index = 0;
            wait_time_info.wait_time = 0;
        }
    };

    DJIWPMInstance* instance();

    Context& context();

private:
    class Impl;
    Impl* _impl;
};

