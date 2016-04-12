#ifndef PLAYER_INCLUDED_27EF33BB_69F2_4B56_909A_1FA47B263E56
#define PLAYER_INCLUDED_27EF33BB_69F2_4B56_909A_1FA47B263E56


#include <core/entity/entity.hpp>
#include <core/world/world_fwd.hpp>
#include <core/context/context_fwd.hpp>
#include <game_objects/game_objects_fwd.hpp>
#include <stdint.h>


struct Players_container
{
  struct Player
  {
    Core::Entity  entity;
    uint32_t      controller_id = 0;
    float         point_on_circle = 0.f;
    float         gun_cooldown = 0.f;
    float         jump_speed = 0;
    float         jump_time = 0;
  };
  
  Player player[1];
  const uint32_t size = 1;
};


namespace Player_utils {


void
init_players(Core::World &world,
             Players_container &player_container);


void
move_players(Core::Context &ctx,
             Core::World &world,
             const float delta_time,
             Players_container &player_container,
             Bullet bullets[],
             const uint32_t number_of_bullets);

  
void
hit_player(Core::World &world,
           const Core::Entity_id id,
           Players_container &player_container,
           Explosion explosions[],
           const uint32_t number_of_explosions);


bool
all_dead(Players_container &player_container);


} // ns


#endif // inc guard