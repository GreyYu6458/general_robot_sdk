#ifndef _SAVED_PHOTO_EVENT_HPP_
#define _SAVED_PHOTO_EVENT_HPP_
#include "ItemTriggerredEvent.hpp"
#include "WPMEventEnum.hpp"
#include <string>

namespace rsdk::mission::waypoint
{
    class SavedPhotoEvent : 
        public ItemTriggerredEvent<valueOfEvent<WPMissionEvent::SAVED_PHOTO>()>
    {
    public:
        SavedPhotoEvent(uint32_t wp_index, uint32_t item_index, const std::string& path):
            ItemTriggerredEvent(wp_index, item_index), _save_path(path){}

        std::string save_path()
        {
            return _save_path;
        }

    private:
        std::string _save_path;
    };

    template<> struct typeOfEvent<WPMissionEvent::SAVED_PHOTO>
    {typedef SavedPhotoEvent type;};
}

#endif
