#ifndef GAME_STATE_INCLUDED_B2DFB00D_B139_4AEA_8037_A2B5C463C23B
#define GAME_STATE_INCLUDED_B2DFB00D_B139_4AEA_8037_A2B5C463C23B


#include <core/common/core_fwd.hpp>
#include <game_objects/game_objects_fwd.hpp>
#include <common/game_state.hpp>
#include <memory>


namespace Game {


class State
{
public:

  explicit                              State(Game_object::World_objects &objs,
                                              Core::World &world,
                                              Core::Context &ctx);
  
  virtual const char *                  get_name() const;
  virtual std::unique_ptr<State>        on_update();
  

protected:

  Core::World&                          get_world();
  Core::Context&                        get_ctx();
  Game_object::World_objects&           get_world_objs();

private:

  Core::World                           &m_world;
  Core::Context                         &m_ctx;
  Game_object::World_objects            &m_world_objects;

};


} // ns


#endif // inc guard