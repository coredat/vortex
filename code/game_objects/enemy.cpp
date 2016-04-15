#include <game_objects/enemy.hpp>
#include <game_objects/explosion.hpp>
#include <game_objects/powerup_pickup.hpp>
#include <core/model/model.hpp>
#include <core/material/texture.hpp>
#include <core/world/world.hpp>
#include <core/transform/transform.hpp>
#include <math/vec/vec2.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <common/level_functions.hpp>
#include <common/object_tags.hpp>


namespace
{
  constexpr float spawn_rate = 0.2f;
  float spawn_timer = 0.f;
  
  Core::Model   model;
  Core::Texture texture_orange;
  Core::Texture texture_magenta;
}


namespace Enemy_utils {


void
init_enemies(Core::World &world,
             Enemies_container &enemies_container)
{
  model           = Core::Model("/Users/PhilCK/Developer/core/assets/models/unit_cube.obj");
  texture_orange  = Core::Texture("/Users/PhilCK/Developer/core/assets/textures/dev_grid_orange_512.png");
  texture_magenta = Core::Texture("/Users/PhilCK/Developer/core/assets/textures/dev_grid_magenta_512.png");
  
  for(uint32_t i = 0; i < enemies_container.size; ++i)
  {
    // Reset values.
    enemies_container.enemy[i] = Enemies_container::Enemy();
  }
}


void
spawn_enemies(Core::World &world,
              const float dt,
              Enemies_container &enemies_container)
{
  spawn_timer += dt;

  if(spawn_timer > spawn_rate)
  {
    spawn_timer = -110000000.f;
    
    const float point = static_cast<float>(rand() % 1000) / 10.f;
    const float depth = Level::get_bottom_of_level();
    
    using Enemy_type = Enemies_container::Enemy::Type;
    Enemy_type what_to_spawn = (Enemy_type)(rand() % (uint32_t)Enemy_type::size);
    
    switch(what_to_spawn)
    {
      case(Enemy_type::climber):
        //spawn_climber(world, enemies_container, point, 1, depth);
        break;
        
      case(Enemy_type::breeder):
        spawn_breeder(world, enemies_container, point, 1, depth);
        break;
        
      default:
        assert(false);
    }
  }
}



void
spawn_climber(Core::World &world,
              Enemies_container &enemies_container,
              float point_on_circle,
              float direction,
              float depth)
{
  // Find space for new entity.
  for(uint32_t i = 0; i < enemies_container.size; ++i)
  {
    auto &enemy = enemies_container.enemy[i];
  
    if(!enemy.entity)
    {
      enemy = Enemies_container::Enemy();
    
      enemy.point_on_circle = static_cast<float>(rand() % 1000) / 10;
      enemy.depth = depth;
      enemy.type = Enemies_container::Enemy::Type::climber;
      enemy.direction = direction;
      enemy.lifetime = 0;

      enemy.entity = Core::Entity(world);
      enemy.entity.set_name("Enemy");
      enemy.entity.add_tag(Object_tags::enemy);
      enemy.entity.set_model(model);
      enemy.entity.set_material_id(texture_orange.get_id());

      const math::vec2 point = Level::get_point_on_cirlce(enemy.point_on_circle);

      const Core::Transform trans(
        math::vec3_init(math::vec2_get_x(point), math::vec2_get_y(point), Level::get_bottom_of_level()),
        math::vec3_one(),
        math::quat_init()
      );

      enemy.entity.set_transform(trans);
      
      return; // We don't need to keep looking.
    }
  }
}

// --

void
spawn_breeder(Core::World &world,
              Enemies_container &enemies_container,
              float point_on_circle,
              float direction,
              float depth)
{
  // Find space for new entity.
  for(uint32_t i = 0; i < enemies_container.size; ++i)
  {
    auto &enemy = enemies_container.enemy[i];
  
    if(!enemy.entity)
    {
      enemy = Enemies_container::Enemy();
    
      enemy.point_on_circle = point_on_circle;
      enemy.depth = depth;
      enemy.type = Enemies_container::Enemy::Type::breeder;
      enemy.direction = direction;
      enemy.lifetime = 0;

      enemy.entity = Core::Entity(world);
      enemy.entity.set_name("Enemy");
      enemy.entity.add_tag(Object_tags::enemy);
      enemy.entity.set_model(model);
      enemy.entity.set_material_id(texture_magenta.get_id());

      const math::vec2 point = Level::get_point_on_cirlce(enemy.point_on_circle);

      const Core::Transform trans(
        math::vec3_init(math::vec2_get_x(point), math::vec2_get_y(point), Level::get_bottom_of_level()),
        math::vec3_one(),
        math::quat_init()
      );

      enemy.entity.set_transform(trans);
      
      return; // We don't need to keep looking.
    }
  }
}

// --


namespace
{
  /*
    Climber will crawel up and down the tube.
  */
  void
  update_climber(Enemies_container::Enemy &enemy, const float dt)
  {
    Core::Transform trans = enemy.entity.get_transform();
    
    // Point on circle
    {
      math::vec2 new_point = Level::get_point_on_cirlce(enemy.point_on_circle);
      
      const math::vec3 position = trans.get_position();
      
      math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                           math::vec2_get_y(new_point),
                                           math::vec3_get_z(position));
      trans.set_position(new_pos);
    }
    
