#include <game_objects/enemy_shooter.hpp>
#include <game_objects/enemy.hpp>
#include <game_objects/world_objects.hpp>
#include <common/level_functions.hpp>
#include <game_objects/bullet.hpp>
#include <core/transform/transform.hpp>


namespace Enemy_logic {


void
shooter_setup(Game_object::Enemy &enemy_to_setup)
{
  auto ref = enemy_to_setup.get_entity();
  
  ref.set_name("Enemy-Shooter");
}


void
shooter_update(Game_object::Enemy &enemy,
               const float dt,
               Game_object::World_objects &objs)
{
  Core::Transform trans = enemy.get_entity().get_transform();
  
  // Point on circle
  {
    enemy.m_point_on_circle += dt;
  
    math::vec2 new_point = Level_funcs::get_point_on_cirlce(enemy.m_point_on_circle);
    
    const math::vec3 position = trans.get_position();
    
    math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                         math::vec2_get_y(new_point),
                                         math::vec3_get_z(position));
    trans.set_position(new_pos);
  }
  
  // Depth
  {
    enemy.m_depth += (0 * dt * static_cast<float>(enemy.m_direction));
    
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
  
  // Rand shoot
  if((rand() % 100) == 0)
  {
    auto bullet = new Game_object::Bullet(enemy.get_world(),
                                          math::vec2_init(math::vec3_get_z(enemy.get_entity().get_transform().get_position()), enemy.m_point_on_circle),
                                          math::vec2_init(0, +1),
                                          70.7f);
    
    objs.push_object(bullet);

  }
  
  enemy.get_entity().set_transform(trans);
}


} // ns