// Game Objects
#include <game_objects/bullet.hpp>
#include <game_objects/player.hpp>
#include <game_objects/enemy.hpp>
#include <game_objects/main_camera.hpp>
#include <game_objects/level.hpp>
#include <game_objects/explosion.hpp>
#include <game_objects/powerup_pickup.hpp>

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
  
  
  // Test
  {
    const std::string file = util::get_resource_path() + "assets/shaders/basic_fullbright.ogl";
    Core::Shader shader(file.c_str());
    
    Core::Material mat("foo-fee");
    
    assert(shader.is_valid());
  }
  
  
  // ** Game Objects ** //
  
  Game_camera cam;
  Camera_utils::init_main_camera(context, world, cam);
  
  Bullets_container bullets_container;
  Bullet_utils::init_bullets(world, bullets_container);
  
  Players_container players_container;
  //Player_utils::init_players(world, players_container);
  
  Enemies_container enemies_container;
  Enemy_utils::init_enemies(world, enemies_container);
    
  Explosions_container explosions_container;
  Explosion_utils::init_explosions(world, explosions_container);
  
  Level_container level;
  Level_utils::init_level(world, level);
  
  Powerups_container powerups_container;
  Powerup_utils::init_powerups(world, powerups_container);
  
  // ** Start Game ** //
  
  Game_state game_state = Game_state::selection;
  
  selection_init(context, world);
  game_init(context, world);
  game_over_init(context, world);
  
  while(context.is_open())
  {
    const util::milliseconds frame_time = delta_time_ms.split();
    const float dt = static_cast<float>(frame_time) / 1000.f;
    
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
                                      players_container,
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
                                 players_container,
                                 enemies_container,
                                 explosions_container,
                                 powerups_container,
                                 bullets_container,
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
                                      enemies_container,
                                      explosions_container,
                                      dt);
        break;
      }
    }
    
    // Test
    const math::vec4 ray_clip = math::vec4_init(0,0,-1,1);
    const auto inv_proj = Core::Camera_utils::camera_get_inverse_projection_matrix(cam.camera);
    
    math::vec4 eye = math::mat4_multiply(ray_clip, inv_proj);
    eye = math::vec4_init(math::vec4_get_x(eye), math::vec4_get_y(eye), -1, 0);
    
    math::vec4 pos = math::mat4_multiply(eye, Core::Camera_utils::camera_get_inverse_view_matrix(cam.camera));
    
    std::cout << math::to_string(pos) << std::endl;
    
    //
    
    /*
      Common Entities to update
    */
    {
      Camera_utils::move_main_camera(cam, dt, players_container);
      Bullet_utils::move_bullets(world, dt, bullets_container);
      Explosion_utils::update_explosions(world, dt, explosions_container);
      Enemy_utils::update_enemies(world, dt, enemies_container);
      Powerup_utils::update_powerups(world, dt, powerups_container);
    }

    world.think(dt);
  }

  return 0;
}