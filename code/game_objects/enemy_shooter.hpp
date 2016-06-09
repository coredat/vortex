#ifndef ENEMY_SHOOTER_INCLUDED_A7E34DB5_BCB8_4743_9328_BF1482B0CD41
#define ENEMY_SHOOTER_INCLUDED_A7E34DB5_BCB8_4743_9328_BF1482B0CD41


#include <game_objects/game_objects_fwd.hpp>


namespace Enemy_logic {


void
shooter_setup(Game_object::Enemy &enemy_to_setup);


void
shooter_update(Game_object::Enemy &enemy,
               const float dt,
               Game_object::World_objects &objs);


} // ns


#endif // inc guard