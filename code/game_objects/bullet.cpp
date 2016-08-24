#include <game_objects/bullet.hpp>
#include <common/level_functions.hpp>
#include <common/object_tags.hpp>
#include <core/entity/entity.hpp>
#include <core/world/world.hpp>
#include <core/model/model.hpp>
#include <core/transform/transform.hpp>
#include <core/resources/texture.hpp>
#include <core/resources/shader.hpp>
#include <core/resources/material.hpp>
#include <core/audio/sample.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/box_collider.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/common/directory.hpp>
#include <math/quat/quat.hpp>
#include <math/vec/vec3.hpp>


namespace
{
  Core::Model     model;
  Core::Sample    gun_shot_sample;
  
  Core::Material  bullet_material;
}


namespace Game_object {


Bullet::Bullet(Core::World &world,
               const math::vec2 depth_point,
               const math::vec2 direction,
               const float speed,
               const uint32_t collision_flags,
               const uint32_t owner_id)
: Game_object(world)
, m_depth(math::vec2_get_x(depth_point))
, m_point(math::vec2_get_y(depth_point))
, m_direction(math::vec2_normalize(direction))
, m_speed(speed)
, m_owner_id(owner_id)
{
  // Load up missnig assets
  {
    if(!model)
    {
      const char *model_path = Core::Directory::volatile_resource_path("assets/models/bullet.obj");
      model = Core::Model(model_path);
    }

    if(!bullet_material)
    {
      bullet_material = Core::Material("Bullet");
    
      const char *texture_path = Core::Directory::volatile_resource_path("assets/textures/dev_grid_red_512.png");
      Core::Texture texture(texture_path);
      
      const char *shader_path = Core::Directory::volatile_resource_path("assets/shaders/vortex_dir_light.ogl");
      Core::Shader shader(shader_path);
      
      bullet_material.set_map_01(texture);
      bullet_material.set_shader(shader);
    }

    if(!gun_shot_sample)
    {
      const char *orange_texture_path = Core::Directory::volatile_resource_path("assets/audio/temp_shot.wav");
      gun_shot_sample = Core::Sample(orange_texture_path);
    }
  }

  auto ref = get_entity();

  // General settings
  {
    ref.set_name("Bullet");
    ref.add_tag(Object_tags::world_cam | Object_tags::bullet);
  }
  
  // Model and texture
  {
    Core::Material_renderer mat_renderer;
    mat_renderer.set_model(model);
    mat_renderer.set_material(bullet_material);
    
    ref.set_renderer(mat_renderer);
  }
  
  // Physics
  {
    Core::Rigidbody rb;
  
    Core::Box_collider collider(1,1,1);
    rb.set_collider(collider);
    rb.set_collision_mask(Object_tags::bullet, collision_flags);
    rb.set_is_trigger(true);
    
    ref.set_rigidbody(rb);
  }
  
  // Transform
  {
    const math::vec2 new_point = Level_funcs::get_point_on_cirlce(m_point);
    math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                         math::vec2_get_y(new_point),
                                         m_depth);
    
    auto scale = math::vec3_init(0.9f, 0.75f, 0.75f);
    
    const math::quat rot = math::quat_init_with_axis_angle(0, 1, 0, math::quart_tau());
    
    const Core::Transform transform(
      new_pos,
      scale,
      rot
    );

    ref.set_transform(transform);
  }
}


void
Bullet::on_start()
{
  auto ref = get_entity();
  
  Core::Transform trans = ref.get_transform();
  
  const math::vec2 new_point = Level_funcs::get_point_on_cirlce(m_point);
  
  math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                       math::vec2_get_y(new_point),
                                       m_depth);
  
  trans.set_position(new_pos);
  ref.set_transform(trans);
}


void
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
    }
  }
  
  // Move
  {
    const float dt_speed = dt * m_speed;
    const math::vec2 velocity = math::vec2_scale(m_direction, dt_speed);
    
    m_point += math::degrees_to_radians(math::vec2_get_x(velocity) * math::half_tau());
    m_depth += math::vec2_get_y(velocity);
    
    const math::vec2 new_point = Level_funcs::get_point_on_cirlce(m_point);
    
    math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                         math::vec2_get_y(new_point),
                                         m_depth);
    
    trans.set_position(new_pos);
    ref.set_transform(trans);
  }
}


uint32_t
Bullet::get_owner_id() const
{
  return m_owner_id;
}


} // ns