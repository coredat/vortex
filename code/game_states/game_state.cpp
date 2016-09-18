#include <game_states/game_state.hpp>


namespace Game {


Game_state::Game_state(Core::World &world, Core::Context &ctx)
: m_world(world)
, m_ctx(ctx)
{
}


std::unique_ptr<Game_state>
Game_state::on_update()
{
  return nullptr;
}


Core::World&
Game_state::get_world()
{
  return m_world;
}


Core::Context&
Game_state::get_ctx()
{
  return m_ctx;
}


} // ns