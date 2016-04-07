#ifndef BULLET_INCLUDED_992A35A6_21D1_4D51_BF4E_06917B3704D3
#define BULLET_INCLUDED_992A35A6_21D1_4D51_BF4E_06917B3704D3


#include <game_objects/game_objects_fwd.hpp>
#include <core/entity/entity.hpp>
#include <stdint.h>


struct Bullet
{
  int32_t         direction = 1;
  float           speed     = 1;
  Core::Entity    entity;
};


namespace Bullet_utils {


void
init_bullets();


void
create_bullets(World_objects &obj);


void
move_bullets();


};


#endif // inc guard