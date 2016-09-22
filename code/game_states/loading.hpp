#ifndef LOADING_INCLUDED_F1D120CC_5CE3_46C3_B409_FB321A456B21
#define LOADING_INCLUDED_F1D120CC_5CE3_46C3_B409_FB321A456B21


#include <game_states/game_state.hpp>
#include <common/game_state.hpp>
#include <core/common/core_fwd.hpp>
#include <core/entity/entity.hpp>


namespace Game {


class Loading_screen : public Game::State
{
public:

  explicit        Loading_screen(Game_object::World_objects &objs,
                                 Core::World &world,
                                 Core::Context &ctx);
  
  Game_state      on_update() override;

private:
  
  Core::Entity    m_loading_entity;
  float           m_timer = 0.f;

};


} // ns


#endif // inc guard