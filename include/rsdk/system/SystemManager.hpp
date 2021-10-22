#ifndef _SYSTEM_MANAGER_HPP_
#define _SYSTEM_MANAGER_HPP_
#include "RobotSystemConfig.hpp"
#include "rsdk/robject/RObject.hpp"
#include <functional>
#include <memory>

namespace rsdk
{
    class RobotSystem;

    class SystemManager : public RObject
    {
        friend class RobotSystem;
    public:
        ~SystemManager();

        /**
         * @brief delete copy or move to support sington
         */

        SystemManager(const SystemManager&) = delete;

        SystemManager(SystemManager&&) = delete;

        SystemManager& operator=(const SystemManager&) = delete;

        SystemManager& operator=(SystemManager&&) = delete;

        /**
         * @brief 
         * 
         * @return SystemManager& 
         */
        static SystemManager& instance();

        /**
         * @brief 
         * 
         * @return std::shared_ptr<RobotSystem> 
         */
        std::shared_ptr<RobotSystem> findByUniqueCode(const std::string&);

    private:

        /**
         * @brief 
         * 
         */
        bool manageSystem(const std::shared_ptr<RobotSystem>&);

        /**
         * @brief Construct a new System Manager object
         * 
         */
        SystemManager();


        class Impl;
        Impl* _impl;
    };
}


#endif

// 用于发现和构造机器人系统