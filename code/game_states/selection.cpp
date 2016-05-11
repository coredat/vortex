#include <game_states/selection.hpp>
#include <core/input/controller.hpp>
#include <common/game_state.hpp>
#include <game_objects/player.hpp>


namespace
{
  
}


void
selection_init(Core::Context &ctx,
               Core::World &world)
{
  
}


Game_state
selection_update(Core::Context &context,
                 Core::World &world,
                 Players_container &players_container,
                 const float dt)
{
  // Wait for input.
  Core::Input::Controller controller_01(context, 0);
  Core::Input::Controller controller_02(context, 1);
  Core::Input::Controller controller_03(context, 2);
  Core::Input::Controller controller_04(context, 3);
  
  /*
    If p1 hits start we start.
  */
  if(controller_01.is_button_down(Core::Input::Button::button_4) ||
     controller_02.is_button_down(Core::Input::Button::button_4) ||
     controller_03.is_button_down(Core::Input::Button::button_4) ||
     controller_04.is_button_down(Core::Input::Button::button_4))
  {
    return Game_state::game_mode;
  }
  
  /*
    Add players as the push their buttons.
  */
  if(controller_01.is_button_down(Core::Input::Button::button_0))
  {
    Player_utils::init_players(world, players_container, 0);
  }
  
  if(controller_01.is_button_down_on_frame(Core::Input::Button::button_0))
  {
    Player_utils::selection(world, players_container, 0, +1);
  }
  
  if(controller_02.is_button_down(Core::Input::Button::button_0))
  {
    Player_utils::init_players(world, players_container, 1);
  }
  
  if(controller_03.is_button_down(Core::Input::Button::button_0))
  {
    Player_utils::init_players(world, players_container, 2);
  }
  
  if(controller_04.is_button_down(Core::Input::Button::button_0))
  {
    Player_utils::init_players(world, players_container, 3);
  }
  
  return Game_state::selection;
}