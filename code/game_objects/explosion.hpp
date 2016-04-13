#ifndef EXPLOSION_INCLUDED_0377F28D_5ECA_4177_88FD_311040A61AC5
#define EXPLOSION_INCLUDED_0377F28D_5ECA_4177_88FD_311040A61AC5


#include <core/entity/entity.hpp>
#include <core/world/world_fwd.hpp>
#include <math/vec/vec_fwd.hpp>
#include <stdint.h>


struct Explosions_container
{
  struct Explosion
  {
    Core::Entity entity;
    float time = 0.f;
  };
  
  Explosion explosion[128];
  const uint32_t size = 128;
};


namespace Explosion_utils {


void
init_explosions(const Core::World &world,
                Explosions_container &explosions_container);


void
update_explosions(const Core::World &world,
                  const float dt,
                  Explosions_container &explosions_container);


void
create_explosion(const Core::World &world,
                 const math::vec3 position,
                 Explosions_container &explosions_container);


} // ns


#endif // inc guard