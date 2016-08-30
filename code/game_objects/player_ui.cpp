#include <game_objects/player_ui.hpp>
#include <common/screen_cast.hpp>
#include <common/object_tags.hpp>
#include <core/resources/material.hpp>
#include <core/resources/texture.hpp>
#include <core/model/model.hpp>
#include <core/resources/shader.hpp>
#include <core/common/directory.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/transform/transform.hpp>
#include <core/camera/camera.hpp>
#include <core/camera/camera_utils.hpp>
#include <core/common/ray.hpp>
#include <core/common/plane.hpp>
#include <core/context/context.hpp>
#include <core/common/plane_utils.hpp>
#include <core/input/axis.hpp>


namespace
{
  Core::Material numbers[10];
}



namespace Game_object {


Player_ui::Player_ui(Core::World &world,
                     Core::Context &ctx,
                     const Core::Camera &cam,
                     const uint32_t controller_id)
: Game_object(world)
{
  const Core::Shader num_shader(Core::Directory::volatile_resource_path("assets/shaders/basic_fullbright.ogl"));

  // Load up numbers for the first run
  if(!numbers[0])
  {
    char buffer[2048];
  
    for(uint32_t i = 0; i < 10; ++i)
    {
      memset(buffer, 0, sizeof(char) * 2048);
      sprintf(buffer, "%s/num_%d.png", Core::Directory::volatile_resource_path("assets/textures"), i);
      
      Core::Texture texture(buffer);
      assert(texture);
      
      // --
      
      
      // --
      
      char mat_name[128];
      memset(mat_name, 0, sizeof(char) * 128);
      sprintf(mat_name, "[player_ui]number_%d", i);
      
      Core::Material material(mat_name);
      assert(material);
      
      material.set_shader(num_shader);
      material.set_map_01(texture);
      
      numbers[i] = material;
    }
  }
  
  // Create counter entitiets
  // These actually render the numbers
  for(auto &counter : m_counters)
  {
    counter = Core::Entity(world);
    counter.set_name("[player_ui]counter");
    counter.set_tags(Object_tags::gui_cam);
  }

  auto ref = get_entity();

  ref.set_name("player_ui");
  ref.set_tags(Object_tags::gui_cam);
  
  Core::Material ui_mat = Core::Material("player_ui_mat");
  
  const char * orange_texture_path = Core::Directory::volatile_resource_path("assets/textures/dev_grid_orange_512.png");
  const Core::Texture texture(orange_texture_path);
  assert(texture);
  
  const char * shader_path = Core::Directory::volatile_resource_path("assets/shaders/vortex_dir_light.ogl");
  const Core::Shader shader(shader_path);
  assert(shader);
  
  ui_mat.set_shader(shader);
  ui_mat.set_map_01(texture);

  const char * unit_cube_path = Core::Directory::volatile_resource_path("assets/models/unit_cube.obj");
  const Core::Model model = Core::Model(unit_cube_path);
  assert(model);
  
  const Core::Material_renderer mat_renderer(numbers[0], model);
//  ref.set_renderer(mat_renderer);
  
  // All the corners of the screen.
  const uint32_t screen_coords_count = 4;
  
  const Core::Axis screen_positions[screen_coords_count] {
    Core::Axis{0,0},
    Core::Axis{math::to_float(ctx.get_width()),0},
    Core::Axis{0,math::to_float(ctx.get_height())},
    Core::Axis{math::to_float(ctx.get_width()),math::to_float(ctx.get_width())},
  };
  
  const uint32_t coord_index = controller_id - 1;
  assert(controller_id < screen_coords_count);
  
  const Core::Ray ray = Core::Camera_utils::get_ray_from_viewport(cam, screen_positions[coord_index]);
  const Core::Plane plane = Core::Camera_utils::get_near_plane(cam);
  
  float out_distance = 0;
  const bool intersects = Core::Plane_utils::ray_intersects_with_plane(plane, ray, out_distance);
  assert(intersects); // should alwasy intersect
  
  const Core::Transform cam_trans = cam.get_attached_entity().get_transform();
  const math::vec3 scale_fwd = math::vec3_scale(cam_trans.get_forward(), out_distance);
  const math::vec3 position  = math::vec3_add(ray.get_origin(), scale_fwd);
  
  const Core::Transform trans(
    position,
    math::vec3_init(128),
    math::quat_init()
  );
  
//  ref.set_transform(trans);

  m_counters[0].set_renderer(mat_renderer);
  m_counters[0].set_transform(trans);
}


void
Player_ui::on_start()
{
}


void
Player_ui::on_update(const float dt, World_objects &objs)
{
  
}


} // ns