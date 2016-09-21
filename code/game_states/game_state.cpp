#include <game_states/game_state.hpp>


namespace Game {


State::State(Game_object::World_objects &world_objs,
                       Core::World &world,
                       Core::Context &ctx)
: m_world(world)
, m_ctx(ctx)
, m_world_objects(world_objs)
{
}


Game_state
State::on_update()
{
  return Game_state::quit;
}


Core::World&
State::get_world()
{
  return m_world;
}


Core::Context&
State::get_ctx()
{
  return m_ctx;
}


Game_object::World_objects&
State::get_world_objs()
{
  return m_world_objects;
}


} // ns