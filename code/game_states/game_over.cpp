#include <game_states/game_over.hpp>
#include <common/object_tags.hpp>
#include <common/game_state.hpp>
#include <lib/menu/button.hpp>
#include <common/screen_cast.hpp>
#include <game_objects/explosion.hpp>
#include <game_objects/enemy.hpp>
#include <game_objects/player.hpp>
#include <core/context/context.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/renderer.hpp>
#include <core/input/controller.hpp>
#include <core/input/buttons.hpp>
#include <core/transform/transform.hpp>
#include <core/resources/shader.hpp>
#include <core/resources/texture.hpp>
#include <core/common/directory.hpp>


namespace
{
  Core::Entity player_entities[4];
  Core::Model plane;
  Core::Material continue_material;
  bool created_screen = false;
  
  Core::Lib::Button continue_button;
}


void
game_over_init(Core::Context &ctx,
               Core::World &world)
{
}


Game_state
game_over_update(Core::Context &ctx,
                 Core::World &world,
                 Core::Camera &cam,
                 Core::Camera &gui_cam,
                 Game_object::Player *players[],
                 const uint32_t player_count,
                 Game_object::World_objects &objs,
                 const float dt)
{
  if(!created_screen)
  {
    created_screen = true;
    
    for(uint32_t i = 0; i < player_count; ++i)
    {
      if(players[i]->is_valid())
      {
        players[i]->clear_ui_and_ship();
      
        player_entities[i] = Core::Entity(world);
        
        player_entities[i].set_name("screen_game_over[scoreboard]");
        player_entities[i].set_tags(Object_tags::world_cam);
 
        const Core::Material_renderer mat_renderer(players[i]->get_material(), players[i]->get_model());
        player_entities[i].set_renderer(mat_renderer);
      }
    }
    
    if(!continue_material)
    {
      const char *shader_path = Core::Directory::volatile_resource_path("assets/shaders/basic_fullbright.ogl");
      const Core::Shader shader(shader_path);
    
      plane = Core::Model(Core::Directory::volatile_resource_path("assets/models/unit_plane.obj"));
      
      const char *press_start = Core::Directory::volatile_resource_path("assets/textures/dev_grid_orange_512.png");
      const Core::Texture continue_texture(press_start);
      
      continue_material = Core::Material("screen_game_over[continue]");
      continue_material.set_shader(shader);
      continue_material.set_map_01(continue_texture);
    }
    
    if(!continue_button)
    {
      // Continue button
      {
        const Core::Texture hot_texture(Core::Directory::volatile_resource_path("assets/textures/button_continue_hot.png"));
        const Core::Texture cold_texture(Core::Directory::volatile_resource_path("assets/textures/button_continue_cold.png"));
        
        continue_button = Core::Lib::Button(world,
                                            ctx,
                                            "continue2",
                                            math::vec2_init(ctx.get_width() / 2, (ctx.get_height() / 6) * 5),
                                            gui_cam,
                                            hot_texture,
                                            cold_texture);
      }
    }
  }
  
  bool button_pushed = false;
  bool button_hovered = false;
  
  if(continue_button.is_over(gui_cam, world, ctx))
  {
    button_hovered = true;
    
    if(continue_button.was_touched(gui_cam, world, ctx))
    {
      button_pushed = true;
    }
  }
  
  /*
    Result screens
  */
  {
    static float time = 0;
    time += dt;

    for(uint32_t i = 0; i < 4; ++i)
    {
      auto &pl = player_entities[i];
      
      if(pl)
      {
        const float quart_screen = math::to_float(ctx.get_width() >> 2);
        const float x_offset     = quart_screen + (quart_screen * i);
        const float y_offset     = math::to_float(ctx.get_height() >> 3) * 3.f;
        
        const math::vec3 pos = Screen_cast::intersect_screen_plane(cam, x_offset, y_offset);
        Core::Transform trans;
        trans.set_position(pos);
        trans.set_scale(math::vec3_init(1.f));

        const math::quat spin_rot = math::quat_init_with_axis_angle(0, 1, 0, time + i);
        const math::quat tilt_rot = math::quat_init_with_axis_angle(0, 0, 1, -0.2f);
        const math::quat rot = math::quat_multiply(tilt_rot, spin_rot);
        trans.set_rotation(rot);
        
        pl.set_transform(trans);
      }
    }
  }

  Core::Controller controller_1(ctx, 0);
  Core::Controller controller_2(ctx, 1);
  Core::Controller controller_3(ctx, 2);
  Core::Controller controller_4(ctx, 3);

  /*
    If any gamepad presses start we go back to the game selection screen.
  */
  if(controller_1.is_button_down_on_frame(Core::Gamepad_button::button_start) ||
     controller_2.is_button_down_on_frame(Core::Gamepad_button::button_start) ||
     controller_3.is_button_down_on_frame(Core::Gamepad_button::button_start) ||
     controller_4.is_button_down_on_frame(Core::Gamepad_button::button_start) ||
     button_pushed)
  {
    created_screen = false;
    continue_button = Core::Lib::Button();
    
    for(auto &pl : player_entities)
    {
      if(pl)
      {
        pl.destroy();
      }
    }
    
    return Game_state::selection;
  }
  
  if(controller_1.is_button_down_on_frame(Core::Gamepad_button::button_back) ||
     controller_2.is_button_down_on_frame(Core::Gamepad_button::button_back) ||
     controller_3.is_button_down_on_frame(Core::Gamepad_button::button_back) ||
     controller_4.is_button_down_on_frame(Core::Gamepad_button::button_back))
  {
    created_screen = false;
    continue_button = Core::Lib::Button();
    
    for(auto &pl : player_entities)
    {
      if(pl)
      {
        pl.destroy();
      }
    }
    
    return Game_state::title_screen;
  }

  return Game_state::game_over;
}