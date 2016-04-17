#include "bullet.hpp"
#include <core/entity/entity.hpp>
#include <core/model/model.hpp>
#include <core/material/texture.hpp>
#include <core/transform/transform.hpp>
#include <core/audio/sample.hpp>
#include <common/level_functions.hpp>
#include <common/object_tags.hpp>
#include <math/vec/vec2.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <utilities/directory.hpp>
#include <iostream>


namespace
{
  Core::Model   model; // Nice to be able to load this at global init.
  Core::Texture texture;
  Core::Sample  gun_shot_sample;
}


namespace Bullet_utils {


void
init_bullets(Core::World &world,
             Bullets_container &bullets_container)
{
  const std::string unit_cube_path = util::get_resource_path() + "assets/models/unit_cube.obj";
  model = Core::Model(unit_cube_path.c_str());

  const std::string texture_path = util::get_resource_path() + "assets/textures/dev_grid_red_512.png";
  texture = Core::Texture(texture_path.c_str());

  const std::string orange_texture_path = util::get_resource_path() + "assets/audio/temp_shot.wav";
  gun_shot_sample = Core::Sample(orange_texture_path.c_str());
}


void
move_bullets(Core::World &world,
             const float dt,
             Bullets_container &bullets_container)
{
  for(uint32_t i = 0; i < bullets_container.size; ++i)
  {
    auto &bullet = bullets_container.bullet[i];
    
    if(!bullet.entity)
    {
      continue;
    }

    Core::Transform trans = bullet.entity.get_transform();
    
    // Time to die?
    {
      const float depth = math::vec3_get_z(trans.get_position());

      if(!math::is_between(depth, Level::get_near_death_zone(), Level::get_far_death_zone()))
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
              Bullets_container &bullets_container)
{
  for(uint32_t i = 0; i < bullets_container.size; ++i)
  {
    auto &bullet = bullets_container.bullet[i];
    
    if(!bullet.entity)
    {
      bullet.entity = Core::Entity(world);
      bullet.entity.set_name("Bullet");
      bullet.entity.add_tag(Object_tags::bullet);
      
      bullet.entity.set_model(model);
      bullet.entity.set_material_id(texture.get_id());
      
      bullet.point_on_circle = position;
      bullet.direction = direction;
      
      const Core::Transform transform(
        math::vec3_zero(),
        math::vec3_init(0.5f, 0.5f, 1.f),
        math::quat_init()
      );
      
      bullet.entity.set_transform(transform);

      gun_shot_sample.play();
      
      break;
    }
  }
}


} // ns