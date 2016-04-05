#include "main_camera.hpp"
#include <core/world/world.hpp>
#include <core/transform/transform.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/camera/camera_properties.hpp>
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
  
  cam.attached_entity = std::move(camera_entity);
  cam.camera = std::move(main_camera);
}


void
move_main_camera(Game_camera &cam,
                 const float dt,
                 const Player players[],
                 const uint32_t number_of_players)
{
}


} // ns