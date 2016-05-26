#ifndef GAME_OVER_INCLUDED_E0A7B472_4C78_4236_BB98_D0FB6E97A4E6
#define GAME_OVER_INCLUDED_E0A7B472_4C78_4236_BB98_D0FB6E97A4E6


#include <game_objects/game_objects_fwd.hpp>
#include <common/game_state.hpp>
#include <core/context/context_fwd.hpp>
#include <core/world/world_fwd.hpp>


void
game_over_init(Core::Context &ctx,
          Core::World &world);


Game_state
game_over_update(Core::Context &context,
                 Core::World &world,
//                 Explosions_container  &explosions_container,
                 const float dt);


#endif // inc guard