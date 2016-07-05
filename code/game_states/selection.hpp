#ifndef SELECTION_INCLUDED_6FFC83A8_6568_4EEE_993A_E7FEF3C177E9
#define SELECTION_INCLUDED_6FFC83A8_6568_4EEE_993A_E7FEF3C177E9


#include <game_objects/game_objects_fwd.hpp>
#include <common/common_fwd.hpp>
#include <core/common/core_fwd.hpp>


void
selection_init(Core::Context &ctx,
               Core::World &world,
               Core::Camera &camera);


Game_state
selection_update(Core::Context &ctx,
                 Core::World &world,
                 Core::Camera &camera,
                 Game_object::Player *players[],
                 const uint32_t player_count,
                 Game_object::World_objects &objects,
                 const float dt);


#endif // inc guard