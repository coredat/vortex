#include <game_objects/horizon.hpp>
#include <game_objects/horizon_particle.hpp>
#include <game_objects/world_objects.hpp>
#include <common/level_functions.hpp>
#include <core/world/world.hpp>
#include <core/resources/texture.hpp>
#include <core/resources/material.hpp>
#include <core/resources/shader.hpp>
#include <core/model/model.hpp>
#include <core/transform/transform.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <math/general/general.hpp>
#include <utilities/directory.hpp>


namespace
{
  Core::Material  horizon_material_top;
  Core::Material  horizon_material_bottom;
  Core::Model     horizon_model;
}


namespace Game_object {


Horizon::Horizon(Core::World &world)
: Game_object(world)
{
  Core::Entity_ref ref = get_entity();
  
  if(!horizon_model)
  {
    const std::string path = util::get_resource_path() + "assets/models/ring.obj";
    Core::Model model(path.c_str());
    
    horizon_model = model;
  }
  
  if(!horizon_material_top)
  {
    horizon_material_top = Core::Material("Horizon-top");
  
    const std::string shader_path = util::get_resource_path() + "assets/shaders/basic_fullbright.ogl";
    Core::Shader shader(shader_path.c_str());
  
    const std::string tex_path2 = util::get_resource_path() + "assets/textures/dev_grid_orange_512.png";
    Core::Texture texture_top(tex_path2.c_str());
    
    horizon_material_top.set_shader(shader);
    horizon_material_top.set_map_01(texture_top);
  }
  
  if(!horizon_material_bottom)
  {
    horizon_material_bottom = Core::Material("Horizon-bottom");
  
    const std::string shader_path = util::get_resource_path() + "assets/shaders/basic_fullbright.ogl";
    Core::Shader shader(shader_path.c_str());
  
    const std::string tex_path = util::get_resource_path() + "assets/textures/dev_grid_magenta_512.png";
    Core::Texture texture_bot(tex_path.c_str());
    
    horizon_material_bottom.set_shader(shader);
    horizon_material_bottom.set_map_01(texture_bot);
  }

  ref.set_name("Horizon Particle Factory");
}


void
Horizon::on_update(const float dt, World_objects &objs)
{
  m_curr_horz_timer += dt;
  
  if (m_curr_horz_timer > m_horizon_timer)
  {
    m_curr_horz_timer = 0;
    
    constexpr uint32_t number_to_spawn = 1;
    constexpr float horz_offset = 15;
    
    for(uint32_t i = 0; i < number_to_spawn; ++i)
    {
      Horizon_particle *particle = new Horizon_particle(get_world());
      Core::Entity_ref ref = particle->get_entity();
      
//      const float x_pos = static_cast<float>(rand() % 200) - 100.f;
      const float x_pos = 0;
      const float scale = 15.f;
      
      Core::Transform trans;
      trans.set_position(math::vec3_init(x_pos, x_pos, Level_funcs::get_far_death_zone() * 2.5f));
      trans.set_scale(math::vec3_init(scale));
      ref.set_transform(trans);

      ref.set_material(horizon_material_bottom);
      ref.set_model(horizon_model);
      
      objs.push_object(particle);
    }
  }
}


} // ns