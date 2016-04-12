#ifndef EXPLOSION_INCLUDED_0377F28D_5ECA_4177_88FD_311040A61AC5
#define EXPLOSION_INCLUDED_0377F28D_5ECA_4177_88FD_311040A61AC5


#include <core/entity/entity.hpp>
#include <core/world/world_fwd.hpp>
#include <math/vec/vec_fwd.hpp>
#include <stdint.h>


struct Explosion
{
  Core::Entity entity;
  float time = 0.f;
};


namespace Explosion_utils {


void
init_explosions(const Core::World &world,
                Explosion explosions[],
                const uint32_t number_of_explosions);


void
update_explosions(const Core::World &world,
                  const float dt,
                  Explosion explosions[],
                  const uint32_t number_of_explosions);


void
create_explosion(const Core::World &world,
                 const math::vec3 position,
                 Explosion explosions[],
                 const uint32_t number_of_explosions);


} // ns


#endif // inc guard