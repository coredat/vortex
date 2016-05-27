#include <game_states/game_over.hpp>
#include <game_objects/explosion.hpp>
#include <game_objects/enemy.hpp>
#include <core/input/controller.hpp>
#include <iostream>


void
game_over_init(Core::Context &ctx,
               Core::World &world)
{
  std::cout << "foop" << std::endl;
}


Game_state
game_over_update(Core::Context &context,
                 Core::World &world,
                 const float dt)
{
//  Enemy_utils::explode_all(world, enemies_container, explosions_container);

  Core::Input::Controller controller(context, 0);
  if(controller.is_button_down_on_frame(Core::Input::Button::button_4))
  {
    std::cout << "button" << std::endl;
    return Game_state::selection;
  }

  return Game_state::game_mode;
}