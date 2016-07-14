#include <game_states/loading.hpp>
#include <common/game_state.hpp>
#include <game_objects/world_objects.hpp>
#include <common/object_tags.hpp>
#include <common/screen_cast.hpp>
#include <core/context/context.hpp>
#include <core/world/world.hpp>
#include <core/resources/texture.hpp>
#include <core/model/model.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/camera/camera.hpp>
#include <core/camera/camera_utils.hpp>
#include <core/resources/material.hpp>
#include <core/resources/shader.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/renderer.hpp>
#include <core/transform/transform.hpp>
#include <core/common/directory.hpp>
#include <math/quat/quat.hpp>
#include <utilities/logging.hpp>


namespace
{
  Core::Entity      loading_entity;
  Core::Material    logo_material;
  
  const float       max_timer = 2.f; // Min time loading screen stays up
  float             curr_timer = 0.f;
}


void
loading_init(Core::Context &context, Core::World &world)
{
  curr_timer = 0;
  
  const Core::Texture texture(Core::Directory::volatile_resource_path("assets/textures/repofa.png"));
  
  // Create an entity for the logo.
  if(!loading_entity)
  {
    loading_entity = Core::Entity(world);
    loading_entity.set_name("Loading Screen Logo");
    loading_entity.set_tags(Object_tags::gui_cam);
    
    const Core::Transform trans(math::vec3_init(0, 0, 0),
                                math::vec3_init(texture.get_width() / 2, 1, texture.get_height() / 2),
                                math::quat_init_with_axis_angle(Core::Transform::get_world_left(), -math::quart_tau()));
    
    loading_entity.set_transform(trans);
  }
  
  // Create a material for the logo.
  if(!logo_material)
  {
    logo_material = Core::Material("start-logo");
    
    const Core::Shader shader(Core::Directory::volatile_resource_path("assets/shaders/basic_fullbright.ogl"));
    
    logo_material.set_shader(shader);
    logo_material.set_map_01(texture);
  }
  
  // Apply a renderer to the logo.
  {
    const Core::Model model = Core::Model(Core::Directory::volatile_resource_path("assets/models/unit_plane.obj"));
    const Core::Material_renderer mat_renderer(logo_material, model);
    
    loading_entity.set_renderer(mat_renderer);
  }
}


Game_state
loading_update(Core::Context &context,
               Core::World &world,
               Core::Camera &cam)
{
  curr_timer += world.get_delta_time();

  if(curr_timer > max_timer)
  {
    loading_entity.destroy();
    return Game_state::title_screen;
  }
  
  return Game_state::loading;
}