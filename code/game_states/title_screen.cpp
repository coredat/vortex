#include <game_states/title_screen.hpp>
#include <game_states/selection_screen.hpp>
#include <game_states/quit.hpp>
#include <game_states/about_screen.hpp>
#include <game_states/settings_screen.hpp>
#include <factories/material.hpp>
#include <game_objects/main_camera.hpp>
#include <common/game_state.hpp>
#include <common/object_tags.hpp>
#include <core/context/context.hpp>
#include <core/world/world.hpp>
#include <core/resources/material.hpp>
#include <core/model/model.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/transform/transform.hpp>
#include <core/transform/transform_utils.hpp>
#include <core/common/directory.hpp>
#include <core/camera/camera_properties.hpp>
#include <core/input/controller.hpp>
#include <core/input/buttons.hpp>
#include <core/common/ray.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/box_collider.hpp>
#include <core/renderer/renderer.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/text_renderer.hpp>
#include <core/font/font.hpp>
#include <core/camera/camera.hpp>
#include <core/camera/camera_utils.hpp>
#include <utilities/directory.hpp>
#include <math/quat/quat.hpp>
#include <utilities/logging.hpp>
#include <renderer/debug_line.hpp>
#include <lib/menu/menu.hpp>
#include <lib/menu_image_list/menu_list.hpp>
#include <assert.h>
#include <core/input/input.hpp>


namespace Game {


Title_screen::Title_screen(Game_object::World_objects &objs,
                           Core::World &world,
                           Core::Context &ctx)
: State(objs, world, ctx)
, m_camera(get_world().find_entity_by_name("Main Camera"))
, m_controllers{
    Core::Controller(get_ctx(), 0),
    Core::Controller(get_ctx(), 1),
    Core::Controller(get_ctx(), 2),
    Core::Controller(get_ctx(), 3),
  }
{
  // We use mouse for screen menu.
  Core::Input::mouse_set_capture(get_ctx(), false);

  assert(m_camera);
  
  Game_object::Main_camera *main_camera = reinterpret_cast<Game_object::Main_camera*>(m_camera.get_user_data());
  assert(main_camera);

  // Button
  {
    enum ENUM_POS {

      button_title_pos,
      button_start_pos,
      button_settings_pos,
      button_about_pos,
      button_quit_pos,
      
      buttons_size
    };
    
    assert(Title_screen_utils::get_button_count() == buttons_size);
    
    m_buttons[button_title_pos].entity = Core::Entity(world);
    m_buttons[button_title_pos].entity.set_tags(Object_tags::gui_cam);
    m_buttons[button_title_pos].cold_material = Factory::Material::get_title_menu_title();
    m_buttons[button_title_pos].hot_material = Core::Material();

    m_buttons[button_start_pos].entity = Core::Entity(world);
    m_buttons[button_start_pos].entity.set_tags(Object_tags::gui_cam);
    m_buttons[button_start_pos].entity.set_name("button_start_game");
    m_buttons[button_start_pos].hot_material  = Factory::Material::get_title_menu_start_hot();
    m_buttons[button_start_pos].cold_material = Factory::Material::get_title_menu_start_cold();

    m_buttons[button_settings_pos].entity = Core::Entity(world);
    m_buttons[button_settings_pos].entity.set_tags(Object_tags::gui_cam);
    m_buttons[button_settings_pos].entity.set_name("button_settings");
    m_buttons[button_settings_pos].hot_material  = Factory::Material::get_title_menu_options_hot();
    m_buttons[button_settings_pos].cold_material = Factory::Material::get_title_menu_options_cold();

    m_buttons[button_about_pos].entity = Core::Entity(world);
    m_buttons[button_about_pos].entity.set_tags(Object_tags::gui_cam);
    m_buttons[button_about_pos].entity.set_name("button_about");
    m_buttons[button_about_pos].hot_material  = Factory::Material::get_title_menu_about_hot();
    m_buttons[button_about_pos].cold_material = Factory::Material::get_title_menu_about_cold();

    m_buttons[button_quit_pos].entity = Core::Entity(world);
    m_buttons[button_quit_pos].entity.set_tags(Object_tags::gui_cam);
    m_buttons[button_quit_pos].entity.set_name("button_quit");
    m_buttons[button_quit_pos].hot_material  = Factory::Material::get_title_menu_quit_hot();
    m_buttons[button_quit_pos].cold_material = Factory::Material::get_title_menu_quit_cold();
    
    const Core::Model model(Core::Directory::volatile_resource_path("assets/models/unit_cube.obj"));
    Core::Lib::Menu_list::inititalize(m_buttons, Title_screen_utils::get_button_count(), model, main_camera->m_gui_camera);
  }
}


std::unique_ptr<State>
Title_screen::on_update()
{
  Game_object::Main_camera *main_camera = reinterpret_cast<Game_object::Main_camera*>(m_camera.get_user_data());
  assert(main_camera);

  Core::Lib::Menu_list::navigate(m_controllers[0], m_buttons, Title_screen_utils::get_button_count());
  Core::Lib::Menu_list::mouse_over(main_camera->m_gui_camera, get_world(), Core::Input::mouse_get_coordinates(get_ctx()), m_buttons, Title_screen_utils::get_button_count());
  
//  menu.think(ctx, world, camera);

  const Core::Entity_ref selected_button = m_buttons[0].entity;
  constexpr uint32_t button_start = Core::Gamepad_button::button_a | Core::Gamepad_button::button_start;

  if(selected_button && strcmp(selected_button.get_name(), "button_start_game") == 0)
  {
    if(m_controllers[0].is_button_up_on_frame(button_start))
    {
      Core::Lib::Menu_list::clear(m_buttons, Title_screen_utils::get_button_count());
//      return Game_state::selection;
      return std::unique_ptr<State>(new Game::Selection_screen(get_world_objs(), get_world(), get_ctx()));
    }
  }

  if(selected_button && strcmp(selected_button.get_name(), "button_settings") == 0)
  {
    if(m_controllers[0].is_button_up_on_frame(button_start))
    {
      Core::Lib::Menu_list::clear(m_buttons, Title_screen_utils::get_button_count());
      return std::unique_ptr<State>(new Game::Settings_screen(get_world_objs(), get_world(), get_ctx()));
    }
  }
  
  if(selected_button && strcmp(selected_button.get_name(), "button_about") == 0)
  {
    if(m_controllers[0].is_button_up_on_frame(button_start))
    {
      Core::Lib::Menu_list::clear(m_buttons, Title_screen_utils::get_button_count());
      return std::unique_ptr<State>(new Game::About_screen(get_world_objs(), get_world(), get_ctx()));
    }
  }
  
  if(selected_button && strcmp(selected_button.get_name(), "button_quit") == 0)
  {
    if(m_controllers[0].is_button_up_on_frame(button_start))
    {
      Core::Lib::Menu_list::clear(m_buttons, Title_screen_utils::get_button_count());
      return std::unique_ptr<State>(new Game::Quit(get_world_objs(), get_world(), get_ctx()));
    }
  }
  
  for(const auto &ctrl : m_controllers)
  {
    if(ctrl.is_button_up_on_frame(button_start))
    {
      Core::Lib::Menu_list::clear(m_buttons, Title_screen_utils::get_button_count());
      return std::unique_ptr<State>(new Game::Selection_screen(get_world_objs(), get_world(), get_ctx()));
    }
  }

return nullptr;
}


} // ns