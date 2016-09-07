#ifndef GAME_OVER_INCLUDED_E0A7B472_4C78_4236_BB98_D0FB6E97A4E6
#define GAME_OVER_INCLUDED_E0A7B472_4C78_4236_BB98_D0FB6E97A4E6


#include <game_objects/game_objects_fwd.hpp>
#include <common/common_fwd.hpp>
#include <core/common/core_fwd.hpp>


void
game_over_init(Core::Context &ctx,
               Core::World &world);


Game_state
game_over_update(Core::Context &context,
                 Core::World &world,
                 Core::Camera &camera,
                 Core::Camera &gui_cam,
                 Game_object::Player *players[],
                 const uint32_t player_count,
                 Game_object::World_objects &objs,                               
                 const float dt);


#endif // inc guard