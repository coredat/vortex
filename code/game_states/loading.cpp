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
#include <utilities/directory.hpp>
#include <math/quat/quat.hpp>
#include <utilities/logging.hpp>


namespace
{
  Core::Entity      loading_entity;
  Core::Material    logo_material;
  
  const float       max_timer = 2.f;  // Min time loading screen stays up
  float             curr_timer = 0.f;
}


void
loading_init(Core::Context &context, Core::World &world)
{
  curr_timer = 0;
  const std::string asset_path = util::get_resource_path() + "assets/";
  
  // Create an entity for the logo.
  if(!loading_entity)
  {
    loading_entity = Core::Entity(world);
    loading_entity.set_name("Loading Screen Logo");
    loading_entity.set_tags(Object_tags::gui_cam);
    
    // Rotate towards the screen.
    {
      math::quat rot = math::quat_init_with_axis_angle(1, 0, 0, math::quart_tau());
      
      Core::Transform trans;
      trans.set_rotation(rot);
      loading_entity.set_transform(trans);
    }
  }
  
  // Create a material for the logo.
  if(!logo_material)
  {
    logo_material = Core::Material("start-logo");
    
    const std::string orange_texture_path = util::get_resource_path() + "assets/textures/dev_grid_orange_512.png";
    Core::Texture texture(Core::Texture(orange_texture_path.c_str()));
    
    const std::string shader_path = util::get_resource_path() + "assets/shaders/basic_fullbright.ogl";
    Core::Shader shader(shader_path.c_str());
    
    logo_material.set_shader(shader);
    logo_material.set_map_01(texture);
  }
  
  // Apply a renderer to the logo.
  {
    const std::string model_01 = asset_path + "models/unit_plane.obj";
    Core::Model model = Core::Model(model_01.c_str());
    
    Core::Material_renderer mat_renderer;
    mat_renderer.set_material(logo_material);
    mat_renderer.set_model(model);
    
    loading_entity.set_renderer(mat_renderer);
  }
}


Game_state
loading_update(Core::Context &context,
               Core::World &world,
               Core::Camera &cam)
{
  curr_timer += world.get_delta_time();
  
  loading_entity.set_transform(Screen_cast::intersect_screen_plane(cam));
  
  if(curr_timer > max_timer)
  {
    loading_entity.destroy();
    return Game_state::title_screen;
  }
  
  return Game_state::loading;
}