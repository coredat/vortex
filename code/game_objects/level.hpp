#ifndef LEVEL_INCLUDED_C38EF587_A418_4228_8DEC_FC3C9E1307A6
#define LEVEL_INCLUDED_C38EF587_A418_4228_8DEC_FC3C9E1307A6


#include <core/entity/entity.hpp>
#include <core/world/world_fwd.hpp>


struct Level_container
{
  struct Level_data
  {
    Core::Entity entity;
  };
  
  Level_data level[1];
  const uint32_t size = 1;
};

namespace Level_utils {


void
init_level(Core::World &world, Level_container &level);


};


#endif // inc guard