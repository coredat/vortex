#ifndef ABOUT_INCLUDED_A339A585_0152_4012_888E_FA26EE6DA61D
#define ABOUT_INCLUDED_A339A585_0152_4012_888E_FA26EE6DA61D


#include <common/game_state.hpp>
#include <core/common/core_fwd.hpp>
#include <game_objects/world_objects.hpp>


void
about_init(Core::Context &context,
           Core::World &world,
           Core::Camera &camera);


Game_state
about_update(Core::Context &context,
             Core::World &world,
             Core::Camera &cam,
             Game_object::World_objects &objs);


#endif // inc guard