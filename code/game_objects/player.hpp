#ifndef PLAYER_INCLUDED_27EF33BB_69F2_4B56_909A_1FA47B263E56
#define PLAYER_INCLUDED_27EF33BB_69F2_4B56_909A_1FA47B263E56


#include <core/entity/entity.hpp>
#include <core/world/world_fwd.hpp>
#include <core/context/context_fwd.hpp>
#include <game_objects/game_objects_fwd.hpp>
#include <stdint.h>


struct Player
{
  float         point_on_circle = 0;
  uint32_t      controller_id = 0;
  Core::Entity  entity;
  float         gun_cooldown = 0;
};


namespace Player_utils {


void
init_players(Core::World &world,
             Player players[],
             const uint32_t number_of_players);


void
move_players(Core::Context &ctx,
             Core::World &world,
             const float delta_time,
             Player players[],
             const uint32_t number_of_players,
             Bullet bullets[],
             const uint32_t number_of_bullets);


} // ns


#endif // inc guard