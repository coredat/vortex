#include <game_states/settings.hpp>
#include <game_states/about.hpp>
#include <game_states/title_screen.hpp>
#include <common/game_state.hpp>
#include <common/object_tags.hpp>
#include <game_objects/main_camera.hpp>
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
#include <lib/menu_image_list/menu_list.hpp>
#include <assert.h>


namespace {

constexpr uint32_t buttons_count = 3;
Core::Lib::Menu_list::Image_button buttons[buttons_count];

}


void
settings_init(Core::Context &context,
              Core::World &world,
              Core::Camera &camera)
{
  // Button
  {
    Core::Shader shader(Core::Directory::volatile_resource_path("assets/shaders/basic_fullbright.ogl"));
    assert(shader);
    
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

    // Fullscreen

    Core::Texture fullscreen_cold(Core::Directory::volatile_resource_path("assets/textures/button_fullscreen_cold.png"));
    assert(fullscreen_cold);
    
    Core::Texture fullscreen_hot(Core::Directory::volatile_resource_path("assets/textures/button_fullscreen_hot.png"));
    assert(fullscreen_hot);
    
    Core::Material cold_fullscreen_button("[button]fullscreen_cold");
    cold_fullscreen_button.set_shader(shader);
    cold_fullscreen_button.set_map_01(fullscreen_cold);

    Core::Material hot_fullscreen_button("[button]fullscreen_hot");
    hot_fullscreen_button.set_shader(shader);
    hot_fullscreen_button.set_map_01(fullscreen_hot);

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
    
    buttons[0].entity         = Core::Entity(world);
    buttons[0].entity.set_name("button_title");
    buttons[0].entity.set_tags(Object_tags::gui_cam);
    buttons[0].cold_material  = cold_settings_button;
    buttons[0].hot_material   = Core::Material();
    
    buttons[1].entity         = Core::Entity(world);
    buttons[1].entity.set_name("button_fullscreen");
    buttons[1].entity.set_tags(Object_tags::gui_cam);
    buttons[1].cold_material  = cold_fullscreen_button;
    buttons[1].hot_material   = hot_fullscreen_button;

    buttons[2].entity         = Core::Entity(world);
    buttons[2].entity.set_name("button_back");
    buttons[2].entity.set_tags(Object_tags::gui_cam);
    buttons[2].cold_material  = cold_quit_button;
    buttons[2].hot_material   = hot_quit_button;

    
    const Core::Model model(Core::Directory::volatile_resource_path("assets/models/unit_cube.obj"));
  
    Core::Lib::Menu_list::inititalize(buttons, buttons_count, model, camera);
  }

}


Game_state
settings_update(Core::Context &ctx,
                Core::World &world,
                Core::Camera &camera,
                Game_object::World_objects &objs)
{
  const Core::Controller controller = Core::Controller(ctx, 0);
 
  Core::Lib::Menu_list::navigate(controller, buttons, buttons_count);
  Core::Lib::Menu_list::mouse_over(camera, world, Core::Input::mouse_get_coordinates(ctx), buttons, buttons_count);
  
  const Core::Entity_ref selected_button = buttons[0].entity;
  
  constexpr uint32_t button_start = Core::Gamepad_button::button_a | Core::Gamepad_button::button_start;

  if(selected_button && strcmp(selected_button.get_name(), "button_back") == 0)
  {
    if(controller.is_button_up_on_frame(button_start))
    {
      Core::Lib::Menu_list::clear(buttons, buttons_count);
      return Game_state::title_screen;
    }
  }

  else if(selected_button && strcmp(selected_button.get_name(), "button_fullscreen") == 0)
  {
    if(controller.is_button_up_on_frame(button_start))
    {
      ctx.set_fullscreen(!ctx.is_fullscreen());
      
      if(!ctx.is_fullscreen())
      {
        ctx.set_width(1024);
        ctx.set_height(576);
        ctx.set_title("Vertex Defender");
        
        // Get cameras
        Core::Entity_ref camera_entity = world.find_entity_by_name("Main Camera");
        
        if(camera_entity)
        {
          Game_object::Main_camera *cam_obj = reinterpret_cast<Game_object::Main_camera*>(camera_entity.get_user_data());
          assert(cam_obj);
          
          if(cam_obj)
          {
            cam_obj->m_world_camera.set_height(ctx.get_height());
            cam_obj->m_world_camera.set_width(ctx.get_width());
            
            cam_obj->m_gui_camera.set_height(ctx.get_height());
            cam_obj->m_gui_camera.set_width(ctx.get_width());
            
            cam_obj->m_final_post_camera.set_width(ctx.get_width());
            cam_obj->m_final_post_camera.set_height(ctx.get_height());
            
            cam_obj->m_level_camera.set_width(ctx.get_width());
            cam_obj->m_level_camera.set_height(ctx.get_height());
          }
          
          settings_init(ctx, world, camera);
        }
        
      }
    }
  }
  
  // Generic if back then go back
  if(controller.is_button_up_on_frame(Core::Gamepad_button::button_back))
  {
    Core::Lib::Menu_list::clear(buttons, buttons_count);
    return Game_state::title_screen;
  }

  return Game_state::settings;
}