option(
    DJI_SYSTEM_SIM_ENABLE
    "A special version for dji sim environment" 
    OFF
)

option(
    DJI_SYSTEM_USE_HARDWARE_TIME 
    "Use GPRNA for hardware sync system time (warning:this may decrease time accuracy，when host ntp server is enable)" 
    OFF
)