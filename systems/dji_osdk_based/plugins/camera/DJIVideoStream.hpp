#ifndef _CAMERA_CONTROLLER_HPP_
#define _CAMERA_CONTROLLER_HPP_
#include "plugins/DJIPluginBase.hpp"
#include "p_rsdk/interfaces/camera/VideoStreamInterface.hpp"

/**
 * @brief 
 * 
 */
class DJIVideoStream : public rsdk::camera::VideoStreamInterface,
                       public DJIPluginBase
{
public:
   DJIVideoStream(const std::shared_ptr<DJIVehicleSystem> &system);

   ~DJIVideoStream();

   bool start() override;

   bool isStarted() override;

   DJIVehicleModels supportModel() override;

   void exec() override;

private:
   class Impl;
   Impl *_impl;
};

#endif
