#include <game_objects/enemy_breeder.hpp>
#include <game_objects/enemy.hpp>
#include <game_objects/world_objects.hpp>
#include <common/level_functions.hpp>
#include <core/transform/transform.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/model/model.hpp>
#include <core/resources/texture.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/common/directory.hpp>
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
breeder_setup(Game_object::Enemy &enemy_to_setup)
{
  if(!model)
  {
    const char * unit_cube_path = Core::Directory::resource_path("assets/models/unit_cube.obj");
    model = Core::Model(unit_cube_path);
  }

  if(!texture)
  {
    const char * orange_texture_path = Core::Directory::resource_path("assets/textures/dev_grid_red_512.png");
    texture  = Core::Texture(orange_texture_path);
  }

  auto ref = enemy_to_setup.get_entity();
  
  ref.set_name("Enemy-Breeder");
  
  Game_object::Enemy_utils::update_position(ref,
                                            enemy_to_setup.m_point_on_circle,
                                            enemy_to_setup.m_depth);
}


void
breeder_update(Game_object::Enemy &enemy,
               const float dt,
              Game_object::World_objects &objs)
{
  Core::Transform trans = enemy.get_entity().get_transform();
  
  // Point on circle
  {
    math::vec2 new_point = Level_funcs::get_point_on_cirlce(enemy.m_point_on_circle);
    
    const math::vec3 position = trans.get_position();
    
    math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                         math::vec2_get_y(new_point),
                                         math::vec3_get_z(position));
    trans.set_position(new_pos);
  }
  
  // Should breed
  {
    if(enemy.m_direction == 0 && enemy.m_lifetime > 2.f)
    {
      enemy.m_lifetime = 0;
      
      ++enemy.m_user_data;
    
      const float new_point = enemy.m_point_on_circle + ((static_cast<float>(rand() % 100) / 250.f) - 0.2f);
      const float new_depth = math::min(enemy.m_depth + ((static_cast<float>(rand() % 100) / 50.f) - 0.75f), Level_funcs::get_top_of_level());
      
      Game_object::Enemy *egg1 = new Game_object::Enemy(enemy.get_world(), Game_object::Enemy::Type::egg);
      egg1->m_point_on_circle = enemy.m_point_on_circle + (0.1f * enemy.m_user_data);
      egg1->m_depth = enemy.m_depth;

      Game_object::Enemy *egg2 = new Game_object::Enemy(enemy.get_world(), Game_object::Enemy::Type::egg);
      egg2->m_point_on_circle = enemy.m_point_on_circle - (0.1f * enemy.m_user_data);
      egg2->m_depth = enemy.m_depth;
    
      objs.push_object(egg1);
      objs.push_object(egg2);
    }
  }
  
  // Depth
  if(enemy.m_direction != 0);
  {
    enemy.m_depth += (20.f * dt * static_cast<float>(enemy.m_direction));
    
    if(enemy.m_depth > Level_funcs::get_top_of_level())
    {
      enemy.m_direction = 0;
//        enemy.lifetime = 0;
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