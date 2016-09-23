#include <game_states/settings_screen.hpp>
#include <factories/material.hpp>
#include <game_states/about_screen.hpp>
#include <game_states/title_screen.hpp>
#include <common/game_state.hpp>
#include <common/object_tags.hpp>
#include <game_objects/main_camera.hpp>
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
#include <lib/menu_image_list/menu_list.hpp>
#include <assert.h>


namespace Game {


Settings_screen::Settings_screen(Game_object::World_objects &objs,
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
 
  // Buttons
  {
    m_buttons[0].entity         = Core::Entity(world);
    m_buttons[0].entity.set_name("button_title");
    m_buttons[0].entity.set_tags(Object_tags::gui_cam);
    m_buttons[0].cold_material  = Factory::Material::get_settings_menu_title();
    m_buttons[0].hot_material   = Core::Material();
    
    m_buttons[1].entity         = Core::Entity(world);
    m_buttons[1].entity.set_name("button_fullscreen");
    m_buttons[1].entity.set_tags(Object_tags::gui_cam);
    m_buttons[1].cold_material  = Factory::Material::get_settings_menu_fullscreen_cold();
    m_buttons[1].hot_material   = Factory::Material::get_settings_menu_fullscreen_hot();

    m_buttons[2].entity         = Core::Entity(world);
    m_buttons[2].entity.set_name("button_back");
    m_buttons[2].entity.set_tags(Object_tags::gui_cam);
    m_buttons[2].cold_material  = Factory::Material::get_menu_back_cold();
    m_buttons[2].hot_material   = Factory::Material::get_menu_back_hot();

    
    const Core::Model model(Core::Directory::volatile_resource_path("assets/models/unit_cube.obj"));
  
    Core::Lib::Menu_list::inititalize(m_buttons,
                                      Settings_screen_utils::get_button_count(),
                                      model,
                                      main_camera->m_gui_camera);
  }
}


std::unique_ptr<State>
Settings_screen::on_update()
{
  Game_object::Main_camera *main_camera = reinterpret_cast<Game_object::Main_camera*>(m_camera.get_user_data());
  assert(main_camera);


  Core::Lib::Menu_list::navigate(m_controllers[0],
                                 m_buttons,
                                 Settings_screen_utils::get_button_count());
  
  Core::Lib::Menu_list::mouse_over(main_camera->m_gui_camera,
                                   get_world(),
                                   Core::Input::mouse_get_coordinates(get_ctx()),
                                   m_buttons,
                                   Settings_screen_utils::get_button_count());
  
  const Core::Entity_ref selected_button = m_buttons[0].entity;
  
  constexpr uint32_t button_start = Core::Gamepad_button::button_a | Core::Gamepad_button::button_start;

  if(selected_button && strcmp(selected_button.get_name(), "button_back") == 0)
  {
    if(m_controllers[0].is_button_up_on_frame(button_start))
    {
      Core::Lib::Menu_list::clear(m_buttons, Settings_screen_utils::get_button_count());
      return std::unique_ptr<State>(new Game::Title_screen(get_world_objs(), get_world(), get_ctx()));
    }
  }

  else if(selected_button && strcmp(selected_button.get_name(), "button_fullscreen") == 0)
  {
    if(m_controllers[0].is_button_up_on_frame(button_start))
    {
      get_ctx().set_fullscreen(!get_ctx().is_fullscreen());
      
      if(!get_ctx().is_fullscreen())
      {
        get_ctx().set_resolution(1024, 576);
        get_ctx().set_title("Vertex Defender");
      }
      
      // Get cameras
      
      if(main_camera)
      {
        main_camera->m_world_camera.set_height(get_ctx().get_height());
        main_camera->m_world_camera.set_width(get_ctx().get_width());
        
        main_camera->m_gui_camera.set_height(get_ctx().get_height());
        main_camera->m_gui_camera.set_width(get_ctx().get_width());
        
        main_camera->m_final_post_camera.set_width(get_ctx().get_width());
        main_camera->m_final_post_camera.set_height(get_ctx().get_height());
        
        main_camera->m_level_camera.set_width(get_ctx().get_width());
        main_camera->m_level_camera.set_height(get_ctx().get_height());
      
        Core::Lib::Menu_list::clear(m_buttons, Settings_screen_utils::get_button_count());
        return std::unique_ptr<State>(new Game::Settings_screen(get_world_objs(), get_world(), get_ctx()));
      }
    }
  }
  
  // Generic if back then go back
  if(m_controllers[0].is_button_up_on_frame(Core::Gamepad_button::button_back))
  {
    Core::Lib::Menu_list::clear(m_buttons, Settings_screen_utils::get_button_count());
    return std::unique_ptr<State>(new Game::Title_screen(get_world_objs(), get_world(), get_ctx()));
  }

  return nullptr;
}


} // ns