#ifndef POWERUP_PICKUP_INCLUDED_2CCA1A14_6DA1_497E_A3D9_1C1DFCA16DF9
#define POWERUP_PICKUP_INCLUDED_2CCA1A14_6DA1_497E_A3D9_1C1DFCA16DF9


#include <core/world/world_fwd.hpp>
#include <core/entity/entity.hpp>
#include <stdint.h>


struct Powerups_container {

  struct Powerup
  {
    Core::Entity entity;
  };
  
  Powerup powerup[8];
  const uint32_t size = 8;

};


namespace Powerup_utils {


void
init_powerups(Core::World &world,
              Powerups_container &powerups_container);


void
update_powerups(Core::World &world,
                Powerups_container &powerups_container,
                const float dt);


void
create_powerup(Core::World &world,
               Powerups_container &powerups_container);


void
destroy_powerup(Core::World &world,
                Powerups_container &powerups_container);


} // ns


#endif // inc guard