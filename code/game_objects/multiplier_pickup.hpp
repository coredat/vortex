#ifndef MULTIPLIER_PICKUP_INCLUDED_8D88A36C_1AE6_493A_A9AA_30CDBE90A7A0
#define MULTIPLIER_PICKUP_INCLUDED_8D88A36C_1AE6_493A_A9AA_30CDBE90A7A0


#include <core/world/world_fwd.hpp>
#include <stdint.h>


struct Multipliers_container
{
  struct Muliplier
  {
  };
  
  Muliplier multiplier[256];
  const uint32_t size = 256;

};


namespace Multiplier_utils {


void
init_multiplier(Core::World &world);



} // ns


#endif // inc guard