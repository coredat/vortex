#ifndef GAME_INCLUDED_74006336_21A6_4078_906C_81BFDC593032
#define GAME_INCLUDED_74006336_21A6_4078_906C_81BFDC593032


#include <game_states/game_state.hpp>
#include <game_objects/game_objects_fwd.hpp>
#include <core/entity/entity_ref.hpp>


namespace Game {


class Game_screen : public State
{
public:

  explicit          Game_screen(Game_object::World_objects &objs,
                                Core::World &world,
                                Core::Context &ctx);
  
  std::unique_ptr<State>        on_update() override;
  
private:

  const Core::Entity_ref  m_camera;
  float             m_spawn_timer = 0.f;
  float             m_game_timer  = 0.f;

};


} // ns


#endif // inc guard