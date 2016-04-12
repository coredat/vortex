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
#include <game_objects/bullet.hpp>
#include <game_objects/player.hpp>
#include <game_objects/enemy.hpp>
#include <game_objects/main_camera.hpp>
#include <game_objects/level.hpp>
#include <game_objects/explosion.hpp>
#include <utilities/timer.hpp>
#include <common/object_tags.hpp>


int
main()
{
  Core::Context context(800, 480, false, "Vortex");
  
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
  
  Bullet bullets[128];
  Bullet_utils::init_bullets(world, bullets, 128);
  
  Player players[1];
  Player_utils::init_players(world, players, 1);
  
  Enemy enemies[128];
  Enemy_utils::init_enemies(world, enemies, 128);
  
  Explosion explosions[128];
  Explosion_utils::init_explosions(world, explosions, 128);
  
  Level_data level[1];
  Level_utils::init_level(world, level, 1);
  
  while(context.is_open())
  {
    const util::milliseconds frame_time = delta_time_ms.split();
    const float dt = static_cast<float>(frame_time) / 1000.f;

    world.think(dt);
    
    world.get_overlapping_aabbs([&](const Core::Entity_ref ref_a,
                                    const Core::Entity_ref ref_b)
    {
      // Enemy collided with a bullet
      if(ref_a.has_tag(Object_tags::enemy) && ref_b.has_tag(Object_tags::bullet))
      {
        Enemy_utils::hit_enemy(world,
                               ref_a.get_id(),
                               enemies,
                               128,
                               explosions,
                               128);
      }
    });

    Camera_utils::move_main_camera(cam, dt, players, 1);
    Player_utils::move_players(context, world, dt, players, 1, bullets, 128);
    Enemy_utils::update_enemies(world, dt, enemies, 128);
    Bullet_utils::move_bullets(world, dt, bullets, 16);
    Explosion_utils::update_explosions(world, dt, explosions, 128);

    mesh_renderer.render();
  }

  return 0;
}