#ifndef ENEMY_EGG_INCLUDED_BEBBB13D_D8C4_4F54_A6C9_23AEBD64C55E
#define ENEMY_EGG_INCLUDED_BEBBB13D_D8C4_4F54_A6C9_23AEBD64C55E


#include <game_objects/game_objects_fwd.hpp>


namespace Enemy_logic {


void egg_setup(Game_object::Enemy &enemy_to_setup);


void egg_update(Game_object::Enemy &enemy,
                const float dt,
                Game_object::World_objects &objs);


} // ns


#endif // inc guard