#include <game_states/selection.hpp>
#include <game_objects/world_objects.hpp>
#include <game_objects/player.hpp>
#include <common/object_tags.hpp>
#include <common/screen_cast.hpp>
#include <common/game_state.hpp>
#include <core/input/controller.hpp>
#include <core/resources/texture.hpp>
#include <core/resources/shader.hpp>
#include <core/resources/material.hpp>
#include <core/transform/transform.hpp>
#include <core/camera/camera.hpp>
#include <core/camera/camera_utils.hpp>
#include <core/model/model.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <utilities/directory.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>


namespace
{
  constexpr uint32_t  number_of_materials = 4;
  
  Core::Material      materials[number_of_materials];
  Core::Material      no_selection_material;
  Core::Material      selection_material;
  Core::Material      start_game_material;

  constexpr uint32_t  number_of_models = 4;
  Core::Model         models[number_of_models];
  Core::Model         plane;
  
  constexpr uint32_t  max_number_of_players = 4;
  uint32_t            players_signed_in = 0;
  uint32_t            current_player_selection[max_number_of_players];
  
  Core::Entity        selection_screens[max_number_of_players];
  Core::Entity        *signed_in_selections[max_number_of_players];
}


void
selection_init(Core::Context &ctx,
               Core::World &world,
               Core::Camera &cam)
{
  const std::string asset_path = util::get_resource_path() + "assets/";

  // Load materials
  if(!no_selection_material || !selection_material || !start_game_material)
  {
    const std::string shader_path = util::get_resource_path() + "assets/shaders/basic_fullbright.ogl";
    Core::Shader shader(shader_path.c_str());
    
    // --
    
    const std::string no_ship_tex = asset_path + "textures/no_ship.png";
    Core::Texture no_selection_texture(no_ship_tex.c_str());
    
    no_selection_material = Core::Material("selection-none");
    no_selection_material.set_shader(shader);
    no_selection_material.set_map_01(no_selection_texture);
    
    // --
    
    const std::string choose_ship = asset_path + "textures/choose_ship.png";
    Core::Texture choose_ship_texture(choose_ship.c_str());
    
    selection_material = Core::Material("selection");
    selection_material.set_shader(shader);
    selection_material.set_map_01(choose_ship_texture);
    
    // --

    const std::string press_start = asset_path + "textures/choose_ship.png";
    Core::Texture press_start_texture(choose_ship.c_str());
    
    start_game_material = Core::Material("press-start");
    start_game_material.set_shader(shader);
    start_game_material.set_map_01(press_start_texture);
    
  }

  // Load materials
  if(!materials[0])
  {
    const std::string shader_path = util::get_resource_path() + "assets/shaders/basic_fullbright.ogl";
    Core::Shader shader(shader_path.c_str());
  
    for(uint32_t i = 0; i < number_of_materials; ++i)
    {
      char buffer[PATH_MAX];
      sprintf(buffer, "player-mat-%02d", i + 1);
      materials[i] = Core::Material(buffer);
      materials[i].set_shader(shader);
      
      memset(buffer, 0, sizeof(buffer));
      sprintf(buffer, "%stextures/ship_%02d.png", asset_path.c_str(), i + 1);
      
      materials[i].set_map_01(Core::Texture(buffer));
    }
  }

  // Load models
  if(!models[0])
  {
    for(uint32_t i = 0; i < number_of_models; ++i)
    {
      char buffer[PATH_MAX];
      sprintf(buffer, "%smodels/ship_%02d.obj", asset_path.c_str(), i + 1);
      
      models[i] = Core::Model(buffer);
    }
  }
  
  // Set player selections
  {
    players_signed_in = 0;
    
    for(uint32_t p = 0; p < max_number_of_players; ++p)
    {
      current_player_selection[p] = p;
    }
  }
  
  // Selection Screens
  {
    char plane_path[PATH_MAX];
    sprintf(plane_path, "%smodels/unit_plane.obj", asset_path.c_str());
    
    plane = Core::Model(plane_path);
    
    for(auto &sel : selection_screens)
    {
      sel = Core::Entity(world);
      sel.set_name("Selection screen");
      sel.set_tags(Object_tags::gui_cam);
      
      const Core::Material_renderer mat_renderer(no_selection_material, plane);
      sel.set_renderer(mat_renderer);
    }
  }
  
  for(auto &sel : signed_in_selections)
  {
    sel = nullptr;
  }
}


