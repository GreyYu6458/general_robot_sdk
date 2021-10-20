#ifndef _PLUGIN_INTERFACE_HPP_
#define _PLUGIN_INTERFACE_HPP_
#include "rsdk/plugins/Startable.hpp"

namespace rsdk
{
    struct PluginInterface : public Startable{};
}

#endif