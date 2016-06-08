#ifndef TITLE_SCREEN_INCLUDED_5D61F85B_95CE_4964_9709_140358F6B8C7
#define TITLE_SCREEN_INCLUDED_5D61F85B_95CE_4964_9709_140358F6B8C7


#include <game_objects/game_objects_fwd.hpp>
#include <game_objects/world_objects.hpp>
#include <common/game_state.hpp>
#include <common/common_fwd.hpp>
#include <core/world/world_fwd.hpp>
#include <core/context/context_fwd.hpp>
#include <core/camera/camera_fwd.hpp>


void
title_screen_init(Core::Context &ctx,
                  Core::World &world,
                  Core::Camera &camera);


Game_state
title_screen_update(Core::Context &ctx,
                    Core::World &world,
                    Core::Camera &camera,
                    Game_object::World_objects &objects,
                    const float dt);



#endif // inc guard