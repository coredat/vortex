#include <game_states/selection_screen.hpp>
#include <game_states/title_screen.hpp>
#include <game_states/game_screen.hpp>
#include <game_objects/main_camera.hpp>
#include <game_objects/world_objects.hpp>
#include <game_objects/player.hpp>
#include <game_objects/player_ship.hpp>
#include <game_objects/player_ui.hpp>
#include <common/object_tags.hpp>
#include <common/screen_cast.hpp>
#include <common/game_state.hpp>
#include <factories/material.hpp>
#include <core/world/world.hpp>
#include <core/context/context.hpp>
#include <core/input/buttons.hpp>
#include <core/resources/texture.hpp>
#include <core/transform/transform.hpp>
#include <core/transform/transform_utils.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/common/directory.hpp>


namespace Game {


Selection_screen::Selection_screen(Game_object::World_objects &objs,
                                   Core::World &world,
                                   Core::Context &ctx)
: State(objs, world, ctx)
, m_camera(get_world().find_entity_by_name("Main Camera"))
, m_controllers{
    Core::Controller(get_ctx(), 0),
    Core::Controller(get_ctx(), 1),
    Core::Controller(get_ctx(), 2),
    Core::Controller(get_ctx(), 3),
  }
, m_materials{
    Core::Material(Factory::Material::get_ship_01_material()),
    Core::Material(Factory::Material::get_ship_02_material()),
    Core::Material(Factory::Material::get_ship_03_material()),
    Core::Material(Factory::Material::get_ship_04_material()),
  }
, m_models{
    Core::Model(Core::Directory::volatile_resource_path("assets/models/ship_01.obj")),
    Core::Model(Core::Directory::volatile_resource_path("assets/models/ship_02.obj")),
    Core::Model(Core::Directory::volatile_resource_path("assets/models/ship_03.obj")),
    Core::Model(Core::Directory::volatile_resource_path("assets/models/ship_04.obj")),
  }
, m_selection_screens{
    Core::Entity(get_world()),
    Core::Entity(get_world()),
    Core::Entity(get_world()),
    Core::Entity(get_world()),
  }
, m_signed_in_selections{
    Core::Entity(),
    Core::Entity(),
    Core::Entity(),
    Core::Entity(),
  }
, m_current_player_selection{
    0,
    0,
    0,
    0,
  }
, m_players_signed_in(0)
{
  Core::Input::mouse_set_capture(get_ctx(), false);
  assert(m_camera);
  
  // Set player selections
  {
    m_players_signed_in = 0;
    
    for(uint32_t p = 0; p < Selection_screen_utils::get_max_models(); ++p)
    {
      m_current_player_selection[p] = p;
    }
  }
  
  // Selection Screens
  {
    for(auto &sel : m_selection_screens)
    {
      sel.set_name("Selection[choose]");
      sel.set_tags(Object_tags::gui_cam);
      
      const Core::Material_renderer mat_renderer(
        Factory::Material::get_selection_none_controller(),
        Core::Model(Core::Directory::volatile_resource_path("assets/models/unit_plane.obj")
      ));
        
      sel.set_renderer(mat_renderer);
    }
  }

}


std::unique_ptr<State>
Selection_screen::on_update()
{
  Game_object::Main_camera *main_camera = reinterpret_cast<Game_object::Main_camera*>(m_camera.get_user_data());
  assert(main_camera);

  bool button_pushed = false;
  bool button_hovered = false;
  
  if(m_continue_button.is_over(main_camera->m_gui_camera, get_world(), get_ctx()))
  {
    button_hovered = true;
    
    if(m_continue_button.was_touched(main_camera->m_gui_camera, get_world(), get_ctx()))
    {
      button_pushed = true;
    }
  }
  
  /*
    If p1 hits start we start.
  */
  for(const auto &ctrl : m_controllers)
  {
    if((button_pushed || ctrl.is_button_up_on_frame(Core::Gamepad_button::start)) && m_players_signed_in > 0)
    {
      // Reset selection screen
      {
        for(auto &sel : m_selection_screens)
        {
          // Currently the best way to hide an entity is just
          // to create a new one :D
          sel.destroy();
        }
      }
      
      Core::Entity_ref *refs_arr;
      size_t refs_count = 0;
      get_world().find_entities_by_name("Player", &refs_arr, &refs_count);
      assert(refs_count == 4);
      
      Game_object::Player *players[4];
      
      for(uint32_t i = 0 ; i < 4; ++i)
      {
        players[i] = reinterpret_cast<Game_object::Player*>(refs_arr[i].get_user_data());
      }
      
      // Remove player selection screens
      // And spawn the player.
      {
        for(uint32_t i = 0; i < 4; ++i)
        {
          auto &sel = m_signed_in_selections[i];
          
          if(sel)
          {
            players[i]->set_model(static_cast<Core::Material_renderer>(sel.get_renderer()).get_model());
            players[i]->set_material(static_cast<Core::Material_renderer>(sel.get_renderer()).get_material());
            players[i]->set_controller(i);
            
            get_world_objs().push_object(players[i]->spawn_ship(get_ctx()));
            get_world_objs().push_object(players[i]->spawn_ui(get_ctx(), main_camera->m_gui_camera, main_camera->m_world_camera));
            
            sel.destroy();
          }
        }
      }
      
      // Reset button
      m_continue_button = Core::Lib::Button();
      
      return std::unique_ptr<State>(new Game::Game_screen(get_world_objs(), get_world(), get_ctx()));

    }
  }
  
  /*
    Selection
  */
  for(uint32_t i = 0; i < 4; ++i)
  {
    if(!m_signed_in_selections[i])
    {
      if(m_controllers[i].is_button_down_on_frame(Core::Gamepad_button::a))
      {
        const Core::Material_renderer mat_renderer(m_materials[0], m_models[0]);
        
        m_signed_in_selections[i] = Core::Entity(get_world());
        m_signed_in_selections[i].set_name("selection[ship-entity]");
        m_signed_in_selections[i].set_tags(Object_tags::world_cam);
        m_signed_in_selections[i].set_renderer(mat_renderer);
        m_signed_in_selections[i].set_transform(m_selection_screens[i].get_transform());
        
        ++m_players_signed_in;
      }
    }
    
    if(m_controllers[i].is_button_down_on_frame(Core::Gamepad_button::a))
    {
      // Start screen
      if(!m_continue_button)
      {
        // Continue button
        {
          const Core::Texture hot_texture(Core::Directory::volatile_resource_path("assets/textures/button_continue_hot.png"));
          const Core::Texture cold_texture(Core::Directory::volatile_resource_path("assets/textures/button_continue_cold.png"));
          
          m_continue_button = Core::Lib::Button(get_world(),
                                              get_ctx(),
                                              "continue",
                                              math::vec2_init(get_ctx().get_width() / 2, (get_ctx().get_height() / 6) * 5),
                                              main_camera->m_gui_camera,
                                              hot_texture,
                                              cold_texture);
        }
      }
      
      if(button_hovered && i == 0)
      {
        continue;
      }
    
      m_current_player_selection[i] = (m_current_player_selection[i] + 1) % Selection_screen_utils::get_max_models();
      const uint32_t selection = m_current_player_selection[i];
     
      const Core::Material_renderer player_renderer(m_materials[selection], m_models[selection]);
      m_signed_in_selections[i].set_renderer(player_renderer);
      
      const Core::Material_renderer sel_renderer(
        Factory::Material::get_selection_choose_ship_controller(),
        Core::Model(Core::Directory::volatile_resource_path("assets/models/unit_plane.obj"))
      );
      m_selection_screens[i].set_renderer(sel_renderer);
    }
  }
  
  /*
    Update sel screens
  */
  for(uint32_t i = 0; i < 4; ++i)
  {
    auto &sel = m_selection_screens[i];
    
    const float screen_div = math::to_float(get_ctx().get_width()) / 5.f;
    const float x_offset   = (screen_div + (screen_div * i)) - (get_ctx().get_width() / 2.f);
    const float y_offset   = math::to_float(get_ctx().get_height() >> 1);

    sel.set_transform(Core::Transform(
      math::vec3_init(x_offset, 0, 0),
      math::vec3_init(128.f, 1.f, 128.f),
      math::quat_init_with_axis_angle(Core::Transform_utils::get_world_left(), -math::quart_tau())
    ));

    if(m_signed_in_selections[i])
    {
      const float screen_div = math::to_float(get_ctx().get_width()) / 5.f;
      const float x_offset   = screen_div + (screen_div * i);
      const float y_offset   = math::to_float(get_ctx().get_height() >> 1);

      auto sel_trans = m_selection_screens[i].get_transform();
      sel_trans.set_scale(math::vec3_init(1.f));
      sel_trans.set_position(Screen_cast::intersect_screen_plane(main_camera->m_world_camera, x_offset, y_offset));
      
      m_signed_in_selections[i].set_transform(sel_trans);
    }
  }
  
  /*
    Update the signed in players rotation.
  */
  {
    static float time = 0;
    time += get_world().get_delta_time();
  
    for(uint32_t i = 0; i < 4; ++i)
    {
      Core::Entity *sel = &m_signed_in_selections[i];
    
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
  for(uint32_t i = 0; i < 4; ++i)
  {
    if(m_controllers[i].is_button_up_on_frame(Core::Gamepad_button::back | Core::Gamepad_button::b))
    {
      for(uint32_t i = 0; i < 4; ++i)
      {
        auto &sel = m_signed_in_selections[i];
        
        if(sel)
        {
          sel.destroy();
        }
      }
      
      for(auto &sel : m_selection_screens)
      {
        // Currently the best way to hide an entity is just
        // to create a new one :D
        sel.destroy();
      }
      
      m_continue_button = Core::Lib::Button();
      
      return std::unique_ptr<State>(new Game::Title_screen(get_world_objs(), get_world(), get_ctx()));
    }
  }
  
  return nullptr;
}


} // ns