#ifndef MAIN_CAMERA_INCLUDED_9A8E6C83_A716_4315_AE91_EE7455B30F77
#define MAIN_CAMERA_INCLUDED_9A8E6C83_A716_4315_AE91_EE7455B30F77


#include <game_objects/game_object.hpp>
#include <game_objects/game_objects_fwd.hpp>
#include <core/entity/entity.hpp>
#include <core/world/world_fwd.hpp>
#include <core/context/context_fwd.hpp>
#include <core/camera/camera.hpp>
#include <math/vec/vec_types.hpp>


namespace Game_object {


class Main_camera : public Game_object
{
public:

  explicit          Main_camera(Core::World &world, Core::Context &ctx);
  
  void              on_start() override;
  bool              on_update(const float dt, World_objects &world_objs) override;

public:

  Core::Camera      m_camera; // Currently selection screen needs to know where to place gui.
  
private:

  math::vec3        m_target_point;
  
};


} // ns


#endif // inc guard