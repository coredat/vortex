#include <game_states/quit.hpp>


namespace Game {

Quit::Quit(Game_object::World_objects &objs,
           Core::World &world,
           Core::Context &ctx)
: State(objs, world, ctx)
{
}


} // ns