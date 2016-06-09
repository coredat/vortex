#ifndef ENEMY_CLIMBER_INCLUDED_0F8BD771_48AC_445B_9E8A_6FF809080DF2
#define ENEMY_CLIMBER_INCLUDED_0F8BD771_48AC_445B_9E8A_6FF809080DF2


#include <game_objects/game_objects_fwd.hpp>


namespace Enemy_logic {


void climber_setup(Game_object::Enemy &enemy_to_setup);


void climber_update(Game_object::Enemy &enemy,
                    const float dt,
                    Game_object::World_objects &objs);


} // ns


#endif // inc guard