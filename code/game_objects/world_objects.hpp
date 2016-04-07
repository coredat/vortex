#ifndef WORLD_OBJECTS_INCLUDED_4589EB4C_B10D_4CF1_BF51_02C3529B8B2E
#define WORLD_OBJECTS_INCLUDED_4589EB4C_B10D_4CF1_BF51_02C3529B8B2E


#include <game_objects/bullet.hpp>


namespace Factory {


void      create_player();


} // ns


namespace Game_Objects {


void*     get_player_data();
uint32_t  get_number_of_players();


} // ns



#endif // inc guard