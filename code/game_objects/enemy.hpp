#ifndef ENEMY_INCLUDED_C60D1C26_65C3_4BF9_AAA6_23A27358D168
#define ENEMY_INCLUDED_C60D1C26_65C3_4BF9_AAA6_23A27358D168


#include <core/world/world_fwd.hpp>
#include <core/entity/entity.hpp>
#include <game_objects/game_objects_fwd.hpp>
#include <stdint.h>


struct Enemy
{
  float         point_on_circle = 0;
  float         depth = 0;
  int32_t       direction = 1;
  Core::Entity  entity;
};


namespace Enemy_utils {


void
init_enemies(Core::World &world,
             Enemy *enemy_arr,
             const uint32_t number_of_entities);


void
update_enemies(Core::World &world,
               const float dt,
               Enemy *enemy_arr,
               const uint32_t number_of_entities);


void
hit_enemy(Core::World &world,
          const Core::Entity_id id,
          Enemy enemy_arr[],
          const uint32_t number_of_entities,
          Explosion explosions_arr[],
          const uint32_t number_of_explosions);


}; // ns


#endif // inc guard