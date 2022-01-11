#ifndef _CAMERA_CONTROLLER_HPP_
#define _CAMERA_CONTROLLER_HPP_
#include "plugins/DJIPluginBase.hpp"
#include "p_rsdk/plugins/camera/VideoStreamPlugin.hpp"

/**
 * @brief 
 * 
 */
class DJIVideoStream : public rsdk::camera::VideoStreamPlugin,
                       public DJIPluginBase
{
public:
   explicit DJIVideoStream(const std::shared_ptr<DJIVehicleSystem> &system);

   ~DJIVideoStream() override;

   bool start() override;

   bool isStarted() override;

   DJIVehicleModels supportModel() override;

   rsdk::camera::VideoInfo videoInfo() override;

   bool startStream() override;

   bool stopStream() override;

   void exec() override;

private:
   class Impl;
   Impl *_impl;
};

#endif
