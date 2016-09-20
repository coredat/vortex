#ifndef ENEMY_BOOMERANG_INCLUDED_
#define ENEMY_BOOMERANG_INCLUDED_


#include <game_objects/game_objects_fwd.hpp>


namespace Enemy_logic {


void boomerang_setup(Game_object::Enemy &enemy_to_setup);


void boomerang_update(Game_object::Enemy &enemy,
                      const float dt,
                      Game_object::World_objects &objs);


} // ns


#endif // inc guard