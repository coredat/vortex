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
    uint32_t      controller_id   = 0;
    float         point_on_circle = 0.f;
    float         gun_cooldown    = 0.f;
    float         jump_speed      = 0.f;
    float         jump_time       = 0.f;
    float         power_up_timer  = 0.f;
    float         momentum        = 0.f;
  };
  
  Player player[4];
  const uint32_t size = 4;
};


namespace Player_utils {


void
init_players(Core::World &world,
             Players_container &player_container,
             const uint32_t controller_id);

void
selection(Core::World &world,
          Players_container &player_container,
          const uint32_t controller_id,
          const uint32_t direction);


void
move_players(Core::Context &ctx,
             Core::World &world,
             const float delta_time,
             Players_container &players_container,
             Bullets_container &bullets_container);

  
void
hit_player(Core::World &world,
           const Core::Entity_ref &hit,
           Players_container &players_container,
           Explosions_container &explosions_container);
  
  
void
power_up(Core::World &world,
         const Core::Entity_ref hit,
         Players_container &players_container);


bool
all_dead(Players_container &player_container);


} // ns


#endif // inc guard