#include "p_rsdk/system/SystemEventLoop.hpp"
#include <queue>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>

namespace rsdk
{
    class EventLoop::Impl
    {
    public:
        ~Impl()
        {
            {
                std::unique_lock<std::mutex> ulck(_queue_wait_mutex);
                _thread_quit = true;
                _cv.notify_all();
            }
            if( _queue_thread.joinable() )
                _queue_thread.join();
        }

        void startLoop()
        {
            std::thread( &EventLoop::Impl::handle, this ).swap(this->_queue_thread);
        }

        void handle()
        {
            while(!_thread_quit)
            {
                std::unique_lock<std::mutex> ulck(_queue_wait_mutex);

                while(_event_queue.empty() && !_thread_quit)
                {
                    _cv.wait( ulck );
                }

                if(_thread_quit)
                    break;

                while(!_event_queue.empty())
                {
                    auto& event_wrapper = _event_queue.front();

                    ulck.unlock();
                    if(_on_event_cb) _on_event_cb(event_wrapper);
                    ulck.lock();

                    _event_queue.pop();
                }
            }
        }

        inline void pushEvent(REventWrapper& event_wrapper)
        {
            // use std::try_to_lock because Push action may invoked in the same thread
            _event_queue.emplace(std::move(event_wrapper));
            _cv.notify_all();
        }

        std::function<void (REventWrapper&)>    _on_event_cb;

        std::queue<REventWrapper>               _event_queue;

        std::thread                             _queue_thread;

        std::atomic<bool>                       _thread_quit;

        std::condition_variable                 _cv;

        std::mutex                              _queue_wait_mutex;
    };

    void EventLoop::startLoop()
    {
        _impl->startLoop();
    }

    EventLoop::EventLoop()
    : _impl(new Impl())
    {}

    EventLoop::~EventLoop()
    {
        delete _impl;
    }

    void EventLoop::pushEvent(REventWrapper event_wrapper)
    {
        std::unique_lock<std::mutex> ulck(_impl->_queue_wait_mutex, std::try_to_lock);
        _impl->pushEvent(event_wrapper);
    }

    void EventLoop::setOnEvent(const std::function<void (REventWrapper&)>& f)
    {
        _impl->_on_event_cb = f;
    }
} // namespace rsdk

