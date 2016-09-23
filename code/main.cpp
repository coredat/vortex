
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
#include <game_states/loading_screen.hpp>
#include <game_states/game_screen.hpp>
#include <game_states/selection_screen.hpp>
#include <game_states/game_over_screen.hpp>
#include <game_states/title_screen.hpp>
#include <game_states/about_screen.hpp>
#include <game_states/settings_screen.hpp>

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



std::unique_ptr<Game::State> curr_state(nullptr);
std::unique_ptr<Game::State> next_state(nullptr);


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
  
  
  curr_state.reset(new Game::Loading_screen(objs, world, context));

  // Game state
  while(context.is_open())// && curr_state != Game_state::quit)
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
  
    objs.on_update(world.get_delta_time());
    
    next_state = curr_state->on_update();
    
    // Run state
    if(next_state)
    {
      curr_state.release();
      curr_state = std::move(next_state);
    }
    
    // Quit?
    if(strcmp(curr_state->get_name(), "Quit") == 0)
    {
      break;
    }
    
    // Needs to give chance to objects to get into the
    // right position before they are rendererd.
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