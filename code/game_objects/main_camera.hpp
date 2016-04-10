#ifndef MAIN_CAMERA_INCLUDED_9A8E6C83_A716_4315_AE91_EE7455B30F77
#define MAIN_CAMERA_INCLUDED_9A8E6C83_A716_4315_AE91_EE7455B30F77


#include <core/entity/entity.hpp>
#include <core/world/world_fwd.hpp>
#include <core/context/context_fwd.hpp>
#include <core/camera/camera.hpp>
#include <math/vec/vec_types.hpp>
#include <game_objects/game_objects_fwd.hpp>


struct Game_camera
{
  Core::Entity entity;
  Core::Camera camera;
  math::vec3 target_point;
};


namespace Camera_utils {


void
init_main_camera(Core::Context &ctx,
                 Core::World &world,
                 Game_camera &cam);


void
move_main_camera(Game_camera &cam,
                 const float dt,
                 const Player players[],
                 const uint32_t number_of_players);


} // ns


#endif // inc guard