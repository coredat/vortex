// Game Objects
#include <game_objects/game_object.hpp>
#include <game_objects/bullet.hpp>
#include <game_objects/player.hpp>
#include <game_objects/enemy.hpp>
#include <game_objects/main_camera.hpp>
#include <game_objects/level.hpp>
#include <game_objects/explosion.hpp>
#include <game_objects/powerup_pickup.hpp>
#include <game_objects/world_objects.hpp>

// Game States
#include <game_states/game.hpp>
#include <game_states/selection.hpp>
#include <game_states/game_over.hpp>

// Core
#include <core/context/context.hpp>
#include <core/world/world.hpp>
#include <core/world/world_setup.hpp>
#include <core/renderer/material_renderer.hpp>
#include <math/vec/vec3.hpp>
#include <utilities/timer.hpp>
#include <utilities/logging.hpp>

// Testing
#include <core/resources/material.hpp>
#include <core/resources/shader.hpp>
#include <utilities/directory.hpp>
#include <math/vec/vec4.hpp>
#include <math/general/to_string.hpp>
#include <math/mat/mat4.hpp>
#include <iostream>
#include <core/camera/camera_utils.hpp>
#include <core/transform/transform.hpp>
#include <vector>


int
main()
{
  // ** Setup Core ** //
  Core::Context_setup context_setup;
  context_setup.vsync = false;

  Core::Context context(800, 480, false, "Vortex Defender 2099", context_setup);
  
  Core::World world(Core::World_setup{});
  
  LOG_TODO("Move delta time into core.");
  
  util::timer delta_time_ms;
  delta_time_ms.start();
  
  // ** Game Objects ** //
  
//  Enemies_container enemies_container;
//  Enemy_utils::init_enemies(world, enemies_container);
  
//  Explosions_container explosions_container;
//  Explosion_utils::init_explosions(world, explosions_container);
  
  Powerups_container powerups_container;
  Powerup_utils::init_powerups(world, powerups_container);
  
  // ** Start Game ** //
  
  Game_state game_state = Game_state::selection;
  
  Game_object::World_objects objs;
  
  Game_object::Main_camera *go_cam = new Game_object::Main_camera(world, context);
  
  objs.push_object(go_cam);
  objs.push_object(new Game_object::Level(world));
  
  selection_init(context, world, go_cam->m_camera);
  game_init(context, world);
  game_over_init(context, world);
  
  while(context.is_open())
  {
    const util::milliseconds frame_time = delta_time_ms.split();
    const float dt = static_cast<float>(frame_time) / 1000.f;
    
    objs.on_start();
    objs.on_update(dt);
    
    /*
      Common Entities to update
    */
    {
//      Explosion_utils::update_explosions(world, dt, explosions_container);
//      Enemy_utils::update_enemies(world, dt, enemies_container);
      Powerup_utils::update_powerups(world, dt, powerups_container);
    }
    
    // ** Game State ** //
    switch(game_state)
    {
      /*
        Player selection screen.
        Displays the screen where players can join a game.
      */
      case(Game_state::selection):
      {
        game_state = selection_update(context,
                                      world,
                                      go_cam->m_camera,
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
        game_state = game_update(context,
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
        game_state = game_over_update(context,
                                      world,
//                                      explosions_container,
                                      dt);
        break;
      }
    }
    
    objs.on_destroy();
    world.think(dt);
  }

  return 0;
}