#ifndef _DJI_TELEMETRY_HPP_
#define _DJI_TELEMETRY_HPP_
#include "plugins/DJIPluginBase.hpp"
#include "p_rsdk/plugins/telemetry/telemetryPlugins.hpp"

#include <dji_vehicle.hpp>
#include <dji_telemetry.hpp>
#include <dji_ack.hpp>
#include <mutex>
#include <tuple>
#include <functional>

namespace DJITelemetrySpace     = DJI::OSDK::Telemetry;

using DJITelemetryEnum          = DJITelemetrySpace::TopicName;
using DJIACK                    = DJI::OSDK::ACK;
using DJIContainer              = DJI::OSDK::RecvContainer;
using DJIUserData               = DJI::OSDK::UserData; // void*

template <int pkg_index, DJITelemetryEnum... topics>
class DJITelemetryPackage
{
public:
    using telem_type = std::tuple<typename DJITelemetrySpace::TypeMap<topics>::type...>;

    typedef void (*UnpackCallBack)(const telem_type&, void* user_data);

    void startPackage(DJIVehiclePtr vehicle, uint16_t freq)
    {
        dji_vehicle = vehicle;
        DJITelemetryEnum _topic_list[] = {topics...};

        auto pkg_status = dji_vehicle->subscribe->initPackageFromTopicList(
            pkg_index, sizeof...(topics), _topic_list, false, freq);

        if (!pkg_status)
            return;

        DJIACK::ErrorCode ec = dji_vehicle->subscribe->startPackage
        (   pkg_index, 10 );

        if (DJIACK::getError(ec) != DJIACK::SUCCESS)
        {
            DJIACK::getErrorCodeMessage(ec, __func__);
            dji_vehicle->subscribe->removePackage(pkg_index, 10);
        }
    }

    // 当解包时调用的回调函数
    void onMessageUnpack(UnpackCallBack _callback, void* user_data)
    {
        _unpack_callback = _callback;
        _user_data = user_data;

        dji_vehicle->subscribe->registerUserPackageUnpackCallback(
            pkg_index, callback, this
        );
    }

private:
    UnpackCallBack  _unpack_callback{nullptr};
    void*           _user_data{nullptr};

    static inline void updateValue(Vehicle *vehicle, telem_type &tuple)
    {
        set<0, topics...>(vehicle, tuple);
    }

    // 递归结束
    template <size_t I>
    static inline typename std::enable_if<I == sizeof...(topics)>::type
    set(Vehicle *vehicle, telem_type &tuple) {
        (void)vehicle;
        (void)tuple;
    }

    // 递归赋值,编译期时展开,没有入栈消耗....
    template <size_t I, Telemetry::TopicName current, Telemetry::TopicName... other>
    static inline typename std::enable_if < I<sizeof...(topics)>::type
    set(Vehicle *vehicle, telem_type &tuple)
    {
        std::get<I>(tuple) = vehicle->subscribe->getValue<current>();
        set<I + 1, other...>(vehicle, tuple);
    }

    using self_type = DJITelemetryPackage<pkg_index, topics...>;

    static void callback(DJIVehicle *vehicle, DJIContainer frame, DJIUserData userData)
    {
        (void)frame;
        self_type *self = static_cast<self_type *>(userData);
        updateValue(vehicle, self->_raw_msg);

        if (self->_unpack_callback)
        {
            self->_unpack_callback(self->_raw_msg, self->_user_data);
        }
    }

    DJIVehiclePtr dji_vehicle;
    telem_type _raw_msg;
};

template <class Child, class MsgType, int pkg_index, DJITelemetryEnum... topics>
class DJITelemetry : public DJIPluginBase
{
public:
    using self_type     = DJITelemetry<Child, MsgType, pkg_index, topics...>;
    using pkg_type      = DJITelemetryPackage<pkg_index, topics...>;
    using pkg_msg_type  = typename pkg_type::telem_type;

    DJITelemetry(const std::shared_ptr<DJIVehicleSystem>& system)
    : DJIPluginBase(system){}

    DJIVehicleModels supportModel() override
    {
        return DJIVehicleModels::MODEL_ALL;
    }

    void setFreqency(uint16_t freq)
    {
        this->_freq = freq;
    }

    void exec() override
    {
        _telem_package.startPackage(this->vehicle(), _freq);

        _telem_package.onMessageUnpack(
            &self_type::messageUnpackCallback, this
        );

        _is_started = true;
    }

    static void messageUnpackCallback(const pkg_msg_type& elem, void* user_data)
    {
        self_type* obj = (self_type*)user_data;

        obj->convert(elem);
    }

    void convert(const pkg_msg_type& data)
    {
        static_cast<Child&>(*this).convert(data);
    }
                
    bool            _is_started{false};
    uint16_t        _freq{1};
    MsgType         _sensor_msg{MsgType()};
    pkg_type        _telem_package;
};

#define GPS_TIME_PJG_INDEX          0
#define ATTITUDE_STATUS_PKG_INDEX   1
#define AVOID_PKG_INDEX             2
#define GNSS_PKG_INDEX              3
#define GNSS_UNCERTAIN_PKG_INDEX    4
#define FLIGHT_STATUS_PKG_INDEX     5

#endif
