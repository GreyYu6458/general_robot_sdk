#include "rsdk/robject/RObject.hpp"
#include <mutex>
#include <vector>

namespace rsdk
{
    class RObject::Impl
    {
    public:
        RObject*                    _parent;
        RObject*                    _root;
        RObject*                    _watcher_bare;
        std::vector<RObject*>       _children;
        std::weak_ptr<RObject>      _watcher;
        std::mutex                  _set_watcher_mutex;
    };

    RObject::RObject()
    : _impl(new Impl())
    {
        _impl->_parent = nullptr;
        _impl->_root = this;
    }

    RObject::RObject(RObject* robject)
    : _impl(new Impl())
    {
        // set parent
        _impl->_parent = robject;
        _impl->_root   = robject->_impl->_root;
        robject->_impl->_children.push_back(this);
    }

    RObject::~RObject()
    {
        for(auto object : _impl->_children)
        {
            delete object;
        }
        delete _impl;
    }

    bool RObject::revent( ::rsdk::event::REventParam event)
    {
        return true;
    }

    void RObject::installEventFilter(RObject* watcher)
    {
        std::lock_guard<std::mutex> lck(_impl->_set_watcher_mutex);
        removeEventFilter();
        _impl->_watcher_bare = watcher;
    }

    bool RObject::notifyied(::rsdk::event::REventParam event)
    {
        if(_impl->_watcher_bare)
        {
            _impl->_watcher_bare->eventFilter(this, event);
        }
        else if(!_impl->_watcher.expired())
        {
            _impl->_watcher.lock()->eventFilter(this, event);
        }
        return revent(event);
    }

    void RObject::installEventFilter(const ::std::shared_ptr<RObject>& watcher)
    {
        std::lock_guard<std::mutex> lck(_impl->_set_watcher_mutex);
        removeEventFilter();
        _impl->_watcher = watcher;
    }

    void RObject::removeEventFilter()
    {
        _impl->_watcher.reset();
        _impl->_watcher_bare = nullptr;
    }

    bool RObject::eventFilter(RObject* r_obj, ::rsdk::event::REventParam)
    {
        return true;
    }

    RObject* RObject::rootObject()
    {
        return _impl->_root;
    }

    RObject* RObject::parent()
    {
        return _impl->_parent;
    }
}