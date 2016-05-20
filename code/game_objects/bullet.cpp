#include <game_objects/bullet.hpp>
#include <common/level_functions.hpp>
#include <common/object_tags.hpp>
#include <core/entity/entity.hpp>
#include <core/model/model.hpp>
#include <core/transform/transform.hpp>
#include <core/resources/texture.hpp>
#include <core/audio/sample.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/box_collider.hpp>
#include <core/physics/rigidbody_properties.hpp>
#include <math/vec/vec2.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <utilities/directory.hpp>


namespace
{
  Core::Model   model; // Nice to be able to load this at global init.
  Core::Texture texture;
  Core::Sample  gun_shot_sample;
}


namespace Game_object {


Bullet::Bullet(Core::World &world)
: Game_object(world)
{
}

  
void
Bullet::on_start()
{
}


void
Bullet::on_update(const float dt)
{
}


} // ns



namespace Bullet_utils {


void
init_bullets(Core::World &world,
             Bullets_container &bullets_container)
{
  const std::string unit_cube_path = util::get_resource_path() + "assets/models/bullet.obj";
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

      if(!math::is_between(depth, Level_funcs::get_near_death_zone(), Level_funcs::get_far_death_zone()))
      {
        bullet.entity.destroy();
        bullet = Bullets_container::Bullet();
        continue;
      }
    }
    
    // Move
    {
      const math::vec2 new_point = Level_funcs::get_point_on_cirlce(bullet.point_on_circle);
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
              const float depth,
              const uint32_t direction,
              Bullets_container &bullets_container)
{
  for(uint32_t i = 0; i < bullets_container.size; ++i)
  {
    auto &bullet = bullets_container.bullet[i];
    
    if(!bullet.entity)
    {
      // General settings
      {
        bullet.entity = Core::Entity(world);
        bullet.entity.set_name("Bullet");
        bullet.entity.add_tag(Object_tags::bullet);
      }
      
      // Model and texture
      {
        bullet.entity.set_model(model);
        bullet.entity.set_material_id(texture.get_id());
      }
      
      // Transform
      {
        bullet.point_on_circle = position;
        bullet.direction = direction;
      
        const math::vec2 new_point = Level_funcs::get_point_on_cirlce(bullet.point_on_circle);
        math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                             math::vec2_get_y(new_point),
                                             depth);
      
        auto scale = math::vec3_init(1.6f, 0.6f, 0.6f);

        const math::quat rot = math::quat_init_with_axis_angle(0, 1, 0, math::quart_tau());
      
        const Core::Transform transform(
          new_pos,//math::vec3_init(0, 0, depth),
          scale,
          rot
        );

        bullet.entity.set_transform(transform);
      }
      
      // Physics
      {
        Core::Box_collider collider(0.5f, 0.5f, 0.5f);
        bullet.entity.set_collider(collider);
      
        // Rigidbody properties
        Core::Rigidbody_properties rb_props;
        rb_props.set_collision_mask(Object_tags::bullet, Object_tags::enemy);
        bullet.entity.set_rigidbody_properties(rb_props);
      }

      gun_shot_sample.play();
      
      break;
    }
  }
}


} // ns