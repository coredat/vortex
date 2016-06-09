#ifndef ENEMY_BREEDER_INCLUDED_5119B924_FE44_4F43_94FC_8A568F83F0F0
#define ENEMY_BREEDER_INCLUDED_5119B924_FE44_4F43_94FC_8A568F83F0F0


#include <game_objects/game_objects_fwd.hpp>


namespace Enemy {


void breeder_setup(Game_object::Enemy &enemy_to_setup);


void breeder_update(Game_object::Enemy &enemy,
                    const float dt,
                    Game_object::World_objects &objs);


} // ns


#endif // inc guard