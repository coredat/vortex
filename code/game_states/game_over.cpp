#include <game_states/game_over.hpp>
#include <game_objects/explosion.hpp>
#include <game_objects/enemy.hpp>
#include <core/input/controller.hpp>


void
game_over_init(Core::Context &ctx,
               Core::World &world)
{
}


Game_state
game_over_update(Core::Context &context,
                 Core::World &world,
                 const float dt)
{
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
    return Game_state::selection;
  }

  return Game_state::game_over;
}