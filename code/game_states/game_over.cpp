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


namespace
{
  Core::Entity *player_entities[4] = {nullptr, nullptr, nullptr, nullptr};
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
        player_entities[i] = new Core::Entity(world);
        
        player_entities[i]->set_name("Entity Score Screen");
        player_entities[i]->set_tags(Object_tags::gui_cam);
 
        Core::Material_renderer mat_renderer;
        mat_renderer.set_material(players[i]->get_material());
        mat_renderer.set_model(players[i]->get_model());
        player_entities[i]->set_renderer(mat_renderer);
      }
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
        
        pl->set_transform(trans);
      }
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
    
    for(auto &pl : player_entities)
    {
      if(pl)
      {
        delete pl;
      }
      
      pl = nullptr;
    }
    
    return Game_state::selection;
  }

  return Game_state::game_over;
}