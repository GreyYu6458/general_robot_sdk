#include "DJIInitializationTool.hpp"

extern "C"
{
    #include "osdkhal_linux.h" // DJI HAL layer, copy from sample
    #include "osdkosal_linux.h"
}

#include <dji_platform.hpp>
#include <stdexcept>

static E_OsdkStat OsdkUser_Console(const uint8_t *data, uint16_t dataLen)
{
    return OSDK_STAT_OK;
    printf("%s", data);

    return OSDK_STAT_OK;
}

void initDJIOsdkHal()
{
    static T_OsdkLoggerConsole printConsole = {
        .consoleLevel = OSDK_LOGGER_CONSOLE_LOG_LEVEL_INFO,
        .func = OsdkUser_Console,
    };

    static T_OsdkHalUartHandler halUartHandler = {
        .UartInit = OsdkLinux_UartInit,
        .UartWriteData = OsdkLinux_UartSendData,
        .UartReadData = OsdkLinux_UartReadData,
        .UartClose = OsdkLinux_UartClose,
    };

    static T_OsdkHalUSBBulkHandler halUSBBulkHandler = {
        .USBBulkInit = OsdkLinux_USBBulkInit,
        .USBBulkWriteData = OsdkLinux_USBBulkSendData,
        .USBBulkReadData = OsdkLinux_USBBulkReadData,
        .USBBulkClose = OsdkLinux_USBBulkClose,
    };

    static T_OsdkOsalHandler osalHandler = {
        .TaskCreate = OsdkLinux_TaskCreate,
        .TaskDestroy = OsdkLinux_TaskDestroy,
        .TaskSleepMs = OsdkLinux_TaskSleepMs,
        .MutexCreate = OsdkLinux_MutexCreate,
        .MutexDestroy = OsdkLinux_MutexDestroy,
        .MutexLock = OsdkLinux_MutexLock,
        .MutexUnlock = OsdkLinux_MutexUnlock,
        .SemaphoreCreate = OsdkLinux_SemaphoreCreate,
        .SemaphoreDestroy = OsdkLinux_SemaphoreDestroy,
        .SemaphoreWait = OsdkLinux_SemaphoreWait,
        .SemaphoreTimedWait = OsdkLinux_SemaphoreTimedWait,
        .SemaphorePost = OsdkLinux_SemaphorePost,
        .GetTimeMs = OsdkLinux_GetTimeMs,
#ifdef OS_DEBUG
        .GetTimeUs = OsdkLinux_GetTimeUs,
#endif
        .Malloc = OsdkLinux_Malloc,
        .Free = OsdkLinux_Free,
    };

    if (DJI_REG_LOGGER_CONSOLE(&printConsole) != true)
    {
        throw std::runtime_error("[DJI]:logger console register fail");
    }

    if (DJI_REG_UART_HANDLER(&halUartHandler) != true)
    {
        throw std::runtime_error("[DJI]:Uart handler register fail");
    }

    if (DJI_REG_USB_BULK_HANDLER(&halUSBBulkHandler) != true)
    {
        throw std::runtime_error("[DJI]:USB Bulk handler register fail");
    }

    if (DJI_REG_OSAL_HANDLER(&osalHandler) != true)
    {
        throw std::runtime_error("[DJI]:Osal handler register fail");
    }
}