#pragma once

#include <functional>
#include <thread>
#include <vector>

#include "rsdk/utils/container/SafeQueue.hpp"

using WorkItem = std::function<void ()>;

class SimplePool
{
public:
    explicit SimplePool(size_t thread_number = 
        std::thread::hardware_concurrency())
    {
        for(size_t i = 0; i < thread_number; i++)
        {
            _workers.emplace_back(
                [this]
                {
                    while(true)
                    {

                    }
                }
            );
        }
    }

    void item_mission()
    {
        while(true)
        {
            
        }
    }

private:
    SafeQueue<WorkItem>         _queue;
    std::vector<std::thread>    _workers;
};
