#include "player.hpp"
#include <core/world/world.hpp>
#include <core/context/context.hpp>
#include <core/entity/entity.hpp>
#include <core/input/controller.hpp>
#include <core/transform/transform.hpp>
#include <math/vec/vec3.hpp>
#include <core/model/model.hpp>
#include <core/material/texture.hpp>
#include <game_objects/bullet.hpp>
#include <game_objects/bullet.hpp>
#include <game_objects/explosion.hpp>
#include <common/level_functions.hpp>
#include <common/object_tags.hpp>
#include <utilities/logging.hpp>
#include <math/vec/vec2.hpp>


namespace
{
  constexpr float gun_cooldown_timer = 0.1f;
  constexpr float move_speed_base = 5.f;
}


namespace Player_utils {


void
init_players(Core::World &world,
             Players_container &players_container)
{
  for(uint32_t i = 0; i < players_container.size; ++i)
  {
    Core::Model   model("/Users/PhilCK/Developer/core/assets/models/unit_cube.obj");
    Core::Texture texture("/Users/PhilCK/Developer/core/assets/textures/dev_grid_green_512.png");
    
    auto &player = players_container.player[i];
    
    player.entity = Core::Entity(world);
    player.entity.set_name("Player");
    player.entity.add_tag(Object_tags::player);
    player.entity.set_model(model);
    player.entity.set_material_id(texture.get_id());
  }
}


void
move_players(Core::Context &ctx,
             Core::World &world,
             const float dt,
             Players_container &players_container,
             Bullet bullets[],
             const uint32_t number_of_bullets)
{
  for(uint32_t i = 0; i < players_container.size; ++i)
  {
    auto &player = players_container.player[i];
    
    if(!player.entity)
    {
      continue;
    }
    
    player.gun_cooldown -= dt;
    
    Core::Input::Controller controller = Core::Input::Controller(ctx, player.controller_id);
    
    // Lateral Movement
    {
      const float move_speed = (controller.get_axis(0).x * move_speed_base) * dt;
      player.point_on_circle += move_speed;
      
      const math::vec2 new_point = Level::get_point_on_cirlce(player.point_on_circle);

      Core::Transform trans = player.entity.get_transform();
      const math::vec3 position = trans.get_position();
      
      const math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                                 math::vec2_get_y(new_point),
                                                 math::vec3_get_z(position));
      trans.set_position(new_pos);
      
      player.entity.set_transform(trans);
    }
    
    // Jump
    if(controller.is_button_down(Core::Input::Button::button_2) && player.jump_speed == 0.f)
    {
      player.jump_speed = 0.5f;
      player.jump_time = 0.f;
    }
    
    // Jump movement
    if(player.jump_speed)
    {
      player.jump_time += dt;
      float offset = (player.jump_speed * player.jump_time) + (-0.9f * player.jump_time * player.jump_time);

      Core::Transform trans = player.entity.get_transform();
      const math::vec3 pos = trans.get_position();
      
      float new_depth = math::vec3_get_z(pos) + offset;
      
      if(new_depth < Level::get_top_of_level())
      {
        new_depth = Level::get_top_of_level();
        player.jump_speed = 0.f;
        player.jump_time = 0.f;
      }
      
      const math::vec3 new_pos = math::vec3_init(math::vec3_get_x(pos), math::vec3_get_y(pos), new_depth);
      trans.set_position(new_pos);
      
      player.entity.set_transform(trans);
    }
    
    // Fire
    if(player.gun_cooldown < 0.f && controller.is_button_down(Core::Input::Button::button_0))
    {
      Bullet_utils::create_bullet(world, player.point_on_circle, -1, bullets, number_of_bullets);
      player.gun_cooldown = gun_cooldown_timer;
    }
  }
}


void
hit_player(Core::World &world,
           const Core::Entity_id id,
           Players_container &players_container,
           Explosion explosions[],
           const uint32_t number_of_explosions)
{
  // Search for entity and hit it.
  for(uint32_t i = 0; i < players_container.size; ++i)
  {
    auto &player = players_container.player[i];
    
    if(player.entity.get_id() != id)
    {
      continue;
    }
    
    Explosion_utils::create_explosion(world,
                                      player.entity.get_transform().get_position(),
                                      explosions,
                                      number_of_explosions);
    
    player.entity.destroy();
  }
}


bool
all_dead(Players_container &players_container)
{
  constexpr bool are_all_dead = true;

  for(uint32_t i = 0; i < players_container.size; ++i)
  {
    auto &player = players_container.player[i];
  
    if(player.entity)
    {
      return false;
    }
  }
  
  return are_all_dead;
}


} // ns