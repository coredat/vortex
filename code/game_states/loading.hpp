#ifndef LOADING_INCLUDED_F1D120CC_5CE3_46C3_B409_FB321A456B21
#define LOADING_INCLUDED_F1D120CC_5CE3_46C3_B409_FB321A456B21


#include <common/game_state.hpp>
#include <core/common/core_fwd.hpp>


void
loading_init(Core::Context &context,
             Core::World &world);


Game_state
loading_update(Core::Context &context,
               Core::World &world,
               Core::Camera &cam);


#endif // inc guard