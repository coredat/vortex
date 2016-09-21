#include <game_states/loading.hpp>
#include <factories/material.hpp>
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
  
  #ifndef NDEBUG
  constexpr float   max_timer = 0.f; // Min time loading screen stays up
  #else
  constexpr float   max_timer = 2.f;
  #endif
  float             curr_timer = 0.f;
}


void
loading_init(Core::Context &context, Core::World &world)
{
  curr_timer = 0;
  
  logo_material = Factory::Material::get_logo();
  const Core::Texture texture(logo_material.get_map_01());
  
  // Create an entity for the logo.
  if(!loading_entity)
  {
    loading_entity = Core::Entity(world);
    loading_entity.set_name("Loading Screen Logo");
    loading_entity.set_tags(Object_tags::gui_cam);
    
    const Core::Transform trans(math::vec3_init(0, 0, 0),
                                math::vec3_init(math::to_float(texture.get_width()),
                                                1,
                                                math::to_float(texture.get_height())),
                                math::quat_init_with_axis_angle(Core::Transform::get_world_left(), -math::quart_tau()));
    
    loading_entity.set_transform(trans);
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