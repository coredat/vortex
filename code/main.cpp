
// Game Objects
#include <game_objects/game_object.hpp>
#include <game_objects/bullet.hpp>
#include <game_objects/player_ship.hpp>
#include <game_objects/enemy.hpp>
#include <game_objects/main_camera.hpp>
#include <game_objects/level.hpp>
#include <game_objects/horizon.hpp>
#include <game_objects/explosion.hpp>
#include <game_objects/powerup_pickup.hpp>
#include <game_objects/world_objects.hpp>
#include <game_objects/player.hpp>

// Game States
#include <game_states/loading.hpp>
#include <game_states/game.hpp>
#include <game_states/selection.hpp>
#include <game_states/game_over.hpp>
#include <game_states/title_screen.hpp>
#include <game_states/about.hpp>
#include <game_states/settings.hpp>

// Common
#include <common/event_ids.hpp>
#include <common/global_vars.hpp>
#include <common/settings.hpp>

// Core
#include <core/context/context.hpp>
#include <core/world/world.hpp>
#include <core/world/world_setup.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/transform/transform.hpp>
#include <core/input/input.hpp>
#include <core/input/controller.hpp>
#include <core/input/axis.hpp>

#include <math/vec/vec3.hpp>
#include <utilities/timer.hpp>
#include <utilities/logging.hpp>
#include <utilities/optimizations.hpp>
#include <3rdparty/imgui/imgui.h>

// Testing
#include <core/renderer/text_renderer.hpp>
#include <core/renderer/renderer.hpp>
#include <core/font/font.hpp>
#include <common/object_tags.hpp>


std::unique_ptr<Game::Title_screen>       title_state = nullptr;
std::unique_ptr<Game::Settings_screen>    settings_screen = nullptr;
std::unique_ptr<Game::Selection_screen>   selection_screen = nullptr;
std::unique_ptr<Game::Loading_screen>     loading_screen = nullptr;
std::unique_ptr<Game::Game_screen>        game_screen = nullptr;
std::unique_ptr<Game::Game_over_screen>   game_over_screen = nullptr;


