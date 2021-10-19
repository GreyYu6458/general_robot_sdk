#ifndef _BASE_EXECUTOR_HPP_
#define _BASE_EXECUTOR_HPP_
#include <memory>
#include <functional>

namespace rsdk::mission
{
    class MissionContext;

    class BaseExecutor
    {
        friend class MissionContext;
    public:
        BaseExecutor();

        ~BaseExecutor();

    protected:
        void setOnBgWorkFinish(
            const std::function<void (std::unique_ptr<MissionContext>)>&
        );

        void onBgWorkFinish();

        void addToBgContextSet(std::unique_ptr<MissionContext>&);

    private:
        class Impl;
        Impl* _impl;
    };
} // namespace rsdk::mission

#endif
