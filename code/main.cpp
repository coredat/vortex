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
#include <data/resource_data/resource_data.hpp>
#include <utilities/logging.hpp>
#include <utilities/timer.hpp>

// Common
#include <common/object_tags.hpp>

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


enum class Game_state
{
  selection,
  game_mode,
  game_over,
};


Game_state game_state = Game_state::selection;


int
main()
{
  Core::Context context(800, 480, false, "Vortex");
  Core::Input::mouse_set_capture(context, true);
  
  Core::Model model("/Users/PhilCK/Developer/core/assets/models/unit_cube.obj");
  assert(model.get_id());
  
  Core::Texture texture("/Users/PhilCK/Developer/core/assets/textures/dev_grid_green_512.png");
  assert(texture.get_id());
  
  Core::World world(Core::World_setup{});
  Core::Mesh_renderer mesh_renderer;
  
  util::timer delta_time_ms;
  delta_time_ms.start();
  
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
  
  while(context.is_open())
  {
    const util::milliseconds frame_time = delta_time_ms.split();
    const float dt = static_cast<float>(frame_time) / 1000.f;

    world.think(dt);

    /*
      Selection.
    */
    if(game_state == Game_state::selection)
    {
      // Wait for input.
      Core::Input::Controller controller(context, 0);
      if(controller.is_button_down(Core::Input::Button::button_0))
      {
        game_state = Game_state::game_mode;
        Player_utils::init_players(world, players_container);
      }
    }
    
    /*
      Play game.
    */
    if(game_state == Game_state::game_mode)
      {
      
      world.get_overlapping_aabbs([&](const Core::Entity_ref ref_a,
                                      const Core::Entity_ref ref_b)
      {
        // Enemy collided with a bullet
        if(ref_a.has_tag(Object_tags::enemy) && ref_b.has_tag(Object_tags::bullet))
        {
          Enemy_utils::hit_enemy(world,
                                 ref_a.get_id(),
                                 enemies_container,
                                 explosions_container,
                                 powerups_container);
        }
        
        if(ref_a.has_tag(Object_tags::player))
        {
          if(ref_b.has_tag(Object_tags::enemy))
          {
            Player_utils::hit_player(world,
                                     ref_a.get_id(),
                                     players_container,
                                     explosions_container);
          }
        }
      });
        
      Enemy_utils::spawn_enemies(world, dt, enemies_container);
      Player_utils::move_players(context, world, dt, players_container, bullets_container);

      
      if(Player_utils::all_dead(players_container))
      {
        game_state = Game_state::game_over;
      }
    }
    
    /*
      Game over screen
    */
    if(game_state == Game_state::game_over)
    {
      Enemy_utils::explode_all(world, enemies_container, explosions_container);
      
      Core::Input::Controller controller(context, 0);
      if(controller.is_button_down(Core::Input::Button::button_2))
      {
        game_state = Game_state::selection;
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