int
main()
{
  #ifndef NDEBUG
  util::logging::set_output(
    util::logging::out::file | util::logging::out::console
  );
  #else
  util::logging::set_output(util::logging::out::file);
  #endif

  Common::Settings saved_settings = Common::get_settings();

  // ** Setup Core ** //
  Core::Context_setup context_setup;
  context_setup.vsync = true;
  context_setup.high_dpi_support = false;
  context_setup.monitor_preference = saved_settings.monitor;

  Core::Context context(saved_settings.width, saved_settings.height, saved_settings.is_fullscreen, "Vortex Defender", context_setup);
  Core::World   world(context, Core::World_setup{});
  
  // ** Start Game ** //
  Game_state curr_state = Game_state::null;
  Game_state next_state = Game_state::null;
  
  Game_object::World_objects objs;
  Game_object::Main_camera *go_cam = new Game_object::Main_camera(world, context);;
  go_cam->set_target_height(100.f);
  
  constexpr uint32_t player_count = 4;
  
  Game_object::Player *players[player_count];
  
  for(auto &pl : players)
  {
    pl = new Game_object::Player(world);
    objs.push_object(pl);
  }
  
  objs.push_object(go_cam);
  
  bool first_load_selection = true;
  bool first_load_title = true;
  bool first_load_level = true;
  
  // Game state
  while(context.is_open() && curr_state != Game_state::quit)
  {  
    #ifdef CORE_DEBUG_MENU
    if (ImGui::BeginMainMenuBar())
    {
      if (ImGui::BeginMenu("Game Settings"))
      {
        ImGui::MenuItem("God mode", nullptr, &Global::g_god_mode);
        ImGui::MenuItem("Auto mode", nullptr, &Global::g_auto_play);
        ImGui::EndMenu();
      }
      
      ImGui::EndMainMenuBar();
    }
    #endif
  
    const float dt = world.get_delta_time();
    
    objs.on_update(dt);
    
    // ** Game State ** //
    if(curr_state != next_state)
    {
      switch(next_state)
      {
        case(Game_state::null):
          break;
      
        case(Game_state::loading):
          loading_screen.reset(new Game::Loading_screen(objs, world, context));
          break;
          
        case(Game_state::title_screen):
        {
          if(first_load_title)
          {
            first_load_title = false;
            objs.push_object(new Game_object::Horizon(world));
          }
          
          title_state.reset(new Game::Title_screen(objs, world, context));
          
          break;
        }
        
        case(Game_state::settings):
        {
          settings_screen.reset(new Game::Settings_screen(objs, world, context));
          break;
        }
        
        case(Game_state::about):
        {
          about_init(context,
                     world,
                     go_cam->m_gui_camera);
          break;
        }

        case(Game_state::selection):
        {
          if(first_load_selection)
          {
            first_load_selection = false;

          }
        
          selection_screen.reset(new Game::Selection_screen(objs, world, context));
          break;
        }
        
        case(Game_state::game_mode):
          Core::Input::mouse_set_capture(context, true);
          
          go_cam->set_target_height(20.f);
          go_cam->set_target_speed(10.f);
          
          if(first_load_level)
          {
            first_load_level = false;
            
            objs.push_object(new Game_object::Level(world));
          }
          
          game_screen.reset(new Game::Game_screen(objs, world, context));
          break;
          
        case(Game_state::game_over):
          Core::Input::mouse_set_capture(context, false);
          
          go_cam->set_target_height(50.f);
          go_cam->set_target_speed(2.f);
        
          game_over_screen.reset(new Game::Game_over_screen(objs, world, context));
          objs.send_event(Event_id::destroy_all_enemies);
          break;
          
        case(Game_state::quit):
          break;
          
        default:
          UNREACHABLE;
      }
      
      curr_state = next_state;
    }
    
    switch(curr_state)
    {
      /*
        Loading
      */
      case(Game_state::null):
        next_state = Game_state::loading;
        break;
      
      case(Game_state::loading):
      {
        next_state = loading_screen->on_update();
        
        break;
      }

      /*
        Title screen
      */
      case(Game_state::title_screen):
      {
        next_state = title_state->on_update();
        
        break;
      }
      
      /*
        Settings
      */
      case(Game_state::about):
      {
        next_state = about_update(context,
                                  world,
                                  go_cam->m_gui_camera,
                                  objs);
      
        break;
      }
      
      /*
        About
      */
      case(Game_state::settings):
      {
        next_state = settings_screen->on_update();
      
        break;
      }

      /*
        Player selection screen.
        Displays the screen where players can join a game.
      */
      case(Game_state::selection):
      {
        next_state = selection_screen->on_update();
        
        if(next_state == Game_state::game_mode)
        {
//          go_cam->get_entity().set_transform(Core::Transform(
//            math::vec3_init(0, 0, 500.f),
//            math::vec3_one(),
//            math::quat_init()
//          ));
        }
        break;
      }
      
      /*
        Main game screen.
        This is the active game that is being played.
      */
      case(Game_state::game_mode):
      {
        next_state = game_screen->on_update();
        
        break;
      }
    
      /*
        The game over screen.
        Shows the winner of the round.
      */
      case(Game_state::game_over):
      {
        next_state = game_over_screen->on_update();
        break;
      }
      
                
      case(Game_state::quit):
        break;
      
      default:
        UNREACHABLE;
    }
    
    
    // Needs to give chance to objects to get into the
    // right position before they are rendererd.
    if(curr_state != Game_state::quit)
    {
      objs.on_start();
      world.think();
      objs.on_destroy();
    }
  }
  
  saved_settings.width = context.get_width();
  saved_settings.height = context.get_height();
  saved_settings.is_fullscreen = context.is_fullscreen() ? 1 : 0;
  saved_settings.monitor = context.get_display();
  
  context.close();
  
  // Write out display settings.
  Common::write_settings(saved_settings);

  return 0;
}