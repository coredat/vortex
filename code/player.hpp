#ifndef PLAYER_INCLUDED_27EF33BB_69F2_4B56_909A_1FA47B263E56
#define PLAYER_INCLUDED_27EF33BB_69F2_4B56_909A_1FA47B263E56


#include <stdint.h>
#include <core/context/context.hpp>


struct Player
{
  
};


namespace Player_utils {


void
move_players(Core::Context &ctx, Player players[], const uint32_t number_of_players);


} // ns


#endif // inc guard