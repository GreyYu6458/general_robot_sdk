#pragma once


#include <condition_variable>
#include <mutex>
#include <queue>

template<typename T> class SafeQueue
{
public:
    void push(const T& item)
    {
        {
            std::scoped_lock lock(_mutex);
            _queue.push(item);
        }
        _cv.notify_one();
    }

    void push(T&& item)
    {
        {
            std::scoped_lock lock(_mutex);
            _queue.push(std::move(item));
        }
        _cv.notify_one();
    }

    bool pop(T& item)
    {
        std::unique_lock lock(_mutex);
        _cv.wait(lock, [&]() { return !_queue.empty() || _stop; });
        if (queue_.empty())
          return false;
        item = std::move(queue_.front());
        _queue.pop();
        return true;
    }

    std::size_t size() const {
        std::scoped_lock lock(_mutex);
        return _queue.size();
    }

    bool empty() const {
        std::scoped_lock lock(_mutex);
        return _queue.empty();
    }

    void stop() {
        {
          std::scoped_lock lock(_mutex);
          _stop = true;
        }
        _cv.notify_all();
    }

private:
    std::condition_variable _cv;
    mutable                 _mutex;
    std::queue<T>           _queue;
    bool                    _stop;
};