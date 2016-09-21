#ifndef SETTINGS_INCLUDED_59256270_54E0_4959_B09A_8F84195EBA6A
#define SETTINGS_INCLUDED_59256270_54E0_4959_B09A_8F84195EBA6A


#include <game_states/game_state.hpp>
#include <common/game_state.hpp>
#include <core/common/core_fwd.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/input/controller.hpp>
#include <game_objects/world_objects.hpp>
#include <lib/menu_image_list/menu_list.hpp>


namespace Game {
namespace Settings_screen_utils {


constexpr uint32_t get_button_count() { return 3; }


} // ns


class Settings_screen : public Game::State
{
public:

  explicit       Settings_screen(Game_object::World_objects &objs,
                                 Core::World &world,
                                 Core::Context &ctx);
  
  Game_state     on_update() override;
  
private:

  const Core::Entity_ref  m_camera;
  const Core::Controller  m_controllers[4];
  
  Core::Lib::Menu_list::Image_button m_buttons[Settings_screen_utils::get_button_count()];
  

}; // class


} // ns


#endif // inc guard