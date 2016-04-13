#ifndef ENEMY_INCLUDED_C60D1C26_65C3_4BF9_AAA6_23A27358D168
#define ENEMY_INCLUDED_C60D1C26_65C3_4BF9_AAA6_23A27358D168


#include <core/world/world_fwd.hpp>
#include <core/entity/entity.hpp>
#include <game_objects/game_objects_fwd.hpp>
#include <stdint.h>


struct Enemies_container
{
  struct Enemy
  {
    float         point_on_circle = 0;
    float         depth = 0;
    int32_t       direction = 1;
    Core::Entity  entity;
  };
  
  Enemy enemy[128];
  const uint32_t size = 128;
};


namespace Enemy_utils {


void
init_enemies(Core::World &world,
             Enemies_container &enemies_container);


void
spawn_enemies(Core::World &world,
              const float dt,
              Enemies_container &enemies_container);


void
update_enemies(Core::World &world,
               const float dt,
               Enemies_container &enemies_container);


void
hit_enemy(Core::World &world,
          const Core::Entity_id id,
          Enemies_container &enemies_container,
          Explosions_container &explosions_container,
          Powerups_container &powerups_container);


void
explode_all(Core::World &world,
            Enemies_container &enemies_container,
            Explosions_container &explosions_container);


}; // ns


#endif // inc guard