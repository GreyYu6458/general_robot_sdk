#include "rsdk/robject/RObject.hpp"
#include <mutex>

namespace rsdk
{
    class RObject::Impl
    {
    public:
        ::std::mutex                _set_watcher_mutex;
        ::std::shared_ptr<RObject>  _watcher{nullptr};
        ::rsdk::event::REventCBType _cb;
    };

    RObject::RObject()
    : _impl(new Impl()){

    }

    RObject::~RObject()
    {
        delete _impl;
    }

    bool RObject::revent( ::rsdk::event::REventParam )
    {

    }

    void RObject::installEventFilter(const ::std::shared_ptr<RObject>& watcher)
    {
        std::lock_guard<std::mutex> lck(_impl->_set_watcher_mutex);
        _impl->_watcher = watcher;
    }

    bool RObject::eventFilter(const ::std::shared_ptr<RObject>& r_obj, ::rsdk::event::REventParam event)
    {
        std::lock_guard<std::mutex> lck(_impl->_set_watcher_mutex);

        r_obj->revent(event);
    }
}
