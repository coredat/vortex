#include "bullet.hpp"
#include <core/entity/entity.hpp>
#include <core/model/model.hpp>
#include <core/material/texture.hpp>
#include <core/transform/transform.hpp>
#include <common/level_functions.hpp>
#include <math/vec/vec2.hpp>
#include <math/vec/vec3.hpp>
#include <iostream>


namespace
{
  Core::Model   model;//("/Users/PhilCK/Developer/core/assets/models/unit_cube.obj"); // It would be nice to support this in core
  Core::Texture texture;//("/Users/PhilCK/Developer/core/assets/textures/dev_grid_blue_512.png");
}


namespace Bullet_utils {


void
init_bullets(Core::World &world,
             Bullet bullets[],
             const uint32_t number_of_bullets)
{
  model = Core::Model("/Users/PhilCK/Developer/core/assets/models/unit_cube.obj");
  texture = Core::Texture("/Users/PhilCK/Developer/core/assets/textures/dev_grid_blue_512.png");
}


void
move_bullets(Core::World &world,
             const float dt,
             Bullet bullets[],
             const uint32_t number_of_bullets)
{
  for(uint32_t i = 0; i < number_of_bullets; ++i)
  {
    Bullet &bullet = bullets[i];
    
    if(!bullet.entity)
    {
      continue;
    }

    Core::Transform trans = bullet.entity.get_transform();
    
    // Time to die?
    {
      const float depth = math::vec3_get_z(trans.get_position());

      if(!math::is_between(depth, Level::get_near_death_zones(), Level::get_far_death_zone()))
      {
        bullet.entity.destroy();
        continue;
      }
    }
    
    // Move
    {
      const math::vec2 new_point = Level::get_point_on_cirlce(bullet.point_on_circle);
      const math::vec3 position  = trans.get_position();
      
      const float velocity = ((bullet.speed * dt) * bullet.direction);
      const float depth    = math::vec3_get_z(position) + velocity;
      
      math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                           math::vec2_get_y(new_point),
                                           depth);
      
      trans.set_position(new_pos);
      bullet.entity.set_transform(trans);
    }
  }
}


void
create_bullet(Core::World &world,
              const float position,
              const uint32_t direction,
              Bullet bullets[],
              const uint32_t number_of_bullets)
{
  for(uint32_t i = 0; i < number_of_bullets; ++i)
  {
    Bullet &bullet = bullets[i];
    
    if(!bullet.entity)
    {
      bullet.entity = Core::Entity(world);
      bullet.entity.add_tag(4);
      
      bullet.entity.set_model(model);
      bullet.entity.set_material_id(texture.get_id());
      
      bullet.point_on_circle = position;
      bullet.direction = direction;
      
      break;
    }
  }
}


} // ns