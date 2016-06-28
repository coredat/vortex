#include <game_objects/enemy_climber.hpp>
#include <game_objects/enemy.hpp>
#include <game_objects/world_objects.hpp>
#include <common/level_functions.hpp>
#include <core/transform/transform.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/model/model.hpp>
#include <core/resources/texture.hpp>
#include <utilities/directory.hpp>
#include <math/vec/vec2.hpp>
#include <math/vec/vec3.hpp>
#include <string>


namespace
{
  Core::Model         model;
  Core::Texture       texture;
  constexpr float     climber_speed      = 40.f;
}


namespace Enemy_logic {


void
climber_setup(Game_object::Enemy &enemy_to_setup)
{
  if(!model)
  {
    const std::string unit_cube_path = util::get_resource_path() + "assets/models/unit_cube.obj";
    model = Core::Model(unit_cube_path.c_str());
  }

  if(!texture)
  {
    const std::string orange_texture_path = util::get_resource_path() + "assets/textures/dev_grid_orange_512.png";
    texture  = Core::Texture(orange_texture_path.c_str());
  }

  auto ref = enemy_to_setup.get_entity();
  
  ref.set_name("Enemy-Climber");

}


void
climber_update(Game_object::Enemy &enemy,
               const float dt,
               Game_object::World_objects &objs)
{
  Core::Transform trans = enemy.get_entity().get_transform();
  
  // Point on circle
  {
    enemy.m_point_on_circle += dt;
  
    const math::vec3 position = trans.get_position();
    const math::vec2 new_point = Level_funcs::get_point_on_cirlce(enemy.m_point_on_circle);
    const math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                               math::vec2_get_y(new_point),
                                               math::vec3_get_z(position));
    trans.set_position(new_pos);
  }
  
  // Depth
  {
    enemy.m_depth += (climber_speed * dt * static_cast<float>(enemy.m_direction));
    
    if(!math::is_between(enemy.m_depth, Level_funcs::get_bottom_of_level(), Level_funcs::get_top_of_level()))
    {
      enemy.m_depth = math::clamp(enemy.m_depth, Level_funcs::get_bottom_of_level(), Level_funcs::get_top_of_level());
      enemy.m_direction *= -1;
    }
    
    const math::vec3 pos = trans.get_position();
    const math::vec3 new_pos = math::vec3_init(math::vec3_get_x(pos),
                                               math::vec3_get_y(pos),
                                               enemy.m_depth);
    
    trans.set_position(new_pos);
  }
  
  enemy.get_entity().set_transform(trans);
}


} // ns