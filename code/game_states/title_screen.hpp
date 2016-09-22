#ifndef TITLE_SCREEN_INCLUDED_5D61F85B_95CE_4964_9709_140358F6B8C7
#define TITLE_SCREEN_INCLUDED_5D61F85B_95CE_4964_9709_140358F6B8C7


#include <game_objects/game_objects_fwd.hpp>
#include <common/common_fwd.hpp>
#include <core/common/core_fwd.hpp>
#include <game_states/game_state.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/input/controller.hpp>
#include <lib/menu_image_list/menu_list.hpp>


namespace Game {
namespace Title_screen_utils {
  
  
constexpr uint32_t get_button_count() { return 5; }
  
  
} // ns


class Title_screen : public Game::State
{
public:
  
  explicit       Title_screen(Game_object::World_objects &objs,
                              Core::World &world,
                              Core::Context &ctx);
        
  std::unique_ptr<State>     on_update() override;
  
private:

  const Core::Entity_ref  m_camera;
  const Core::Controller  m_controllers[4];
  
  Core::Lib::Menu_list::Image_button m_buttons[Title_screen_utils::get_button_count()];

};


} // ns

//
//void
//title_screen_init(Core::Context &ctx,
//                  Core::World &world,
//                  Core::Camera &camera);
//
//
//Game_state
//title_screen_update(Core::Context &ctx,
//                    Core::World &world,
//                    Core::Camera &camera,
//                    Game_object::World_objects &objects,
//                    const float dt);
//


#endif // inc guard