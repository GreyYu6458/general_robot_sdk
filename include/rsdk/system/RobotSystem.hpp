#ifndef _ROBOT_SYSTEM_HPP_
#define _ROBOT_SYSTEM_HPP_
#include <string>
#include <memory>
#include "RobotSystemConfig.hpp"
#include "SystemInfoPublisher.hpp"
#include "rsdk/description/RobotDescription.hpp"

namespace rsdk
{
    class PluginInterface;

    // 飞机访问者接口类,具体的飞机实现时需要继承该接口
    // 注意:飞机尽量具体到某一个机型
    class RobotSystem : public SystemInfoPublisher
    {
    protected:
        // 子类实现时调用的注册函数, 
        // 如果plguin_hash不符合任何预定义的插件类型，则返回false
        template<class T, class G>
        bool registPlugin(const std::shared_ptr<G>& impl)
        {
            static_assert(std::is_base_of<rsdk::PluginInterface, T>::value, "error!");
            static_assert(std::is_base_of<T, G>::value, "error!");
            return _regist_plugin( typeid(T).hash_code(), impl );
        }

    private:
        // 注册插件
        bool _regist_plugin(size_t plugin_hash, const std::shared_ptr<PluginInterface>& impl);

        // 尝试在系统中获取某种插件的具体实现
        // 如果没有则返回 nullptr
        std::shared_ptr<PluginInterface> _getPlugin(size_t plugin_hash);

        class SystemImpl;
        SystemImpl* _impl;

    public:
        virtual bool link(const SystemConfig& config) = 0;

        // 是否连接
        virtual bool isLink() = 0;

        // 设备的生产商信息
        virtual const std::string &manufacturer() = 0;

        // 设备序列号，或者其他唯一绑定飞机的字符序列
        virtual const std::string &uniqueCode() = 0;

        // 机器人的名字
        virtual const std::string &robotName() = 0;

        // 更新SystemTime
        void updateSystemTime(uint32_t);

        // 如果无效返回0, 如果生效返回unix time
        uint32_t systemTime();
        
        virtual ~RobotSystem();

        RobotSystem();

        template<class T> 
        std::shared_ptr<T> getPlugin()
        {
            auto ptr = _getPlugin( typeid(T).hash_code() );
            return ptr ? std::dynamic_pointer_cast<T>(ptr) : nullptr;
        }
    };
}

#endif