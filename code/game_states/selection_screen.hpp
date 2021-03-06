#ifndef SELECTION_INCLUDED_6FFC83A8_6568_4EEE_993A_E7FEF3C177E9
#define SELECTION_INCLUDED_6FFC83A8_6568_4EEE_993A_E7FEF3C177E9


#include <game_states/game_state.hpp>
#include <game_objects/game_objects_fwd.hpp>
#include <common/common_fwd.hpp>
#include <core/common/core_fwd.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/resources/material.hpp>
#include <core/input/controller.hpp>
#include <core/model/model.hpp>
#include <lib/menu/button.hpp>


namespace Game {
namespace Selection_screen_utils {


constexpr uint32_t get_max_players() { return 4; }
constexpr uint32_t get_max_models() { return 4; }


} // ns


class Selection_screen : public Game::State
{
public:

  explicit       Selection_screen(Game_object::World_objects &objs,
                                  Core::World &world,
                                  Core::Context &ctx);
  
  std::unique_ptr<State>     on_update() override;

private:

  const Core::Entity_ref    m_camera;
  const Core::Controller    m_controllers[4];
  const Core::Material      m_materials[Selection_screen_utils::get_max_models()];
  const Core::Model         m_models[Selection_screen_utils::get_max_models()];
  
  Core::Lib::Button         m_continue_button;
  Core::Entity              m_selection_screens[Selection_screen_utils::get_max_players()];
  Core::Entity              m_signed_in_selections[Selection_screen_utils::get_max_players()];
  uint32_t                  m_current_player_selection[Selection_screen_utils::get_max_players()];
  uint32_t                  m_players_signed_in = 0;


};


} // ns


#endif // inc guard