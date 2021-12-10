#pragma once
#include <memory>

namespace rsdk
{
    struct DelegateMemory{};

    template<class T>
    class DelegateCreateHelp : public DelegateMemory
    {
    public:
        template<typename... Args>
        std::shared_ptr<T> create(Args&&... args)
        {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }
    };

    /**
     * @brief 用于依赖于状态上下文的接口继承
     * 
     */
    class StateDelegation
    {
    public:
        void setDelegateMemory(const std::shared_ptr<DelegateMemory> &memory)
        {
            _memory = memory;
        }

        std::shared_ptr<DelegateMemory> delegateMemory()
        {
            return _memory;
        }

    private:
        std::shared_ptr<DelegateMemory> _memory{nullptr};
    };
}