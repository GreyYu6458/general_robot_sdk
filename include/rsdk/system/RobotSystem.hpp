#ifndef _ROBOT_SYSTEM_HPP_
#define _ROBOT_SYSTEM_HPP_
#include <string>
#include <memory>
#include "rsdk/system/PluginMap.hpp"
#include "rsdk/robject/RObject.hpp"
#include "RobotSystemConfig.hpp"
#include "SystemInfoPublisher.hpp"

namespace rsdk
{
    class BaseREvent;

    // 飞机访问者接口类,具体的飞机实现时需要继承该接口
    // 注意:飞机尽量具体到某一个机型
    // fixme : When using HUI_EXPORT, enable_shared_from_this should be avoided
    class HUI_EXPORT RobotSystem : public RObject, 
                        public SystemInfoPublisher, 
                        public PluginMap,
                        public std::enable_shared_from_this<RobotSystem> 
    {
    public:
        RobotSystem();

        virtual ~RobotSystem();

        bool link(const SystemConfig& config);

        /**
         * @brief 是否连接
         * 
         * @return true 
         * @return false 
         */
        bool isLink();

        /**
         * @brief 设备的生产商信息
         * 
         * @return const std::string& 
         */
        virtual const std::string &manufacturer() = 0;

        /**
         * @brief 设备序列号，或者其他唯一绑定飞机的字符序列
         * 
         * @return const std::string& 
         */
        virtual const std::string &uniqueCode() = 0;

        /**
         * @brief 机器人的名字
         * 
         * @return const std::string& 
         */
        virtual const std::string &robotName() = 0;

        /**
         * @brief 将事件放入事件循环中，
         * 
         * @return true 
         * @return false 
         */
        bool sendEvent(const std::shared_ptr<RObject>&, ::rsdk::event::REventParam);

        bool sendEvent(RObject*, ::rsdk::event::REventParam);

        /**
         * @brief 
         * 
         */
        void postEvent(const ::std::shared_ptr<RObject>&, ::rsdk::event::REventParam);

        void postEvent(RObject*, ::rsdk::event::REventParam);

        /**
         * @brief 
         * 
         */
        void updateSystemBias(int64_t);

        /**
         * @brief 
         * 
         * @return int64_t 
         */
        int64_t systemTimeBias();

        // 返回系统时间
        int64_t systemTime();

    protected:

        virtual bool tryLink(const SystemConfig& config) = 0;

        /**
         * 重写该函数可以获得所有事件的控制权
        */
        virtual bool notify(const std::shared_ptr<RObject>&, ::rsdk::event::REventParam);

        /**
         * 重写该函数可以获得所有事件的控制权
        */
        virtual bool notify(RObject*, ::rsdk::event::REventParam);
        
    private:

        void onEventLoopPop();

        class SystemImpl;

        SystemImpl* _impl;
    };
}

#endif