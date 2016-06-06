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


Bullet::Bullet(Core::World &world,
               const float point,
               const float depth,
               const uint32_t dir)
: Game_object(world)
{
  // Load up missnig assets
  {
    if(!model)
    {
      const std::string unit_cube_path = util::get_resource_path() + "assets/models/bullet.obj";
      model = Core::Model(unit_cube_path.c_str());
    }

    if(!texture)
    {
      const std::string texture_path = util::get_resource_path() + "assets/textures/dev_grid_red_512.png";
      texture = Core::Texture(texture_path.c_str());
    }

    if(!gun_shot_sample)
    {
      const std::string orange_texture_path = util::get_resource_path() + "assets/audio/temp_shot.wav";
      gun_shot_sample = Core::Sample(orange_texture_path.c_str());
    }
  }

  // Setup Members
  {
    m_direction = dir;
    m_point_on_circle = point;
  }

  auto ref = get_entity();

  // General settings
  {
    ref.set_name("Bullet");
    ref.add_tag(Object_tags::bullet);
  }
  
  // Model and texture
  {
    ref.set_model(model);
    ref.set_material_id(texture.get_id());
  }
  
  // Physics
  {
    Core::Box_collider collider(0.5f, 0.5f, 0.5f);
    ref.set_collider(collider);
  
    // Rigidbody properties
    Core::Rigidbody_properties rb_props;
    rb_props.set_collision_mask(Object_tags::bullet, Object_tags::enemy);
    ref.set_rigidbody_properties(rb_props);
  }
  
  // Transform
  {
    const math::vec2 new_point = Level_funcs::get_point_on_cirlce(m_point_on_circle);
    math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                         math::vec2_get_y(new_point),
                                         depth);
  
    auto scale = math::vec3_init(1.f, 0.5f, 0.5f);

    const math::quat rot = math::quat_init_with_axis_angle(0, 1, 0, math::quart_tau());
  
    const Core::Transform transform(
      new_pos,
      scale,
      rot
    );

    ref.set_transform(transform);
  }
}


bool
Bullet::on_update(const float dt, World_objects &world_objs)
{
  Core::Entity_ref ref = get_entity();
  
  Core::Transform trans = ref.get_transform();
  
  // Time to die?
  {
    const float depth = math::vec3_get_z(trans.get_position());

    if(!math::is_between(depth,
                         Level_funcs::get_near_death_zone(),
                         Level_funcs::get_far_death_zone()))
    {
      destroy();
      return false;
    }
  }
  
  // Move
  {
    const math::vec2 new_point = Level_funcs::get_point_on_cirlce(m_point_on_circle);
    const math::vec3 position  = trans.get_position();
    
    const float velocity = ((m_speed * dt) * m_direction);
    const float depth    = math::vec3_get_z(position) + velocity;
    
    math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                         math::vec2_get_y(new_point),
                                         depth);
    
    trans.set_position(new_pos);
    ref.set_transform(trans);
  }
  
  return true;
}


} // ns