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
#include <lib/menu_image_list/menu_list.hpp>
#include <assert.h>


namespace {

//  Core::Lib::Menu   menu;
  
  constexpr uint32_t buttons_count = 5;
  Core::Lib::Menu_list::Image_button buttons[buttons_count];
  
} // anon ns


void
title_screen_init(Core::Context &ctx,
                  Core::World &world,
                  Core::Camera &camera)
{
  // Button
  {
    Core::Shader shader(Core::Directory::volatile_resource_path("assets/shaders/basic_fullbright.ogl"));
    assert(shader);
    
    Core::Texture title_texture(Core::Directory::volatile_resource_path("assets/textures/title.png"));
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

    Core::Texture quit_cold(Core::Directory::volatile_resource_path("assets/textures/button_quit_cold.png"));
    assert(quit_cold);
    
    Core::Texture quit_hot(Core::Directory::volatile_resource_path("assets/textures/button_quit_hot.png"));
    assert(quit_hot);
    
    Core::Material cold_quit_button("[button]quit_cold");
    cold_quit_button.set_shader(shader);
    cold_quit_button.set_map_01(quit_cold);

    Core::Material hot_quit_button("[button]quit_hot");
    hot_quit_button.set_shader(shader);
    hot_quit_button.set_map_01(quit_hot);
    
//    menu.set_home(math::vec2_init(50, 50), world, camera, title_mat);
//    menu.add_button("button_start_game", world, hot_start_button, cold_start_button);
//    menu.add_button("button_settings", world, hot_settings_button, cold_settings_button);
//    menu.add_button("button_about", world, hot_about_button, cold_about_button);
//    menu.add_button("button_quit", world, hot_quit_button, cold_quit_button);
    
    // New button thing
    const char *path = Core::Directory::volatile_resource_path("assets/models/unit_cube.obj");
    Core::Model model(path);
    
    enum ENUM_POS {

      button_title_pos,
      button_start_pos,
      button_settings_pos,
      button_about_pos,
      button_quit_pos,
      
      buttons_size
    };
    
    assert(buttons_count == buttons_size);
    
    buttons[button_title_pos].entity = Core::Entity(world);
    buttons[button_title_pos].entity.set_tags(Object_tags::gui_cam);
    buttons[button_title_pos].cold_material = title_mat;
    buttons[button_title_pos].hot_material = Core::Material();

    buttons[button_start_pos].entity = Core::Entity(world);
    buttons[button_start_pos].entity.set_tags(Object_tags::gui_cam);
    buttons[button_start_pos].entity.set_name("button_start_game");
    buttons[button_start_pos].hot_material  = hot_start_button;
    buttons[button_start_pos].cold_material = cold_start_button;

    buttons[button_settings_pos].entity = Core::Entity(world);
    buttons[button_settings_pos].entity.set_tags(Object_tags::gui_cam);
    buttons[button_settings_pos].entity.set_name("button_settings");
    buttons[button_settings_pos].hot_material  = hot_settings_button;
    buttons[button_settings_pos].cold_material = cold_settings_button;

    buttons[button_about_pos].entity = Core::Entity(world);
    buttons[button_about_pos].entity.set_tags(Object_tags::gui_cam);
    buttons[button_about_pos].entity.set_name("button_about");
    buttons[button_about_pos].hot_material  = hot_about_button;
    buttons[button_about_pos].cold_material = cold_about_button;

    buttons[button_quit_pos].entity = Core::Entity(world);
    buttons[button_quit_pos].entity.set_tags(Object_tags::gui_cam);
    buttons[button_quit_pos].entity.set_name("button_quit");
    buttons[button_quit_pos].hot_material  = hot_quit_button;
    buttons[button_quit_pos].cold_material = cold_quit_button;
    
    Core::Lib::Menu_list::inititalize(buttons, buttons_count, model, camera);
  }
}


Game_state
title_screen_update(Core::Context &ctx,
                    Core::World &world,
                    Core::Camera &camera,
                    Game_object::World_objects &objects,
                    const float dt)
{
  constexpr uint32_t number_of_controllers = 4;
  
  const Core::Controller controllers[number_of_controllers]
  {
    Core::Controller(ctx, 0),
    Core::Controller(ctx, 1),
    Core::Controller(ctx, 2),
    Core::Controller(ctx, 3),
  };
  
  Core::Lib::Menu_list::navigate(controllers[0], buttons, buttons_count);
  Core::Lib::Menu_list::mouse_over(camera, world, Core::Input::mouse_get_coordinates(ctx), buttons, buttons_count);
  
//  menu.think(ctx, world, camera);

  const Core::Entity_ref selected_button = buttons[0].entity;
  const uint32_t button_start = Core::Gamepad_button::button_a | Core::Gamepad_button::button_start;

  if(selected_button && strcmp(selected_button.get_name(), "button_start_game") == 0)
  {
    if(controllers[0].is_button_up_on_frame(button_start))
    {
      Core::Lib::Menu_list::clear(buttons, buttons_count);
      return Game_state::selection;
    }
  }

  if(selected_button && strcmp(selected_button.get_name(), "button_settings") == 0)
  {
    if(controllers[0].is_button_up_on_frame(button_start))
    {
      Core::Lib::Menu_list::clear(buttons, buttons_count);
      return Game_state::settings;
    }
  }
  
  if(selected_button && strcmp(selected_button.get_name(), "button_about") == 0)
  {
    if(controllers[0].is_button_up_on_frame(button_start))
    {
      Core::Lib::Menu_list::clear(buttons, buttons_count);
      return Game_state::about;
    }
  }
  
  if(selected_button && strcmp(selected_button.get_name(), "button_quit") == 0)
  {
    if(controllers[0].is_button_up_on_frame(button_start))
    {
      Core::Lib::Menu_list::clear(buttons, buttons_count);
      return Game_state::quit;
    }
  }
  
  for(const auto &ctrl : controllers)
  {
    if(ctrl.is_button_up_on_frame(button_start))
    {
      Core::Lib::Menu_list::clear(buttons, buttons_count);
      return Game_state::selection;
    }
  }

  return Game_state::title_screen;
}