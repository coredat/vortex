#include "main_camera.hpp"
#include "player.hpp"
#include <core/world/world.hpp>
#include <core/transform/transform.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/camera/camera_properties.hpp>
#include <core/transform/transform.hpp>
#include <core/color/color_predefined.hpp>
#include <math/vec/vec3.hpp>


namespace Camera_utils {


void
init_main_camera(Core::World &world, Game_camera &cam)
{
  Core::Entity camera_entity = world.create_entity();
  Core::Camera main_camera;

  camera_entity.set_name("Main Camera");
  Core::Transform curr_trans = camera_entity.get_transform();
  curr_trans.set_position(math::vec3_init(0.f, 0.f, 12.f));
  camera_entity.set_transform(curr_trans);
  
  main_camera.set_width(800);
  main_camera.set_height(480);

  main_camera.set_attached_entity(camera_entity);
  main_camera.set_type(Core::Camera_type::perspective);
  main_camera.set_clear_flags(Core::Camera_clear::color | Core::Camera_clear::depth);
  main_camera.set_clear_color(Core::Color_utils::pink());
  
  cam.entity = std::move(camera_entity);
  cam.camera = std::move(main_camera);
}


void
move_main_camera(Game_camera &cam,
                 const float dt,
                 const Player players[],
                 const uint32_t number_of_players)
{

  // We set the camera origin as the first point.
  math::vec3 accum_target = math::vec3_init(0,0,20.f);
  
  Core::Transform this_trans = cam.entity.get_transform();
  
  // Go through the players and accumulate the target point.
  
  for(uint32_t i = 0; i < number_of_players; ++i)
  {
    const math::vec3 player_pos = players[i].entity.get_transform().get_position();
    
    const math::vec3 direction = math::vec3_subtract(player_pos, accum_target);
    const math::vec3 scaled_dir = math::vec3_scale(direction, 0.5f);
    
    accum_target = math::vec3_add(accum_target, scaled_dir);
  }
  
  cam.target_point = accum_target;
  
  const math::vec3 this_pos = this_trans.get_position();

  const math::vec3 move_dir   = math::vec3_subtract(cam.target_point, this_pos);
  const math::vec3 scaled_dir = math::vec3_scale(move_dir, dt * 2);
  const math::vec3 new_pos    = math::vec3_add(this_pos, scaled_dir);

  this_trans.set_position(new_pos);
  
  cam.entity.set_transform(this_trans);
}

} // ns