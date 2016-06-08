#include <game_states/loading.hpp>
#include <core/context/context.hpp>
#include <core/world/world.hpp>
#include <core/resources/texture.hpp>
#include <core/model/model.hpp>
#include <core/entity/entity.hpp>
#include <core/transform/transform.hpp>
#include <utilities/directory.hpp>
#include <math/quat/quat.hpp>
#include <utilities/logging.hpp>


namespace
{
  Core::Texture     texture;
  Core::Model       model;
  Core::Entity      loading_entity;
  
  const float       max_timer = 0.f; // Min time loading screen stays up
  float             curr_timer = 0.f;
}


void
loading_init(Core::Context &context, Core::World &world)
{
  LOG_TODO("This needs some fixing.");

  curr_timer = 0;
  
  const std::string asset_path = util::get_resource_path() + "assets/";
  
  if(!texture)
  {
    const std::string tex_01 = asset_path + "textures/ship_04.png";
    texture = Core::Texture(tex_01.c_str());
  }
  
  if(!model)
  {
    const std::string model_01 = asset_path + "models/unit_plane.obj";
    model = Core::Model(model_01.c_str());
  }
  
  if(!loading_entity)
  {
    loading_entity = Core::Entity(world);
    loading_entity.set_name("Loading Screen Logo");
    loading_entity.set_model(model);
    loading_entity.set_material_id(texture.get_id());
    
    // Rotate towards the screen.
    {
      math::quat rot = math::quat_init_with_axis_angle(0, 0, 1, math::quart_tau());
      
      Core::Transform trans;
      trans.set_rotation(rot);
      
      loading_entity.set_transform(trans);
    }
  }
}


Game_state
loading_update(Core::Context &context, Core::World &world)
{
  curr_timer += world.get_delta_time();
  
  if(curr_timer > max_timer)
  {
    return Game_state::title_screen;
  }
  
  return Game_state::null;
}