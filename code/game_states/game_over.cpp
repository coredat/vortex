#include <game_states/game_over.hpp>
#include <game_states/title_screen.hpp>
#include <game_states/selection.hpp>
#include <game_objects/world_objects.hpp>
#include <common/object_tags.hpp>
#include <common/game_state.hpp>
#include <lib/menu/button.hpp>
#include <common/screen_cast.hpp>
#include <game_objects/explosion.hpp>
#include <game_objects/enemy.hpp>
#include <game_objects/player.hpp>
#include <game_objects/main_camera.hpp>
#include <game_objects/player_ui.hpp>
#include <common/event_ids.hpp>
#include <core/context/context.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/renderer.hpp>
#include <core/world/world.hpp>
#include <core/input/controller.hpp>
#include <core/input/buttons.hpp>
#include <core/transform/transform.hpp>
#include <core/resources/shader.hpp>
#include <core/resources/texture.hpp>
#include <core/common/directory.hpp>


namespace
{
  Core::Entity player_entities[4];
  Core::Model plane;
  Core::Material continue_material;
  bool created_screen = false;
  Game_object::Player_ui *ui_refs[4];
  
  Core::Lib::Button continue_button;
}


namespace Game {


Game_over_screen::Game_over_screen(Game_object::World_objects &objs,
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
{
  // We use mouse for screen menu.
  Core::Input::mouse_set_capture(get_ctx(), false);
  assert(m_camera);

  Game_object::Main_camera *main_camera = reinterpret_cast<Game_object::Main_camera*>(m_camera.get_user_data());
  assert(main_camera);
  
  main_camera->set_target_height(50.f);
  main_camera->set_target_speed(2.f);
  
  get_world_objs().send_event(Event_id::destroy_all_enemies);
}


std::unique_ptr<State>
Game_over_screen::on_update()
{
  Game_object::Main_camera *main_camera = reinterpret_cast<Game_object::Main_camera*>(m_camera.get_user_data());
  assert(main_camera);

  if(!created_screen)
  {
    created_screen = true;
    
    Core::Entity_ref *refs_arr;
    size_t refs_count = 0;
    get_world().find_entities_by_name("Player", &refs_arr, &refs_count);
    assert(refs_count == 4);
    
    Game_object::Player *players[4];
    
    for(uint32_t i = 0 ; i < 4; ++i)
    {
      players[i] = reinterpret_cast<Game_object::Player*>(refs_arr[i].get_user_data());
    }
    
    for(uint32_t i = 0; i < 4; ++i)
    {
      if(players[i]->is_valid())
      {
        ui_refs[i] = new Game_object::Player_ui(get_world(), get_ctx(), main_camera->m_gui_camera, main_camera->m_world_camera, i + 1, Game_object::Player_ui::Ui_type::game_over_ui);
        
        get_world_objs().push_object(ui_refs[i]);
        
        assert(ui_refs[i]->m_avatar);
        
        Core::Material_renderer renderer;
        renderer.set_material(players[i]->get_material());
        renderer.set_model(players[i]->get_model());
        
        ui_refs[i]->m_avatar.set_renderer(renderer);
        ui_refs[i]->set_score(players[i]->get_score());
      }
    }
    
    if(!continue_material)
    {
      const char *shader_path = Core::Directory::volatile_resource_path("assets/shaders/basic_fullbright.ogl");
      const Core::Shader shader(shader_path);
    
      plane = Core::Model(Core::Directory::volatile_resource_path("assets/models/unit_plane.obj"));
      
      const char *press_start = Core::Directory::volatile_resource_path("assets/textures/dev_grid_orange_512.png");
      const Core::Texture continue_texture(press_start);
      
      continue_material = Core::Material("screen_game_over[continue]");
      continue_material.set_shader(shader);
      continue_material.set_map_01(continue_texture);
    }
    
    if(!continue_button)
    {
      // Continue button
      {
        const Core::Texture hot_texture(Core::Directory::volatile_resource_path("assets/textures/button_continue_hot.png"));
        const Core::Texture cold_texture(Core::Directory::volatile_resource_path("assets/textures/button_continue_cold.png"));
        
        continue_button = Core::Lib::Button(get_world(),
                                            get_ctx(),
                                            "continue2",
                                            math::vec2_init(get_ctx().get_width() / 2, (get_ctx().get_height() / 6) * 5),
                                            main_camera->m_gui_camera,
                                            hot_texture,
                                            cold_texture);
      }
    }
  }
  
  bool button_pushed = false;
  bool button_hovered = false;
  
  if(continue_button.is_over(main_camera->m_gui_camera, get_world(), get_ctx()))
  {
    button_hovered = true;
    
    if(continue_button.was_touched(main_camera->m_gui_camera, get_world(), get_ctx()))
    {
      button_pushed = true;
    }
  }
  
  /*
    Result screens
  */
  {
    static float time = 0;
    const float dt = get_world().get_delta_time();
    time += dt;

    for(uint32_t i = 0; i < 4; ++i)
    {
      auto &pl = player_entities[i];
      
      if(pl)
      {
        const float quart_screen = math::to_float(get_ctx().get_width() >> 2);
        const float x_offset     = quart_screen + (quart_screen * i);
        const float y_offset     = math::to_float(get_ctx().get_height() >> 3) * 3.f;
        
        const math::vec3 pos = Screen_cast::intersect_screen_plane(main_camera->m_world_camera, x_offset, y_offset);
        Core::Transform trans;
        trans.set_position(pos);
        trans.set_scale(math::vec3_init(1.f));

        const math::quat spin_rot = math::quat_init_with_axis_angle(0, 1, 0, time + i);
        const math::quat tilt_rot = math::quat_init_with_axis_angle(0, 0, 1, -0.2f);
        const math::quat rot = math::quat_multiply(tilt_rot, spin_rot);
        trans.set_rotation(rot);
        
        pl.set_transform(trans);
      }
    }
  }
  
  /*
    If any gamepad presses start we go back to the game selection screen.
  */
  if(m_controllers[0].is_button_down_on_frame(Core::Gamepad_button::button_start) ||
     m_controllers[1].is_button_down_on_frame(Core::Gamepad_button::button_start) ||
     m_controllers[2].is_button_down_on_frame(Core::Gamepad_button::button_start) ||
     m_controllers[3].is_button_down_on_frame(Core::Gamepad_button::button_start) ||
     button_pushed)
  {
    created_screen = false;
    continue_button = Core::Lib::Button();
    
    for(auto &pl : player_entities)
    {
      if(pl)
      {
        pl.destroy();
        pl = Core::Entity();
      }
    }
    
    for(uint32_t i = 0; i < 4; ++i)
    {
      if(ui_refs[i])
      {
        ui_refs[i]->destroy();
      }
    }
    
    return std::unique_ptr<State>(new Game::Selection_screen(get_world_objs(), get_world(), get_ctx()));
  }
  
  if(m_controllers[0].is_button_down_on_frame(Core::Gamepad_button::button_back) ||
     m_controllers[1].is_button_down_on_frame(Core::Gamepad_button::button_back) ||
     m_controllers[2].is_button_down_on_frame(Core::Gamepad_button::button_back) ||
     m_controllers[3].is_button_down_on_frame(Core::Gamepad_button::button_back))
  {
    created_screen = false;
    continue_button = Core::Lib::Button();
    
    for(auto &pl : player_entities)
    {
      if(pl)
      {
        pl.destroy();
        pl = Core::Entity();
      }
    }
    
    for(uint32_t i = 0; i < 4; ++i)
    {
      if(ui_refs[i])
      {
        ui_refs[i]->destroy();
      }
    }
    
    return std::unique_ptr<State>(new Game::Title_screen(get_world_objs(), get_world(), get_ctx()));
  }

  return nullptr;
}


} // ns