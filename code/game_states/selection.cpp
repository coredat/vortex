#include <game_states/selection.hpp>
#include <game_objects/player.hpp>
#include <common/game_state.hpp>
#include <core/input/controller.hpp>
#include <core/resources/texture.hpp>
#include <core/model/model.hpp>
#include <utilities/directory.hpp>


namespace
{
  constexpr uint32_t number_of_textures = 4;
  Core::Texture textures[number_of_textures];
  
  constexpr uint32_t number_of_models = 4;
  Core::Model models[number_of_models];
  
  constexpr uint32_t max_number_of_players = 4;
  uint32_t current_player_selection[max_number_of_players];
}


void
selection_init(Core::Context &ctx,
               Core::World &world)
{
  // Load textures
  {
    uint32_t load_texture = 0;
    
    const std::string tex_01 = util::get_resource_path() + "assets/textures/dev_grid_green_512.png";
    textures[load_texture++] = Core::Texture(tex_01.c_str());
    
    const std::string tex_02 = util::get_resource_path() + "assets/textures/dev_grid_yellow_512.png";
    textures[load_texture++] = Core::Texture(tex_02.c_str());
    
    const std::string tex_03 = util::get_resource_path() + "assets/textures/dev_grid_red_512.png";
    textures[load_texture++] = Core::Texture(tex_03.c_str());
    
    const std::string tex_04 = util::get_resource_path() + "assets/textures/dev_grid_blue_512.png";
    textures[load_texture++] = Core::Texture(tex_04.c_str());
  }
  
  // Load models
  {
    uint32_t load_model = 0;
    const std::string model_01 = util::get_resource_path() + "assets/models/ship.obj";
    models[load_model++] = Core::Model(model_01.c_str());
    
    const std::string model_02 = util::get_resource_path() + "assets/models/ship.obj";
    models[load_model++] = Core::Model(model_02.c_str());
    
    const std::string model_03 = util::get_resource_path() + "assets/models/ship.obj";
    models[load_model++] = Core::Model(model_03.c_str());
    
    const std::string model_04 = util::get_resource_path() + "assets/models/ship.obj";
    models[load_model++] = Core::Model(model_04.c_str());
  }
  
  // Set player selections
  {
    for(uint32_t p = 0; p < max_number_of_players; ++p)
    {
      current_player_selection[p] = p;
    }
  }
}

#include <iostream>
Game_state
selection_update(Core::Context &context,
                 Core::World &world,
                 Players_container &players_container,
                 const float dt)
{
  constexpr uint32_t number_of_controllers = 4;
  
  Core::Input::Controller controllers[number_of_controllers] = {
    Core::Input::Controller(context, 0),
    Core::Input::Controller(context, 1),
    Core::Input::Controller(context, 2),
    Core::Input::Controller(context, 3),
  };
  
  /*
    If p1 hits start we start.
  */
  for(const auto &ctrl : controllers)
  {
    if(ctrl.is_button_down(Core::Input::Button::button_4))
    {
      return Game_state::game_mode;
    }
  }
  
  /*
    Add players as the push their buttons.
  */
  for(uint32_t i = 0; i < number_of_controllers; ++i)
  {
    if(controllers[i].is_button_down(Core::Input::Button::button_0))
    {
      Player_utils::init_players(world, players_container, i);
    }
    
    if(controllers[i].is_button_down_on_frame(Core::Input::Button::button_0))
    {
      current_player_selection[i] = (current_player_selection[i] + 1) % number_of_textures;
      const uint32_t selection = current_player_selection[i];
      
      Player_utils::selection(world,
                              players_container,
                              i,
                              models[selection],
                              textures[selection]);
    }
  }
  
  return Game_state::selection;
}