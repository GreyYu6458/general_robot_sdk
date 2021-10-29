#pragma once


namespace rsdk::mission
{
    /**
     * @brief   mission instance 状态 转移图如下
     *          其中STARTING和EXECUTING的状态和MAIN TASK重叠
     *        
     *          WAITTING                --(instance start)                                >> STARTING
     *          STARTING                --(success)                                       >> EXECUTING
     *          STARTING                --(failed)                                        >> FAILED(END STATE)
     *          EXECUTING               --(failed)                                        >> FAILED(END STATE)
     *          EXECUTING               --(success)                                       >> FINISHED(END)
     *          
     *          PS:
     *          真实的状态只有 WATTING STARTTING EXECUTING FAILED FINISHED
     *          如下几个状态是FAILED 和 FINISHED 和 是否有子任务在运行 的组合
     *          叫做 准终态
     *          FAILED_WITH_SUBTASK
     *          FINISHED_WITH_SUBTASK
     */
    enum class InstanceState : uint16_t
    {
        // 等待启动
        WAITTING                = 1 << 0,
        // 启动阶段
        STARTING                = 1 << 1,
        // 执行中
        EXECUTING               = 1 << 2,
        // 准终态 ( expectant end state ) 代表main task失败，有子任务运行
        FAILED_WITH_SUBTASK     = 1 << 3, 
        // 终态   (end state) 代表main task失败，而且没有子任务
        FAILED                  = 1 << 4,
        // 准终态 代表main task完成, 但还有子任务
        FINISHED_WITH_SUBTASK   = 1 << 5,
        // 终态 代表main task完成，而且没有子任务
        FINISHED                = 1 << 6, 
        // 状态紊乱
        CHAOS                   = 1 << 7
    };

    // for faster compare
    static constexpr uint16_t end_mask = 
        static_cast<uint16_t>(InstanceState::FAILED) |
        static_cast<uint16_t>(InstanceState::FINISHED); 

    static constexpr uint16_t expectant_end_mask = 
        static_cast<uint16_t>(InstanceState::FAILED_WITH_SUBTASK) |
        static_cast<uint16_t>(InstanceState::FINISHED_WITH_SUBTASK);

    inline bool is_end_state(InstanceState state)
    {
        return static_cast<uint16_t>(state) & end_mask;
    }

    inline bool id_expectant_end(InstanceState state)
    {
        return static_cast<uint16_t>(state) & expectant_end_mask;
    }
} // namespace rsdk::mission
