#ifndef ENEMY_INCLUDED_C60D1C26_65C3_4BF9_AAA6_23A27358D168
#define ENEMY_INCLUDED_C60D1C26_65C3_4BF9_AAA6_23A27358D168


#include <game_objects/game_objects_fwd.hpp>
#include <core/world/world_fwd.hpp>
#include <core/entity/entity.hpp>
#include <stdint.h>


struct Enemies_container
{
  struct Enemy
  {
    enum class Type
    {
      climber,    // Climbs up and down.
      breeder,    // Climbs to the top and drops eggs.
      egg,        // Spaned by the breader.
      
      size // nothing under here.
    };
  
    float         point_on_circle = 0;
    float         depth = 0;
    float         lifetime = 0;
    int32_t       direction = 1;
    Type          type = Type::climber;    
    Core::Entity  entity;
  };
  
  Enemy enemy[256];
  const uint32_t size = 256;
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
spawn_climber(Core::World &world,
              Enemies_container &enemies_container,
              float point_on_circle,
              float direction,
              float depth);


void
spawn_breeder(Core::World &world,
              Enemies_container &enemies_container,
              float point_on_circle,
              float direction,
              float depth);

void
spawn_egg(Core::World &world,
          Enemies_container &enemy_container,
          float point_on_circle,
          float depth);
  

void
update_enemies(Core::World &world,
               const float dt,
               Enemies_container &enemies_container);


void
hit_enemy(Core::World &world,
          const Core::Entity_ref &hit,
          Enemies_container &enemies_container,
          Explosions_container &explosions_container,
          Powerups_container &powerups_container);


void
explode_all(Core::World &world,
            Enemies_container &enemies_container,
            Explosions_container &explosions_container);


}; // ns


#endif // inc guard