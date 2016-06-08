#include <game_objects/level.hpp>
#include <game_objects/horizon_particle.hpp>
#include <game_objects/world_objects.hpp>
#include <common/level_functions.hpp>
#include <core/world/world.hpp>
#include <core/resources/texture.hpp>
#include <core/model/model.hpp>
#include <core/transform/transform.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <math/general/general.hpp>
#include <utilities/directory.hpp>


namespace Game_object {


Level::Level(Core::World &world)
: Game_object(world)
{
  Core::Entity_ref ref = get_entity();

  ref.set_name("Level");
  
  const std::string level_path = util::get_resource_path() + "assets/models/unit_tube.obj";
  Core::Model model(level_path.c_str());

  const std::string grey_texture_path = util::get_resource_path() + "assets/textures/dev_grid_grey_512.png";
  Core::Texture texture(grey_texture_path.c_str());

  const float depth = Level_funcs::get_top_of_level() - Level_funcs::get_bottom_of_level();
  Core::Transform trans(math::vec3_init(0, 0, -depth * 0.5f),
                        math::vec3_init(Level_funcs::get_radius() * 2.1f, Level_funcs::get_radius() * 2.1f, math::abs(depth) * 0.9f),
                        math::quat_init());

  ref.set_model(model);
  ref.set_material_id(texture.get_id());
  ref.set_transform(trans);
}


void
Level::on_update(const float dt, World_objects &objs)
{
  m_curr_horz_timer += dt;
  
  if (m_curr_horz_timer > m_horizon_timer)
  {
    m_curr_horz_timer = 0;
    
    
    const std::string path = util::get_resource_path() + "assets/models/unit_plane.obj";
    Core::Model model(path.c_str());
    
    const std::string tex_path = util::get_resource_path() + "assets/textures/dev_grid_magenta_512.png";
    Core::Texture texture_bot(tex_path.c_str());
    
    const std::string tex_path2 = util::get_resource_path() + "assets/textures/dev_grid_orange_512.png";
    Core::Texture texture_top(tex_path2.c_str());
    
    constexpr uint32_t number_to_spawn = 10;
    constexpr float horz_offset = 15;
    
    for(uint32_t i = 0; i < number_to_spawn; ++i)
    {
      Horizon_particle *particle = new Horizon_particle(get_world());
      Core::Entity_ref ref = particle->get_entity();
      
      const float x_pos = static_cast<float>(rand() % 200) - 100.f;
      
      Core::Transform trans;
      trans.set_position(math::vec3_init(x_pos, -horz_offset, Level_funcs::get_far_death_zone() * 2.5f));
      trans.set_scale(math::vec3_init(10.f, 10.f, 10.f));
      ref.set_transform(trans);

      ref.set_model(model);
      
      ref.set_material_id(texture_bot.get_id());
      
      objs.push_object(particle);
    }
    
    for(uint32_t i = 0; i < number_to_spawn; ++i)
    {
      Horizon_particle *particle = new Horizon_particle(get_world());
      Core::Entity_ref ref = particle->get_entity();
      
      const float x_pos = static_cast<float>(rand() % 200) - 100.f;
      
      Core::Transform trans;
      trans.set_position(math::vec3_init(x_pos, +horz_offset, Level_funcs::get_far_death_zone() * 2.5f));
      trans.set_scale(math::vec3_init(10.f, 10.f, 10.f));
      
      const math::quat rot = math::quat_init_with_axis_angle(0, 0, 1, math::half_tau());
      trans.set_rotation(rot);
      
      ref.set_transform(trans);

      ref.set_model(model);
      
      ref.set_material_id(texture_top.get_id());
      
      objs.push_object(particle);
    }
  }
}


} // ns