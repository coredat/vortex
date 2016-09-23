#ifndef GAME_OVER_INCLUDED_E0A7B472_4C78_4236_BB98_D0FB6E97A4E6
#define GAME_OVER_INCLUDED_E0A7B472_4C78_4236_BB98_D0FB6E97A4E6


#include <game_states/game_state.hpp>
#include <game_objects/game_objects_fwd.hpp>
#include <common/common_fwd.hpp>
#include <core/common/core_fwd.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/input/controller.hpp>


namespace Game {


class Game_over_screen : public State
{
public:

  explicit        Game_over_screen(Game_object::World_objects &objs,
                                   Core::World &world,
                                   Core::Context &ctx);
  
  std::unique_ptr<State>      on_update() override;
  
private:

  const Core::Entity_ref    m_camera;
  const Core::Controller    m_controllers[4];
  Core::Entity              m_players[4];

};


} // ns


#endif // inc guard