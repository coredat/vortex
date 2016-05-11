#include <core/entity/entity_ref.hpp>
#include <core/context/context.hpp>
#include <core/world/world.hpp>
#include <core/world/world_setup.hpp>
#include <core/camera/camera.hpp>
#include <core/camera/camera_properties.hpp>
#include <core/entity/entity.hpp>
#include <core/color/color.hpp>
#include <core/color/color_predefined.hpp>
#include <core/renderer/mesh_renderer.hpp>
#include <core/transform/transform.hpp>
#include <core/input/input.hpp>
#include <core/input/controller.hpp>
#include <math/vec/vec3.hpp>

// Temp Resource loading.
#include <core/model/model.hpp>
#include <core/model/mesh.hpp>
#include <core/material/texture.hpp>
#include <utilities/logging.hpp>
#include <utilities/timer.hpp>

// Common
#include <common/object_tags.hpp>
#include <common/game_state.hpp>

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

#include <utilities/bits.hpp>

#include <iostream>





Game_state game_state = Game_state::selection;


int
main()
{
  Core::Context_setup context_setup;
  context_setup.vsync = false;

  Core::Context context(800, 480, false, "Vortex", context_setup);
//  Core::Input::mouse_set_capture(context, true);
  
  Core::World world(Core::World_setup{});
  Core::Mesh_renderer mesh_renderer;
  
  util::timer delta_time_ms;
  delta_time_ms.start();
  
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
  
  selection_init(context, world);
  game_init(context, world);
  game_over_init(context, world);
  
  while(context.is_open())
  {
    const util::milliseconds frame_time = delta_time_ms.split();
    const float dt = static_cast<float>(frame_time) / 1000.f;

    world.think(dt);
    
    // ** Game State ** //
    switch(game_state)
    {
      case(Game_state::selection):
      {
        game_state = selection_update(context, world, players_container, dt);
        break;
      }
    
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
    
      case(Game_state::game_over):
      {
        game_state = game_over_update(context, world, enemies_container, explosions_container, dt);
        break;
      }
    }
    
    /*
      Common Entities to update
    */
    {
      Camera_utils::move_main_camera(cam, dt, players_container);
      Bullet_utils::move_bullets(world, dt, bullets_container);
      Explosion_utils::update_explosions(world, dt, explosions_container);
      Enemy_utils::update_enemies(world, dt, enemies_container);
      Powerup_utils::update_powerups(world, powerups_container, dt);
    }

    mesh_renderer.render();
  }

  return 0;
}