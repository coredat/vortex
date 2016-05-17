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
#include <core/transform/transform.hpp>


int
main()
{
  // ** Setup Core ** //

  Core::Context_setup context_setup;
  context_setup.vsync = false;

  Core::Context context(1200, 700, false, "Vortex Defender 2099", context_setup);
  
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
  
  selection_init(context, world, cam.camera);
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
                                      cam.camera,
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
    {
//      const math::vec4 ray_clip = math::vec4_init(0,0,-1,1);
//      const auto inv_proj = Core::Camera_utils::camera_get_inverse_projection_matrix(cam.camera);
//      
//      math::vec4 eye = math::mat4_multiply(ray_clip, inv_proj);
//      eye = math::vec4_init(math::vec4_get_x(eye), math::vec4_get_y(eye), -1, 0);
//      
//      math::vec4 pos = math::mat4_multiply(eye, Core::Camera_utils::camera_get_inverse_view_matrix(cam.camera));
//      
//      math::mat4 proj = Core::Camera_utils::camera_get_projection_matrix(cam.camera);
//      math::mat4 view = Core::Camera_utils::camera_get_view_matrix(cam.camera);
//      math::mat4 inv_vp = math::mat4_get_inverse(math::mat4_multiply(proj, view));
//      math::vec4 screen_pos = math::vec4_init(1,1,-1, 1);
//      math::vec4 world_pos = math::mat4_multiply(screen_pos, inv_vp);
//      math::vec3 world_pos3 = math::vec3_init(math::vec3_get_x(world_pos), math::vec3_get_y(world_pos), math::vec3_get_z(world_pos));
//      math::vec3 dir = math::vec3_normalize(world_pos3);
//      math::vec3 distance = math::vec3_scale(dir, 10);
//      math::vec3 ray = math::vec3_add(cam.entity.get_transform().get_position(), distance);
//      
//      auto intersect_plane = [](const math::vec3 n, const math::vec3 p0, const math::vec3 l0, const math::vec3 l, float &t) -> bool
//      {
//          // assuming vectors are all normalized
//          float denom = math::vec3_dot(n, l);
//          if (denom > 1e-6)
//          {
//              math::vec3 p0l0 = math::vec3_subtract(p0, l0);
//              t = math::vec3_dot(p0l0, n) / denom;
//              return (t >= 0); 
//          } 
//       
//          return false; 
//      };
//      
//      float time;
//      const bool did_intersect = intersect_plane(math::vec3_init(0, 0, -1), math::vec3_zero(), cam.entity.get_transform().get_position(), dir, time);
//      
//      std::cout << time << " - " << did_intersect << std::endl;
    }
    
//   float mouseX = getMousePositionX() / (getWindowWidth()  * 0.5f) - 1.0f;
//    float mouseY = getMousePositionY() / (getWindowHeight() * 0.5f) - 1.0f;

//    glm::mat4 invVP = glm::inverse(proj * view);
//    glm::vec4 screenPos = glm::vec4(mouseX, -mouseY, 1.0f, 1.0f);
//    glm::vec4 worldPos = invVP * screenPos;
//
//    glm::vec3 dir = glm::normalize(glm::vec3(worldPos));
    
    
//        std::cout << math::to_string(dir) << std::endl;
//    std::cout << math::to_string(cam.entity.get_transform().get_position()) << std::endl;
//    std::cout << "--" << std::endl;

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