#include <game_states/selection.hpp>
#include <factories/material.hpp>
#include <game_objects/world_objects.hpp>
#include <game_objects/player.hpp>
#include <game_objects/player_ship.hpp>
#include <game_objects/player_ui.hpp>
#include <lib/menu/button.hpp>
#include <common/object_tags.hpp>
#include <common/screen_cast.hpp>
#include <common/game_state.hpp>
#include <core/context/context.hpp>
#include <core/input/controller.hpp>
#include <core/input/buttons.hpp>
#include <core/resources/texture.hpp>
#include <core/resources/material.hpp>
#include <core/transform/transform.hpp>
#include <core/camera/camera.hpp>
#include <core/camera/camera_utils.hpp>
#include <core/model/model.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/color/color.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/common/directory.hpp>
#include <utilities/file.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <utilities/assert.hpp>


namespace
{
  constexpr uint32_t  number_of_materials = 4;
  
  Core::Material      materials[number_of_materials];
  Core::Material      no_selection_material;
  Core::Material      selection_material;

  constexpr uint32_t  number_of_models = 4;
  Core::Model         models[number_of_models];
  Core::Model         plane;
  
  constexpr uint32_t  max_number_of_players = 4;
  uint32_t            players_signed_in = 0;
  uint32_t            current_player_selection[max_number_of_players];
  
  Core::Entity        selection_screens[max_number_of_players];
  Core::Entity        signed_in_selections[max_number_of_players];
  
  Core::Lib::Button   continue_button;
}


