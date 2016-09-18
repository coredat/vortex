#ifndef GAME_STATE_INCLUDED_
#define GAME_STATE_INCLUDED_


#include <core/common/core_fwd.hpp>
#include <memory>


namespace Game {


class Game_state
{
public:

  explicit                              Game_state(Core::World &world, Core::Context &ctx);
  virtual std::unique_ptr<Game_state>   on_update();

protected:

  Core::World&      get_world();
  Core::Context&    get_ctx();

private:

  Core::World       &m_world;
  Core::Context     &m_ctx;

};


} // ns


#endif // inc guard