Game_state
selection_update(Core::Context &context,
                 Core::World &world,
                 Core::Camera &cam,
                 Game_object::World_objects &objects,
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
    if(ctrl.is_button_up_on_frame(Core::Input::Button::button_4) && players_signed_in > 0)
    {
      // Reset selection screen
      {
        for(auto &sel : selection_screens)
        {
          // Currently the best way to hide an entity is just
          // to create a new one :D
          sel.destroy();
        }
      }
      
      // Remove player selection screens
      // And spawn the player.
      {
        uint32_t controller_id = 0;
        
        for(auto &sel : signed_in_selections)
        {
          if(sel)
          {
            auto new_player = new Game_object::Player(world, context, controller_id);
            new_player->get_entity().set_renderer(sel->get_renderer());
            
            objects.push_object(new_player);
          
            sel->destroy();
            delete sel;
            sel = nullptr;
          }
          
          ++controller_id;
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
    if(!signed_in_selections[i])
    {
      if(controllers[i].is_button_down_on_frame(Core::Input::Button::button_0))
      {
        const Core::Material_renderer mat_renderer(materials[0], models[0]);
        
        signed_in_selections[i] = new Core::Entity(world);
        signed_in_selections[i]->set_name("selection-entity");
        signed_in_selections[i]->set_tags(Object_tags::gui_cam);
        signed_in_selections[i]->set_renderer(mat_renderer);
        signed_in_selections[i]->set_transform(selection_screens[i].get_transform());
        
        ++players_signed_in;
      }
    }
    
    if(controllers[i].is_button_down_on_frame(Core::Input::Button::button_0))
    {
      current_player_selection[i] = (current_player_selection[i] + 1) % number_of_materials;
      const uint32_t selection = current_player_selection[i];
     
      const Core::Material_renderer player_renderer(materials[selection], models[selection]);
      signed_in_selections[i]->set_renderer(player_renderer);
      
      const Core::Material_renderer sel_renderer(selection_material, plane);
      selection_screens[i].set_renderer(sel_renderer);
    }
  }
  
  /*
    Update sel screens
  */
  for(uint32_t i = 0; i < 4; ++i)
  {
    auto &sel = selection_screens[i];
    
    const float offset = -3.f + (i * 2.f);
    
    sel.set_transform(Screen_cast::intersect_screen_plane(cam, offset, 0));
  
    if(signed_in_selections[i])
    {
      auto sel_trans = selection_screens[i].get_transform();
      sel_trans.set_scale(math::vec3_init(0.01, 0.01, 0.01));
      sel_trans.set_position(math::vec3_add(sel.get_transform().get_position(), math::vec3_init(0,0,0.002f)));
      
      signed_in_selections[i]->set_transform(sel_trans);
    }
  }
  
  /*
    Update the signed in players rotation.
  */
  {
    static float time = 0;
    time += dt;
  
    for(uint32_t i = 0; i < number_of_controllers; ++i)
    {
      Core::Entity *sel = signed_in_selections[i];
    
      if(sel && sel->is_valid())
      {
        Core::Transform trans = sel->get_transform();

        const math::quat spin_rot = math::quat_init_with_axis_angle(0, 1, 0, time + i);
        const math::quat tilt_rot = math::quat_init_with_axis_angle(0, 0, 1, -0.2f);
        const math::quat rot = math::quat_multiply(tilt_rot, spin_rot);
    
        trans.set_rotation(rot);
        
        sel->set_transform(trans);
      }
    }
  }
  
  return Game_state::selection;
}