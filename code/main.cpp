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

// Game States
#include <game_states/loading.hpp>
#include <game_states/game.hpp>
#include <game_states/selection.hpp>
#include <game_states/game_over.hpp>
#include <game_states/title_screen.hpp>

// Core
#include <core/context/context.hpp>
#include <core/world/world.hpp>
#include <core/world/world_setup.hpp>
#include <core/renderer/material_renderer.hpp>
#include <math/vec/vec3.hpp>
#include <utilities/timer.hpp>
#include <utilities/logging.hpp>
#include <utilities/optimizations.hpp>

// Testing
#include <core/resources/render_target.hpp>
#include <core/resources/material.hpp>
#include <core/resources/shader.hpp>
#include <core/resources/texture.hpp>
#include <core/camera/camera_utils.hpp>
#include <core/transform/transform.hpp>
#include <math/vec/vec4.hpp>
#include <math/general/to_string.hpp>
#include <math/mat/mat4.hpp>
#include <utilities/directory.hpp>
#include <vector>

#include <common/event_ids.hpp>


int
main()
{
  // ** Setup Core ** //
  Core::Context_setup context_setup;
  context_setup.vsync = false;

  Core::Context context(1200, 720, false, "Vortex Defender 2099", context_setup);
  Core::World   world(context, Core::World_setup{});
  
  // ** Start Game ** //
  Game_state curr_state = Game_state::null;
  Game_state next_state = Game_state::null;
  
  Game_object::World_objects objs;
  Game_object::Main_camera *go_cam = new Game_object::Main_camera(world, context);;
  
  objs.push_object(go_cam);
  objs.push_object(new Game_object::Horizon(world));
  
  bool first_load = true;
  
  // Game state
  while(context.is_open())
  {
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
          loading_init(context, world);
          break;
          
        case(Game_state::title_screen):
          title_screen_init(context,
                            world,
                            go_cam->m_world_camera);
          break;

        case(Game_state::selection):
        {
          if(first_load)
          {
            first_load = false;
            
            objs.push_object(new Game_object::Level(world));
          }
        
          selection_init(context, world, go_cam->m_world_camera);
          break;
        }
        
        case(Game_state::game_mode):
          game_init(context, world);
          break;
          
        case(Game_state::game_over):
          game_over_init(context, world);
          objs.send_event(Event_id::destroy_all_enemies);
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
        next_state = loading_update(context,
                                    world,
                                    go_cam->m_world_camera);
        
        break;
      }

      /*
        Title screen
      */
      case(Game_state::title_screen):
      {
        next_state = title_screen_update(context,
                                         world,
                                         go_cam->m_world_camera,
                                         objs,
                                         dt);
        
        break;
      }

      /*
        Player selection screen.
        Displays the screen where players can join a game.
      */
      case(Game_state::selection):
      {
        next_state = selection_update(context,
                                      world,
                                      go_cam->m_world_camera,
                                      objs,
                                      dt);
        break;
      }
      
      /*
        Main game screen.
        This is the active game that is being played.
      */
      case(Game_state::game_mode):
      {
        next_state = game_update(context,
                                 world,
                                 objs,
                                 dt);
        
        
        break;
      }
    
      /*
        The game over screen.
        Shows the winner of the round.
      */
      case(Game_state::game_over):
      {
        next_state = game_over_update(context,
                                      world,
                                      dt);
        break;
      }
      
      default:
        UNREACHABLE;
    }
    
    
    objs.on_destroy();
    
    // Needs to give chance to objects to get into the
    // right position before they are rendererd.
    objs.on_start();
    
    world.think();
  }

  return 0;
}