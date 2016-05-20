#include <game_objects/enemy.hpp>
#include <game_objects/explosion.hpp>
#include <game_objects/powerup_pickup.hpp>
#include <common/level_functions.hpp>
#include <common/object_tags.hpp>
#include <core/model/model.hpp>
#include <core/transform/transform.hpp>
#include <core/resources/texture.hpp>
#include <core/world/world.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/box_collider.hpp>
#include <core/physics/rigidbody_properties.hpp>
#include <math/vec/vec2.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <utilities/directory.hpp>


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
  const std::string unit_cube_path = util::get_resource_path() + "assets/models/unit_cube.obj";
  model = Core::Model(unit_cube_path.c_str());

  const std::string orange_texture_path = util::get_resource_path() + "assets/textures/dev_grid_orange_512.png";
  texture_orange  = Core::Texture(orange_texture_path.c_str());

  const std::string magenta_texture_path = util::get_resource_path() + "assets/textures/dev_grid_magenta_512.png";
  texture_magenta = Core::Texture(magenta_texture_path.c_str());
  
  for(uint32_t i = 0; i < enemies_container.size; ++i)
  {
    // Reset values.
    enemies_container.enemy[i] = Enemies_container::Enemy();
  }
}

// --

void
spawn_enemies(Core::World &world,
              const float dt,
              Enemies_container &enemies_container)
{
  spawn_timer += dt;

  if(spawn_timer > spawn_rate)
  {
    spawn_timer = 0;
    
    const float point = static_cast<float>(rand() % 1000) / 10.f;
    const float depth = Level_funcs::get_bottom_of_level();
    
    using Enemy_type = Enemies_container::Enemy::Type;
    Enemy_type what_to_spawn = (Enemy_type)(rand() % (uint32_t)Enemy_type::size);
    
    switch(what_to_spawn)
    {
      case(Enemy_type::climber):
        spawn_climber(world, enemies_container, point, 1, depth);
        break;
        
      case(Enemy_type::breeder):
        spawn_breeder(world, enemies_container, point, 1, depth);

        break;
        
      case(Enemy_type::egg):
        // Do nohting
        break;
      
      default:
        assert(false);
    }
  }
}

// --

namespace
{
  /*
    Helper to setup entities as most entity settings are the same.
    the caller can then customize.
  */
  inline void
  common_spawn_setup(Core::Entity &entity, const float depth, const float angle)
  {
    // General
    {
      entity.set_name("Enemy");
      entity.add_tag(Object_tags::enemy);
    }

    // Model and Texture
    {
      entity.set_model(model);
      entity.set_material_id(texture_orange.get_id());
    }

    // Set Transform
    {        
      const math::vec2 point = Level_funcs::get_point_on_cirlce(angle);

      const Core::Transform trans(
        math::vec3_init(math::vec2_get_x(point), math::vec2_get_y(point), depth),
        math::vec3_one(),
        math::quat_init()
      );

      entity.set_transform(trans);
    }

    // Physics
    {
      Core::Box_collider collider(0.5f, 0.5f, 0.5f);
      entity.set_collider(collider);

      Core::Rigidbody_properties rb_props;
      rb_props.set_collision_mask(Object_tags::enemy, Object_tags::bullet | Object_tags::player);
      entity.set_rigidbody_properties(rb_props);
    }
  }
}

// --

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
      enemy.type = Enemies_container::Enemy::Type::climber;
      enemy.direction = direction;
      enemy.lifetime = 0;
      enemy.point_on_circle = static_cast<float>(rand() % 1000) / 10;
      enemy.depth = depth;

      enemy.entity = Core::Entity(world);
      common_spawn_setup(enemy.entity, enemy.depth, enemy.point_on_circle);
            
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

      common_spawn_setup(enemy.entity, enemy.depth, enemy.point_on_circle);

      enemy.entity.set_material_id(texture_magenta.get_id());
      
      return; // We don't need to keep looking.
    }
  }
}

// --

void
spawn_egg(Core::World &world,
          Enemies_container &enemies_container,
          float point_on_circle,
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
      enemy.type = Enemies_container::Enemy::Type::egg;
      enemy.direction = 0;
      enemy.lifetime = 0;

      enemy.entity = Core::Entity(world);
      common_spawn_setup(enemy.entity, enemy.depth, enemy.point_on_circle);

      enemy.entity.set_material_id(texture_magenta.get_id());
   
      
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
      math::vec2 new_point = Level_funcs::get_point_on_cirlce(enemy.point_on_circle);
      
      const math::vec3 position = trans.get_position();
      
      math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                           math::vec2_get_y(new_point),
                                           math::vec3_get_z(position));
      trans.set_position(new_pos);
    }
    
    // Depth
    {
      enemy.depth += (20.f * dt * static_cast<float>(enemy.direction));
      
      if(!math::is_between(enemy.depth, Level_funcs::get_bottom_of_level(), Level_funcs::get_top_of_level()))
      {
        enemy.depth = math::clamp(enemy.depth, Level_funcs::get_bottom_of_level(), Level_funcs::get_top_of_level());
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
      math::vec2 new_point = Level_funcs::get_point_on_cirlce(enemy.point_on_circle);
      
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
        const float new_depth = math::min(enemy.depth + ((static_cast<float>(rand() % 100) / 50.f) - 0.75f), Level_funcs::get_top_of_level());
      
        Enemy_utils::spawn_egg(world, enemies_container, new_point, new_depth);
      }
    }
    
    // Depth
    if(enemy.direction != 0);
    {
      enemy.depth += (20.f * dt * static_cast<float>(enemy.direction));
      
      if(enemy.depth > Level_funcs::get_top_of_level())
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
        
      case(Enemy_type::egg):
        break;
        
      default:
        assert(false);
    };
  }
}


void
hit_enemy(Core::World &world,
          const Core::Entity_ref &hit,
          Enemies_container &enemies_container,
          Explosions_container &explosions_container,
          Powerups_container &powerups_container)
{
  for(uint32_t i = 0; i < enemies_container.size; ++i)
  {
    auto &enemy = enemies_container.enemy[i];
    
    if(enemy.entity == hit)
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