    // Depth
    {
      enemy.depth += (20.f * dt * static_cast<float>(enemy.direction));
      
      if(!math::is_between(enemy.depth, Level::get_bottom_of_level(), Level::get_top_of_level()))
      {
        enemy.depth = math::clamp(enemy.depth, Level::get_bottom_of_level(), Level::get_top_of_level());
        enemy.direction *= -1;
      }
      
      const math::vec3 pos = trans.get_position();
      const math::vec3 new_pos = math::vec3_init(math::vec3_get_x(pos),
                                                 math::vec3_get_y(pos),
                                                 enemy.depth);
      
      trans.set_position(new_pos);
    }
    
    enemy.entity.set_transform(trans);

  }
  
  
  /*
    Breaders climb to the top then start to bread.
  */
  void
  update_breeder(Core::World &world,
                 Enemies_container &enemies_container,
                 Enemies_container::Enemy &enemy,
                 const float dt)
  {
    Core::Transform trans = enemy.entity.get_transform();
    
    // Point on circle
    {
      math::vec2 new_point = Level::get_point_on_cirlce(enemy.point_on_circle);
      
      const math::vec3 position = trans.get_position();
      
      math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                           math::vec2_get_y(new_point),
                                           math::vec3_get_z(position));
      trans.set_position(new_pos);
    }
    
    // Should breed
    {
      if(enemy.direction == 0 && enemy.lifetime > 0.4f)
      {
        enemy.lifetime = 0.f; // Need to fix this.
        
        const float new_point = enemy.point_on_circle + ((static_cast<float>(rand() % 100) / 250.f) - 0.2f);
        const float new_depth = math::min(enemy.depth + ((static_cast<float>(rand() % 100) / 50.f) - 0.75f), Level::get_top_of_level());
      
        Enemy_utils::spawn_breeder(world, enemies_container, new_point, 0, new_depth);
      }
    }
    
    // Depth
    if(enemy.direction != 0);
    {
      enemy.depth += (20.f * dt * static_cast<float>(enemy.direction));
      
      if(enemy.depth > Level::get_top_of_level())
      {
        enemy.direction = 0;
//        enemy.lifetime = 0;
      }
      
      const math::vec3 pos = trans.get_position();
      const math::vec3 new_pos = math::vec3_init(math::vec3_get_x(pos),
                                                 math::vec3_get_y(pos),
                                                 enemy.depth);
      
      trans.set_position(new_pos);
    }
    
    enemy.entity.set_transform(trans);
  }
}


void
update_enemies(Core::World &world,
               const float dt,
               Enemies_container &enemies_container)
{
  for(uint32_t i = 0; i < enemies_container.size; ++i)
  {
    auto &enemy = enemies_container.enemy[i];
    
    if(!enemy.entity)
    {
      continue;
    }
    
    enemy.lifetime += dt;
    
    using Enemy_type = Enemies_container::Enemy::Type;
    
    switch(enemy.type)
    {
      case(Enemy_type::breeder):
        update_breeder(world, enemies_container, enemy, dt);
        break;
        
      case(Enemy_type::climber):
        update_climber(enemy, dt);
        break;
        
      default:
        assert(false);
    };
  }
}


void
hit_enemy(Core::World &world,
          const Core::Entity_id id,
          Enemies_container &enemies_container,
          Explosions_container &explosions_container,
          Powerups_container &powerups_container)
{
  for(uint32_t i = 0; i < enemies_container.size; ++i)
  {
    auto &enemy = enemies_container.enemy[i];
    
    if(enemy.entity.get_id() == id)
    {
      const math::vec3 position = enemy.entity.get_transform().get_position();
    
      Explosion_utils::create_explosion(world,
                                        position,
                                        explosions_container);
      
      Powerup_utils::create_powerup(world,
                                    position,
                                    powerups_container);
      
      enemy.entity.destroy();
      enemy = Enemies_container::Enemy();
    }
  }
}


void
explode_all(Core::World &world,
            Enemies_container &enemies_container,
            Explosions_container &explosions_container)
{
  for(uint32_t i = 0; i < enemies_container.size; ++i)
  {
    auto &enemy = enemies_container.enemy[i];
    
    if(enemy.entity)
    {
      const math::vec3 pos = enemy.entity.get_transform().get_position();
      Explosion_utils::create_explosion(world, pos, explosions_container);
      
      enemy.entity.destroy();
    }
  }
}


} // ns