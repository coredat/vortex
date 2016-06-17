#include <game_objects/powerup_pickup.hpp>
#include <common/object_tags.hpp>
#include <common/level_functions.hpp>
#include <core/world/world.hpp>
#include <core/model/model.hpp>
#include <core/resources/texture.hpp>
#include <core/resources/material.hpp>
#include <core/resources/shader.hpp>
#include <core/transform/transform.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/box_collider.hpp>
#include <core/physics/box_collider_utils.hpp>
#include <core/physics/rigidbody_properties.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <utilities/directory.hpp>
#include <random>


namespace
{
  Core::Material power_up_material;

  constexpr float powerup_climb_speed = 10.f;
}


namespace Game_object {


Powerup_pickup::Powerup_pickup(Core::World &world,
                               const float point_on_circle,
                               const float depth)
: Game_object(world)
, m_point_on_circle(point_on_circle)
, m_depth(depth)
{
  if(!power_up_material)
  {
    const std::string green_texture_path = util::get_resource_path() + "assets/textures/dev_grid_green_512.png";
    Core::Texture texture(green_texture_path.c_str());
    
    const std::string shader_path = util::get_resource_path() + "assets/shaders/basic_fullbright.ogl";
    Core::Shader shader(shader_path.c_str());
    
    power_up_material = Core::Material("Powerup");
    power_up_material.set_shader(shader);
    power_up_material.set_map_01(texture);
  }

  const std::string unit_cube_path = util::get_resource_path() + "assets/models/unit_cube.obj";
  Core::Model model(unit_cube_path.c_str());

  Core::Box_collider collider = Core::Box_collider_utils::create_with_full_extents(math::vec3_one());
  
  Core::Rigidbody_properties rb_props;
  rb_props.set_collision_mask(Object_tags::powerup, Object_tags::player);
 
  // Setup entity.
  Core::Entity_ref ref = get_entity();
  {
    ref.set_name("Powerup");
    ref.set_tags(Object_tags::powerup | Object_tags::world_cam);
    ref.set_model(model);
    ref.set_material(power_up_material);
    ref.set_collider(collider);
    ref.set_rigidbody_properties(rb_props);
  }
}


void
Powerup_pickup::on_start()
{
}


void
Powerup_pickup::on_update(const float dt, World_objects &objs)
{
  auto ref = get_entity();

  Core::Transform trans = ref.get_transform();
  
  // Point on circle
  {
    math::vec2 new_point = Level_funcs::get_point_on_cirlce(m_point_on_circle);
    
    const math::vec3 position = trans.get_position();
    
    math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                         math::vec2_get_y(new_point),
                                         math::vec3_get_z(position));
    trans.set_position(new_pos);
  }
  
  // Depth
  {
    m_depth += (powerup_climb_speed * dt);
    
    if(m_depth > Level_funcs::get_near_death_zone())
    {
      should_destroy();
    }
    
    const math::vec3 pos     = trans.get_position();
    const math::vec3 new_pos = math::vec3_init(math::vec3_get_x(pos),
                                               math::vec3_get_y(pos),
                                               m_depth);
    
    trans.set_position(new_pos);
  }
  
  ref.set_transform(trans);
}


void
Powerup_pickup::on_collision(Game_object::Game_object *obj)
{
}


} // ns