void
selection_init(Core::Context &ctx,
               Core::World &world,
               Core::Camera &cam)
{
  // Load materials
  no_selection_material = Factory::Material::get_selection_none_controller();
  selection_material    = Factory::Material::get_selection_choose_ship_controller();
  
  uint32_t curr_mat = 0;
  materials[curr_mat++] = Factory::Material::get_ship_01_material();
  materials[curr_mat++] = Factory::Material::get_ship_02_material();
  materials[curr_mat++] = Factory::Material::get_ship_03_material();
  materials[curr_mat++] = Factory::Material::get_ship_04_material();
  assert(curr_mat == number_of_materials);
  
  // Load models
  if(!models[0])
  {
    for(uint32_t i = 0; i < number_of_models; ++i)
    {
      char buffer[MAX_FILE_PATH_SIZE];
      sprintf(buffer, "%sassets/models/ship_%02d.obj", Core::Directory::volatile_resource_path(""), i + 1);
      
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
    plane = Core::Model(Core::Directory::volatile_resource_path("assets/models/unit_plane.obj"));
    
    for(auto &sel : selection_screens)
    {
      sel = Core::Entity(world);
      sel.set_name("Selection[choose]");
      sel.set_tags(Object_tags::gui_cam);
      
      const Core::Material_renderer mat_renderer(no_selection_material, plane);
      sel.set_renderer(mat_renderer);
    }
  }
}


Game_state
selection_update(Core::Context &ctx,
                 Core::World &world,
                 Core::Camera &cam,
                 Core::Camera &gui_cam,
                 Game_object::Player *players[],
                 const uint32_t player_count,
                 Game_object::World_objects &objects,
                 const float dt)
{
  constexpr uint32_t number_of_controllers = 4;
  
  const Core::Controller controllers[number_of_controllers]
  {
    Core::Controller(ctx, 0),
    Core::Controller(ctx, 1),
    Core::Controller(ctx, 2),
    Core::Controller(ctx, 3),
  };
  
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
    If p1 hits start we start.
  */
  for(const auto &ctrl : controllers)
  {
    if((button_pushed || ctrl.is_button_up_on_frame(Core::Gamepad_button::button_start)) && players_signed_in > 0)
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
        for(uint32_t i = 0; i < 4; ++i)
        {
          auto &sel = signed_in_selections[i];
          
          if(sel)
          {
            players[i]->set_model(static_cast<Core::Material_renderer>(sel.get_renderer()).get_model());
            players[i]->set_material(static_cast<Core::Material_renderer>(sel.get_renderer()).get_material());
            players[i]->set_controller(i);
            
            objects.push_object(players[i]->spawn_ship(ctx));
            objects.push_object(players[i]->spawn_ui(ctx, gui_cam, cam));
            
            sel.destroy();
          }
        }
      }
      
      // Reset button
      continue_button = Core::Lib::Button();
      
      return Game_state::game_mode;
    }
  }
  
  /*
    Selection
  */
  for(uint32_t i = 0; i < number_of_controllers; ++i)
  {
    if(!signed_in_selections[i])
    {
      if(controllers[i].is_button_down_on_frame(Core::Gamepad_button::button_a))
      {
        const Core::Material_renderer mat_renderer(materials[0], models[0]);
        
        signed_in_selections[i] = Core::Entity(world);
        signed_in_selections[i].set_name("selection[ship-entity]");
        signed_in_selections[i].set_tags(Object_tags::world_cam);
        signed_in_selections[i].set_renderer(mat_renderer);
        signed_in_selections[i].set_transform(selection_screens[i].get_transform());
        
        ++players_signed_in;
      }
    }
    
    if(controllers[i].is_button_down_on_frame(Core::Gamepad_button::button_a))
    {
      // Start screen
      if(!continue_button)
      {
        // Continue button
        {
          const Core::Texture hot_texture(Core::Directory::volatile_resource_path("assets/textures/button_continue_hot.png"));
          const Core::Texture cold_texture(Core::Directory::volatile_resource_path("assets/textures/button_continue_cold.png"));
          
          continue_button = Core::Lib::Button(world,
                                              ctx,
                                              "continue",
                                              math::vec2_init(ctx.get_width() / 2, (ctx.get_height() / 6) * 5),
                                              gui_cam,
                                              hot_texture,
                                              cold_texture);
        }
      }
      
      if(button_hovered && i == 0)
      {
        continue;
      }
    
      current_player_selection[i] = (current_player_selection[i] + 1) % number_of_materials;
      const uint32_t selection = current_player_selection[i];
     
      const Core::Material_renderer player_renderer(materials[selection], models[selection]);
      signed_in_selections[i].set_renderer(player_renderer);
      
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
    
    const float screen_div = math::to_float(ctx.get_width()) / 5.f;
    const float x_offset   = (screen_div + (screen_div * i)) - (ctx.get_width() / 2.f);
    const float y_offset   = math::to_float(ctx.get_height() >> 1);

    sel.set_transform(Core::Transform(
      math::vec3_init(x_offset, 0, 0),
      math::vec3_init(128.f, 1.f, 128.f),
      math::quat_init_with_axis_angle(Core::Transform::get_world_left(), -math::quart_tau())
    ));

    if(signed_in_selections[i])
    {
      const float screen_div = math::to_float(ctx.get_width()) / 5.f;
      const float x_offset   = screen_div + (screen_div * i);
      const float y_offset   = math::to_float(ctx.get_height() >> 1);

      auto sel_trans = selection_screens[i].get_transform();
      sel_trans.set_scale(math::vec3_init(1.f));
      sel_trans.set_position(Screen_cast::intersect_screen_plane(cam, x_offset, y_offset));
      
      signed_in_selections[i].set_transform(sel_trans);
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
      Core::Entity *sel = &signed_in_selections[i];
    
      if(sel && sel->is_valid())
      {
        Core::Transform trans = sel->get_transform();
  
        const math::quat spin_rot = math::quat_init_with_axis_angle(0.f, 1.f, 0.f, time + i);
        const math::quat tilt_rot = math::quat_init_with_axis_angle(0.f, 0.f, 1.f, -0.2f);
        const math::quat rot      = math::quat_multiply(tilt_rot, spin_rot);
    
        trans.set_rotation(rot);
        
        sel->set_transform(trans);
      }
    }
  }
  
  
  /*
    Go back a screen!
  */
  for(uint32_t i = 0; i < number_of_controllers; ++i)
  {
    if(controllers[i].is_button_up_on_frame(Core::Gamepad_button::button_back | Core::Gamepad_button::button_b))
    {
      for(uint32_t i = 0; i < 4; ++i)
      {
        auto &sel = signed_in_selections[i];
        
        if(sel)
        {
          sel.destroy();
        }
      }
      
      for(auto &sel : selection_screens)
      {
        // Currently the best way to hide an entity is just
        // to create a new one :D
        sel.destroy();
      }
      
      continue_button = Core::Lib::Button();
      
      return Game_state::title_screen;
    }
  }
  
  return Game_state::selection;
}