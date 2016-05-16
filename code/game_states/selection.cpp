#include <game_states/selection.hpp>
#include <game_objects/player.hpp>
#include <common/game_state.hpp>
#include <core/input/controller.hpp>
#include <core/resources/texture.hpp>
#include <core/transform/transform.hpp>
#include <core/model/model.hpp>
#include <utilities/directory.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>


namespace
{
  constexpr uint32_t number_of_textures = 4;
  Core::Texture textures[number_of_textures];
  
  constexpr uint32_t number_of_models = 4;
  Core::Model models[number_of_models];
  
  constexpr uint32_t max_number_of_players = 4;
  uint32_t current_player_selection[max_number_of_players];
  
  Core::Entity selection_screens[max_number_of_players];
  
  Core::Model plane;
}


void
selection_init(Core::Context &ctx,
               Core::World &world)
{
  const std::string asset_path = util::get_resource_path() + "assets/";

  // Load textures
  {
    uint32_t load_texture = 0;
    
    const std::string tex_01 = asset_path + "textures/dev_grid_green_512.png";
    textures[load_texture++] = Core::Texture(tex_01.c_str());
    
    const std::string tex_02 = asset_path + "textures/dev_grid_yellow_512.png";
    textures[load_texture++] = Core::Texture(tex_02.c_str());
    
    const std::string tex_03 = asset_path + "textures/dev_grid_red_512.png";
    textures[load_texture++] = Core::Texture(tex_03.c_str());
    
    const std::string tex_04 = asset_path + "textures/dev_grid_blue_512.png";
    textures[load_texture++] = Core::Texture(tex_04.c_str());
  }
  
  // Load models
  {
    uint32_t load_model = 0;
    const std::string model_01 = asset_path + "models/ship.obj";
    models[load_model++] = Core::Model(model_01.c_str());
    
    const std::string model_02 = asset_path + "models/ship.obj";
    models[load_model++] = Core::Model(model_02.c_str());
    
    const std::string model_03 = asset_path + "models/ship.obj";
    models[load_model++] = Core::Model(model_03.c_str());
    
    const std::string model_04 = asset_path + "models/ship.obj";
    models[load_model++] = Core::Model(model_04.c_str());
  }
  
  // Set player selections
  {
    for(uint32_t p = 0; p < max_number_of_players; ++p)
    {
      current_player_selection[p] = p;
    }
  }
  
  // Selection Screens
  {
    const std::string plane_path = asset_path + "models/unit_plane.obj";
    
    plane = Core::Model(plane_path.c_str());
    
    for(auto &sel : selection_screens)
    {
      sel = Core::Entity(world);
      sel.set_name("Selection screen");
    }
  }
}


Game_state
selection_update(Core::Context &context,
                 Core::World &world,
                 Players_container &players_container,
                 const float dt)
{
  constexpr uint32_t number_of_controllers = 4;
  
  const Core::Input::Controller controllers[number_of_controllers]
  {
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
      // Reset selection screen
      {
        for(auto &sel : selection_screens)
        {
          // Currently the best way to hide an entity is just
          // to create a new one :D
          sel = Core::Entity(world);
        }
      }
    
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
      
      // Add selection screen.
      selection_screens[i].set_model(plane);
      selection_screens[i].set_material_id(textures[selection].get_id());
      
      Core::Transform trans;
      trans.set_position(math::vec3_init(0, 0, -2));
      constexpr float scale = 5;
      trans.set_scale(math::vec3_init(scale, 1, scale * math::g_ratio()));
      trans.set_rotation(math::quat_init_with_axis_angle(1, 0, 0, -math::quart_tau()));
      
      selection_screens[i].set_transform(trans);
    }
  }
  
  return Game_state::selection;
}