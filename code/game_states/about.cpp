#include <game_states/about.hpp>
#include <game_states/title_screen.hpp>
#include <common/game_state.hpp>
#include <common/object_tags.hpp>
#include <core/context/context.hpp>
#include <core/world/world.hpp>
#include <core/resources/material.hpp>
#include <core/resources/texture.hpp>
#include <core/model/model.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/resources/shader.hpp>
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
#include <assert.h>


namespace
{
  Core::Lib::Menu   menu;
}

void
about_init(Core::Context &context,
           Core::World &world,
           Core::Camera &camera)
{
  // Button
  {
    Core::Shader shader(Core::Directory::volatile_resource_path("assets/shaders/basic_fullbright.ogl"));
    assert(shader);
    
    Core::Texture title_texture(Core::Directory::volatile_resource_path("assets/textures/button_about_cold.png"));
    assert(title_texture);
    
    Core::Material title_mat("[menu]title");
    title_mat.set_shader(shader);
    title_mat.set_map_01(title_texture);
    
    // Start Button
    
    Core::Texture start_game_cold(Core::Directory::volatile_resource_path("assets/textures/button_start_game_cold.png"));
    assert(start_game_cold);
    
    Core::Texture start_game_hot(Core::Directory::volatile_resource_path("assets/textures/button_start_game_hot.png"));
    assert(start_game_hot);
    
    Core::Material cold_start_button("[button]start_cold");
    cold_start_button.set_shader(shader);
    cold_start_button.set_map_01(start_game_cold);

    Core::Material hot_start_button("[button]start_hot");
    hot_start_button.set_shader(shader);
    hot_start_button.set_map_01(start_game_hot);
    
    // Settings Button

    Core::Texture settings_cold(Core::Directory::volatile_resource_path("assets/textures/button_options_cold.png"));
    assert(settings_cold);
    
    Core::Texture settings_hot(Core::Directory::volatile_resource_path("assets/textures/button_options_hot.png"));
    assert(settings_hot);
    
    Core::Material cold_settings_button("[button]settings_cold");
    cold_settings_button.set_shader(shader);
    cold_settings_button.set_map_01(settings_cold);

    Core::Material hot_settings_button("[button]settings_hot");
    hot_settings_button.set_shader(shader);
    hot_settings_button.set_map_01(settings_hot);

    // About Button

    Core::Texture about_cold(Core::Directory::volatile_resource_path("assets/textures/button_about_cold.png"));
    assert(about_cold);
    
    Core::Texture about_hot(Core::Directory::volatile_resource_path("assets/textures/button_about_hot.png"));
    assert(about_hot);
    
    Core::Material cold_about_button("[button]about_cold");
    cold_about_button.set_shader(shader);
    cold_about_button.set_map_01(about_cold);

    Core::Material hot_about_button("[button]about_hot");
    hot_about_button.set_shader(shader);
    hot_about_button.set_map_01(about_hot);

    // Quit Button

    Core::Texture quit_cold(Core::Directory::volatile_resource_path("assets/textures/button_back_cold.png"));
    assert(quit_cold);
    
    Core::Texture quit_hot(Core::Directory::volatile_resource_path("assets/textures/button_back_hot.png"));
    assert(quit_hot);
    
    Core::Material cold_quit_button("[button]quit_cold");
    cold_quit_button.set_shader(shader);
    cold_quit_button.set_map_01(quit_cold);

    Core::Material hot_quit_button("[button]quit_hot");
    hot_quit_button.set_shader(shader);
    hot_quit_button.set_map_01(quit_hot);
    
    menu.set_home(math::vec2_init(50, 50), world, camera, title_mat);
//    menu.add_button("button_start_game", world, hot_start_button, cold_start_button);
//    menu.add_button("button_settings", world, hot_settings_button, cold_settings_button);
//    menu.add_button("button_about", world, hot_about_button, cold_about_button);
    menu.add_button("button_back", world, hot_quit_button, cold_quit_button);
  }

}



Game_state
about_update(Core::Context &ctx,
             Core::World &world,
             Core::Camera &camera,
             Game_object::World_objects &objs)
{
  constexpr uint32_t number_of_controllers = 4;
  
  const Core::Controller controllers[number_of_controllers]
  {
    Core::Controller(ctx, 0),
    Core::Controller(ctx, 1),
    Core::Controller(ctx, 2),
    Core::Controller(ctx, 3),
  };
  
  
  menu.think(ctx, world, camera);

  if(menu.current_button_selected() && strcmp(menu.current_button_selected().get_name(), "button_start_game") == 0)
  {
    if(controllers[0].is_button_up_on_frame(Core::Gamepad_button::button_a))
    {
      menu.clear();
      return Game_state::selection;
    }
  }

  if(menu.current_button_selected() && strcmp(menu.current_button_selected().get_name(), "button_settings") == 0)
  {
    if(controllers[0].is_button_up_on_frame(Core::Gamepad_button::button_a))
    {
      menu.clear();
      return Game_state::settings;
    }
  }
  
  if(menu.current_button_selected() && strcmp(menu.current_button_selected().get_name(), "button_about") == 0)
  {
    if(controllers[0].is_button_up_on_frame(Core::Gamepad_button::button_a))
    {
      menu.clear();
      return Game_state::about;
    }
  }
  
  if(menu.current_button_selected() && strcmp(menu.current_button_selected().get_name(), "button_back") == 0)
  {
    if(controllers[0].is_button_up_on_frame(Core::Gamepad_button::button_a))
    {
      menu.clear();
      return Game_state::title_screen;
    }
  }

  return Game_state::about;
}