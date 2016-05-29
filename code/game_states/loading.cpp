#include <game_states/loading.hpp>
#include <core/context/context.hpp>
#include <core/world/world.hpp>


void
loading_init(Core::Context &context, Core::World &world)
{
}


Game_state
loading_update(Core::Context &context, Core::World &world)
{
  return Game_state::selection;
}