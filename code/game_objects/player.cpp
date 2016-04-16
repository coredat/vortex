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
#include <math/quat/quat.hpp>


namespace
{
  constexpr float gun_cooldown_timer = 0.1f;
  constexpr float move_speed_base = 5.f;
}


namespace Player_utils {


void
init_players(Core::World &world,
             Players_container &players_container,
             const uint32_t controller_id)
{
  for(uint32_t i = 0; i < players_container.size; ++i)
  {
    Core::Model   model("/Users/PhilCK/Developer/core/assets/models/unit_cube.obj");
    Core::Texture texture("/Users/PhilCK/Developer/core/assets/textures/dev_grid_green_512.png");
    
    auto &player = players_container.player[i];
    
    // This entity is already taken.
    if(player.entity)
    {
      // Controller id has already been alocated so
      // don't do it again.
      if(player.controller_id == controller_id)
      {
        return;
      }
      continue;
    }
    
    player.controller_id = controller_id;
    
    player.entity = Core::Entity(world);
    player.entity.set_name("Player");
    player.entity.set_tags(Object_tags::player);
    player.entity.set_model(model);
    player.entity.set_material_id(texture.get_id());
    
    // We set a temp transform
    // because this will be the player selection screen.
    
    Core::Transform trans(
      math::vec3_init(-4.f + (i * (8.f / 4.f)), 0, 0),
      math::vec3_one(),
      math::quat_init()
    );
    
    player.entity.set_transform(trans);
    
    break;
  }
}


void
move_players(Core::Context &ctx,
             Core::World &world,
             const float dt,
             Players_container &players_container,
             Bullets_container &bullets_container)
{
  for(uint32_t i = 0; i < players_container.size; ++i)
  {
    auto &player = players_container.player[i];
    
    if(!player.entity)
    {
      continue;
    }
    
    player.power_up_timer -= dt;
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
    if(controller.is_button_down(Core::Input::Button::button_0) && player.jump_speed == 0.f)
    {
      player.jump_speed = 0.5f;
      player.jump_time = 0.f;
    }
    
    // Jump movement
    if(player.jump_speed)
    {
      player.jump_time += dt;
      float offset = (player.jump_speed * player.jump_time) + (-player.jump_time * player.jump_time * player.jump_time * player.jump_time);

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
    {
      const float multipler = player.power_up_timer > 0 ? dt * 15.f : 0.f;
      const float timer = player.gun_cooldown;
      
      if(timer < (0.f + multipler) && (controller.get_trigger(0) || controller.get_trigger(1)))
      {
        Bullet_utils::create_bullet(world, player.point_on_circle, -1, bullets_container);
        player.gun_cooldown = gun_cooldown_timer;
      }
    }
  }
}


void
power_up(Core::World &world,
         const Core::Entity_id id,
         Players_container &players_container)
{
  for(uint32_t i = 0; i < players_container.size; ++i)
  {
    auto &player = players_container.player[i];
    
    if(player.entity.get_id() == id)
    {
      player.power_up_timer = 5.f;
    }
  }
}


void
hit_player(Core::World &world,
           const Core::Entity_id id,
           Players_container &players_container,
           Explosions_container &explosions_container)
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
                                      explosions_container);
    
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