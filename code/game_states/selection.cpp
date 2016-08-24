#include <game_states/selection.hpp>
#include <game_objects/world_objects.hpp>
#include <game_objects/player.hpp>
#include <game_objects/player_ship.hpp>
#include <common/object_tags.hpp>
#include <common/screen_cast.hpp>
#include <common/game_state.hpp>
#include <core/context/context.hpp>
#include <core/input/controller.hpp>
#include <core/input/axis.hpp>
#include <core/input/buttons.hpp>
#include <core/resources/texture.hpp>
#include <core/resources/shader.hpp>
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
#include <utilities/directory.hpp>
#include <utilities/file.hpp>
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
  Core::Entity        signed_in_selections[max_number_of_players];
  Core::Entity        start_screen;
}


void
selection_init(Core::Context &ctx,
               Core::World &world,
               Core::Camera &cam)
{
  // Load materials
  if(!no_selection_material || !selection_material || !start_game_material)
  {
    const char *shader_path = Core::Directory::volatile_resource_path("assets/shaders/basic_fullbright.ogl");
    Core::Shader shader(shader_path);
    
    // --
    
    const char *no_ship_tex = Core::Directory::volatile_resource_path("assets/textures/no_ship.png");
    Core::Texture no_selection_texture(no_ship_tex);
    
    no_selection_material = Core::Material("selection-none");
    no_selection_material.set_shader(shader);
    no_selection_material.set_map_01(no_selection_texture);
    
    no_selection_material.set_color(0x33333399);
    
    // --
    
    const char *choose_ship = Core::Directory::volatile_resource_path("assets/textures/choose_ship.png");
    Core::Texture choose_ship_texture(choose_ship);
    
    selection_material = Core::Material("selection");
    selection_material.set_shader(shader);
    selection_material.set_map_01(choose_ship_texture);
    
    // --

    const char *press_start = Core::Directory::volatile_resource_path("assets/textures/dev_grid_orange_512.png");
    Core::Texture press_start_texture(press_start);
    
    start_game_material = Core::Material("press-start");
    start_game_material.set_shader(shader);
    start_game_material.set_map_01(press_start_texture);
    
  }

  // Load materials
  if(!materials[0])
  {
    const char *shader_path = Core::Directory::volatile_resource_path("assets/shaders/vortex_dir_light.ogl");
    Core::Shader shader(shader_path);
    assert(shader);
  
    for(uint32_t i = 0; i < number_of_materials; ++i)
    {
      char buffer[MAX_FILE_PATH_SIZE];
      sprintf(buffer, "player-mat-%02d", i + 1);
      materials[i] = Core::Material(buffer);
      materials[i].set_shader(shader);
      
      memset(buffer, 0, sizeof(buffer));
      sprintf(buffer, "%sassets/textures/ship_%02d.png", util::dir::resource_path(), i + 1);
      
      materials[i].set_map_01(Core::Texture(buffer));
    }
  }

  // Load models
  if(!models[0])
  {
    for(uint32_t i = 0; i < number_of_models; ++i)
    {
      char buffer[MAX_FILE_PATH_SIZE];
      sprintf(buffer, "%sassets/models/ship_%02d.obj", util::dir::resource_path(), i + 1);
      
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
  
  /*
    If p1 hits start we start.
  */
  for(const auto &ctrl : controllers)
  {
    if(ctrl.is_button_up_on_frame(Core::Gamepad_button::button_start) && players_signed_in > 0)
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
        start_screen.destroy();
      
        for(uint32_t i = 0; i < 4; ++i)
        {
          auto &sel = signed_in_selections[i];
          
          if(sel)
          {
            players[i]->set_model(static_cast<Core::Material_renderer>(sel.get_renderer()).get_model());
            players[i]->set_material(static_cast<Core::Material_renderer>(sel.get_renderer()).get_material());
            players[i]->set_controller(i);
            
            objects.push_object(players[i]->spawn_ship(ctx));
            
            sel.destroy();
          }
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
    
    if(controllers[i].is_button_down_on_frame(Core::Gamepad_button::button_a))// || controllers[i].get_axis(0).y != 0.f)
    {
      // Start screen
      if(!start_screen)
      {
        start_screen = Core::Entity(world);
        start_screen.set_name("Selection[start]");
        start_screen.set_tags(Object_tags::gui_cam);
        
        const Core::Material_renderer mat_renderer(start_game_material, plane);
        start_screen.set_renderer(mat_renderer);
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
    
    constexpr float offsets[] = {-2.5f, -0.8f, 0.8f, 2.5f};
    
//    const float offset = -3.f + (i * 2.f);
    const float offset = offsets[i];
    
//    sel.set_transform(Screen_cast::intersect_screen_plane(cam, offset, 1.5f));
//    sel.set_transform(Screen_cast::intersect_screen_plane(cam, offset, 1.5f));
    
    const float horz_quart_screen = math::to_float(ctx.get_width()) / 6.f;
    const float horz_margin = horz_quart_screen * 0.5f;
    const float horz_half_screen = math::to_float(ctx.get_width()) / 3.f;
    const float horz_pos = (i * horz_quart_screen) - horz_half_screen + horz_margin;

    sel.set_transform(Core::Transform(
      math::vec3_init(horz_pos, 0, 0),
      math::vec3_init(128.f, 1.f, 128.f),
      math::quat_init_with_axis_angle(Core::Transform::get_world_left(), -math::quart_tau())
    ));

    
    if(signed_in_selections[i])
    {
      Core::Transform trans = Screen_cast::intersect_screen_plane(cam, offset, 0.f);
    
      auto sel_trans = selection_screens[i].get_transform();
      sel_trans.set_scale(math::vec3_init(0.01, 0.01, 0.01));
      sel_trans.set_position(math::vec3_add(trans.get_position(), math::vec3_init(0,0,0.002f)));
      
      signed_in_selections[i].set_transform(sel_trans);
    }
  }
  
  /*
    Update
  */
  {
    if(start_screen)
    {
      start_screen.set_transform(Core::Transform(
        math::vec3_init(0, math::to_float(ctx.get_height()) / -3.f, 0),
        math::vec3_init(128.f, 1.f, 64.f),
        math::quat_init_with_axis_angle(Core::Transform::get_world_left(), -math::quart_tau())
      ));
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
        const math::quat rot = math::quat_multiply(tilt_rot, spin_rot);
    
        trans.set_rotation(rot);
        
        sel->set_transform(trans);
      }
    }
  }
  
  return Game_state::selection;
}