#include <game_states/game_over.hpp>
#include <common/object_tags.hpp>
#include <common/game_state.hpp>
#include <common/screen_cast.hpp>
#include <game_objects/explosion.hpp>
#include <game_objects/enemy.hpp>
#include <game_objects/player.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/renderer.hpp>
#include <core/input/controller.hpp>
#include <core/transform/transform.hpp>
#include <core/resources/shader.hpp>
#include <core/resources/texture.hpp>
#include <core/common/directory.hpp>


namespace
{
  Core::Entity player_entities[4];
  Core::Entity continue_screen;
  Core::Model plane;
  Core::Material continue_material;
  bool created_screen = false;
}


void
game_over_init(Core::Context &ctx,
               Core::World &world)
{
}


Game_state
game_over_update(Core::Context &context,
                 Core::World &world,
                 Core::Camera &cam,
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
        player_entities[i] = Core::Entity(world);
        
        player_entities[i].set_name("screen_game_over[scoreboard]");
        player_entities[i].set_tags(Object_tags::gui_cam);
 
        Core::Material_renderer mat_renderer;
        mat_renderer.set_material(players[i]->get_material());
        mat_renderer.set_model(players[i]->get_model());
        player_entities[i].set_renderer(mat_renderer);
      }
    }
    
    if(!continue_material)
    {
      const char *shader_path = Core::Directory::volatile_resource_path("assets/shaders/basic_fullbright.ogl");
      Core::Shader shader(shader_path);
    
      plane = Core::Model(Core::Directory::volatile_resource_path("assets/models/unit_plane.obj"));
      
      const char *press_start = Core::Directory::volatile_resource_path("assets/textures/choose_ship.png");
      Core::Texture continue_texture(press_start);
      
      continue_material = Core::Material("screen_game_over[continue]");
      continue_material.set_shader(shader);
      continue_material.set_map_01(continue_texture);
    }
    
    if(!continue_screen)
    {
      continue_screen = Core::Entity(world);
      continue_screen.set_name("screen_game_over[start]");
      continue_screen.set_tags(Object_tags::gui_cam);
      
      const Core::Material_renderer mat_renderer(continue_material, plane);
      continue_screen.set_renderer(mat_renderer);
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
        const float offset = -3.f + (i * 2.f);
        
        Core::Transform trans = Screen_cast::intersect_screen_plane(cam, offset, 1.5f);
        trans.set_scale(math::vec3_init(0.01, 0.01, 0.01));

        const math::quat spin_rot = math::quat_init_with_axis_angle(0, 1, 0, time + i);
        const math::quat tilt_rot = math::quat_init_with_axis_angle(0, 0, 1, -0.2f);
        const math::quat rot = math::quat_multiply(tilt_rot, spin_rot);
        trans.set_rotation(rot);
        
        pl.set_transform(trans);
      }
      
      continue_screen.set_transform(Screen_cast::intersect_screen_plane(cam, 0.f, -1.5f));
    }
  }

  Core::Input::Controller controller_1(context, 0);
  Core::Input::Controller controller_2(context, 1);
  Core::Input::Controller controller_3(context, 2);
  Core::Input::Controller controller_4(context, 3);

  /*
    If any gamepad presses start we go back to the game selection screen.
  */
  if(controller_1.is_button_down_on_frame(Core::Input::Button::button_4) ||
     controller_2.is_button_down_on_frame(Core::Input::Button::button_4) ||
     controller_3.is_button_down_on_frame(Core::Input::Button::button_4) ||
     controller_4.is_button_down_on_frame(Core::Input::Button::button_4))
  {
    created_screen = false;
    continue_screen.destroy();
    
    for(auto &pl : player_entities)
    {
      pl.destroy();
    }
    
    return Game_state::selection;
  }

  return Game_state::game_over;
}