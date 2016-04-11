#ifndef BULLET_INCLUDED_992A35A6_21D1_4D51_BF4E_06917B3704D3
#define BULLET_INCLUDED_992A35A6_21D1_4D51_BF4E_06917B3704D3


#include <game_objects/game_objects_fwd.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <stdint.h>


struct Bullet
{
  int32_t           direction = -1;
  float             speed     = 100.5f;
  float             point_on_circle  = 1;
  Core::Entity      entity;
  Core::Entity_ref  owner;
};


namespace Bullet_utils {


void
init_bullets(Core::World &world,
             Bullet bullets[],
             const uint32_t number_of_bullets);


void
move_bullets(Core::World &world,
             const float dt,
             Bullet bullets[],
             const uint32_t number_of_bullets);


void
create_bullet(Core::World &world,
              const float position,
              const uint32_t direction,
              Bullet bullets[],
              const uint32_t number_of_bullets);

};


#endif // inc guard