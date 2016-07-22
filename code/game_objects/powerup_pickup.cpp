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
#include <core/physics/rigidbody.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/common/directory.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <random>


namespace
{
  Core::Material power_up_material;
  Core::Model model;

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
  if(!model)
  {
    const char *unit_cube_path = Core::Directory::volatile_resource_path("assets/models/unit_cube.obj");
    model = Core::Model(unit_cube_path);
  }

  if(!power_up_material)
  {
    const char *green_texture_path = Core::Directory::volatile_resource_path("assets/textures/dev_grid_green_512.png");
    Core::Texture texture(green_texture_path);
    
    const char *shader_path = Core::Directory::volatile_resource_path("assets/shaders/basic_fullbright.ogl");
    Core::Shader shader(shader_path);
    
    power_up_material = Core::Material("Powerup");
    power_up_material.set_shader(shader);
    power_up_material.set_map_01(texture);
  }
}


void
Powerup_pickup::on_start()
{
  Core::Box_collider collider = Core::Box_collider_utils::create_with_full_extents(math::vec3_one());
  
  Core::Rigidbody rb;
  rb.set_collider(collider);
  rb.set_is_trigger(true);
  rb.set_collision_mask(Object_tags::powerup, Object_tags::player);
 
  // Setup entity.
  Core::Entity_ref ref = get_entity();
  {
    ref.set_name("Powerup");
    ref.set_tags(Object_tags::powerup | Object_tags::world_cam);

    ref.set_rigidbody(rb);
    
    Core::Transform trans = ref.get_transform();
    
    math::vec2 new_point = Level_funcs::get_point_on_cirlce(m_point_on_circle);
    
    const math::vec3 position = trans.get_position();
    
    math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                         math::vec2_get_y(new_point),
                                         math::vec3_get_z(position));
    trans.set_position(new_pos);
    ref.set_transform(trans);
  }

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
    const math::vec3 pos     = trans.get_position();
    const math::vec3 new_pos = math::vec3_init(math::vec3_get_x(pos),
                                               math::vec3_get_y(pos),
                                               m_depth);
    
    trans.set_position(new_pos);
  }
  
  ref.set_transform(trans);
  
  Core::Material_renderer mat_renderer;
  mat_renderer.set_model(model);
  mat_renderer.set_material(power_up_material);
  ref.set_renderer(mat_renderer);
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
    
    if(!math::is_between(m_depth,
                         Level_funcs::get_near_death_zone(),
                         Level_funcs::get_far_death_zone()))
    {
      destroy();
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
Powerup_pickup::on_end()
{
  
}

void
Powerup_pickup::on_collision(Game_object *obj)
{
}


} // ns