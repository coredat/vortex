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
  void              on_update(const float dt, World_objects &world_objs) override;
  
  void              set_target_height(const float height);

public:

  Core::Camera      m_world_camera;
  Core::Camera      m_level_camera;
  Core::Camera      m_gui_camera;
  Core::Camera      m_final_post_camera;
  
private:

  Core::Entity      m_gui_camera_entity; // this doens't move

  math::vec3        m_target_point;
  float             m_target_height = 20.f;
  
};


} // ns


#endif // inc guard