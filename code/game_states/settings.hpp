#ifndef SETTINGS_INCLUDED_59256270_54E0_4959_B09A_8F84195EBA6A
#define SETTINGS_INCLUDED_59256270_54E0_4959_B09A_8F84195EBA6A


#include <common/game_state.hpp>
#include <core/common/core_fwd.hpp>
#include <game_objects/world_objects.hpp>


void
settings_init(Core::Context &context,
              Core::World &world,
              Core::Camera &camera);


Game_state
settings_update(Core::Context &context,
                Core::World &world,
                Core::Camera &cam,
                Game_object::World_objects &objs);



#endif // inc guard