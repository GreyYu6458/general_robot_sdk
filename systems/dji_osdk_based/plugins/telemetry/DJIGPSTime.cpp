#include "DJIGPSTime.hpp"

void NMEACallback(Vehicle* vehiclePtr,
                  RecvContainer recvFrame,
                  UserData userData)
{
  int length = recvFrame.recvInfo.len-OpenProtocol::PackageMin-4;
  uint8_t rawBuf[length];
  memcpy(rawBuf, recvFrame.recvData.raw_ack_array, length);
  DSTATUS("%s\n", std::string((char*)rawBuf, length).c_str());
}

void UTCTimeCallback(Vehicle* vehiclePtr,
                     RecvContainer recvFrame,
                     UserData userData)
{
  int length = recvFrame.recvInfo.len-OpenProtocol::PackageMin-4;
  uint8_t rawBuf[length];
  memcpy(rawBuf, recvFrame.recvData.raw_ack_array, length);
  DSTATUS("The UTC time for the next PPS pulse (ard 500ms) is...");
  DSTATUS("%s\n", std::string((char*)rawBuf, length).c_str());
}

void FCTimeInUTCCallback(Vehicle* vehiclePtr,
                       RecvContainer recvFrame,
                       UserData userData)
{
  DSTATUS("Received Flight controller time in UTC reference...");
  DSTATUS("FC: %u, UTC time: %u, UTC date: %u\n",
          recvFrame.recvData.fcTimeInUTC.fc_timestamp_us,
          recvFrame.recvData.fcTimeInUTC.utc_hhmmss,
          recvFrame.recvData.fcTimeInUTC.utc_yymmdd);
}

void PPSSourceCallback(Vehicle* vehiclePtr,
                         RecvContainer recvFrame,
                         UserData userData)
{
  std::vector<std::string> stringVec = {"0", "INTERNAL_GPS", "EXTERNAL_GPS", "RTK"};
  DSTATUS("PPS pulse is coming from %s\n", stringVec[recvFrame.recvData.ppsSourceType].c_str());
}

DJIGPSTime::DJIGPSTime(const std::shared_ptr<DJIVehicleSystem>& system)
: dji::gps_time::BaseType(system)
{
    // Note that these CBs share the same thread with serial reading
  vehicle()->hardSync->subscribeNMEAMsgs(NMEACallback, nullptr);
  vehicle()->hardSync->subscribeUTCTime(UTCTimeCallback, nullptr);
  vehicle()->hardSync->subscribeFCTimeInUTCRef(FCTimeInUTCCallback, nullptr);
  vehicle()->hardSync->subscribePPSSource(PPSSourceCallback, nullptr);
}

void DJIGPSTime::convert(const DJIGPSTime::pkg_msg_type& date)
{
    _dji_system->updateSystemTime(std::get<0>(date));
}
