#include <game_objects/enemy.hpp>
#include <game_objects/world_objects.hpp>
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
#include <core/physics/box_collider_utils.hpp>
#include <core/physics/rigidbody_properties.hpp>
#include <math/vec/vec2.hpp>
#include <math/vec/vec3.hpp>
#include <math/geometry/aabb.hpp>
#include <math/quat/quat.hpp>
#include <utilities/directory.hpp>


namespace
{
  Core::Model   model;
  Core::Texture texture_orange;
  Core::Texture texture_magenta;
}


namespace
{

/*
  Climber will crawel up and down the tube.
*/
void
update_climber(Game_object::Enemy &enemy, const float dt)
{
  Core::Transform trans = enemy.get_entity().get_transform();
  
  // Point on circle
  {
    math::vec2 new_point = Level_funcs::get_point_on_cirlce(enemy.m_point_on_circle);
    
    const math::vec3 position = trans.get_position();
    
    math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                         math::vec2_get_y(new_point),
                                         math::vec3_get_z(position));
    trans.set_position(new_pos);
  }
  
  // Depth
  {
    enemy.m_depth += (20.f * dt * static_cast<float>(enemy.m_direction));
    
    if(!math::is_between(enemy.m_depth, Level_funcs::get_bottom_of_level(), Level_funcs::get_top_of_level()))
    {
      enemy.m_depth = math::clamp(enemy.m_depth, Level_funcs::get_bottom_of_level(), Level_funcs::get_top_of_level());
      enemy.m_direction *= -1;
    }
    
    const math::vec3 pos = trans.get_position();
    const math::vec3 new_pos = math::vec3_init(math::vec3_get_x(pos),
                                               math::vec3_get_y(pos),
                                               enemy.m_depth);
    
    trans.set_position(new_pos);
  }
  
  enemy.get_entity().set_transform(trans);
}


/*
  Breaders climb to the top then start to bread.
*/
void
update_breeder(Core::World &world,
               Game_object::World_objects &enemies_container,
               Game_object::Enemy &enemy,
               const float dt)
{
  Core::Transform trans = enemy.get_entity().get_transform();
  
  // Point on circle
  {
    math::vec2 new_point = Level_funcs::get_point_on_cirlce(enemy.m_point_on_circle);
    
    const math::vec3 position = trans.get_position();
    
    math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                         math::vec2_get_y(new_point),
                                         math::vec3_get_z(position));
    trans.set_position(new_pos);
  }
  
  // Should breed
  {
    if(enemy.m_direction == 0 && enemy.m_lifetime > 0.4f)
    {
      enemy.m_lifetime = 0.f; // Need to fix this.
      
      const float new_point = enemy.m_point_on_circle + ((static_cast<float>(rand() % 100) / 250.f) - 0.2f);
      const float new_depth = math::min(enemy.m_depth + ((static_cast<float>(rand() % 100) / 50.f) - 0.75f), Level_funcs::get_top_of_level());
    
      //Enemy_utils::spawn_egg(world, enemies_container, new_point, new_depth);
      enemies_container.push_object(new Game_object::Enemy(world, Game_object::Enemy::Type::egg));
    }
  }
  
  // Depth
  if(enemy.m_direction != 0);
  {
    enemy.m_depth += (20.f * dt * static_cast<float>(enemy.m_direction));
    
    if(enemy.m_depth > Level_funcs::get_top_of_level())
    {
      enemy.m_direction = 0;
//        enemy.lifetime = 0;
    }
    
    const math::vec3 pos = trans.get_position();
    const math::vec3 new_pos = math::vec3_init(math::vec3_get_x(pos),
                                               math::vec3_get_y(pos),
                                               enemy.m_depth);
    
    trans.set_position(new_pos);
  }
  
  enemy.get_entity().set_transform(trans);
}

} // anon ns


namespace Game_object {


Enemy::Enemy(Core::World &world, Type type)
: Game_object(world)
, m_type(type)
{
  const std::string unit_cube_path = util::get_resource_path() + "assets/models/unit_cube.obj";
  model = Core::Model(unit_cube_path.c_str());

  const std::string orange_texture_path = util::get_resource_path() + "assets/textures/dev_grid_orange_512.png";
  texture_orange  = Core::Texture(orange_texture_path.c_str());

  const std::string magenta_texture_path = util::get_resource_path() + "assets/textures/dev_grid_magenta_512.png";
  texture_magenta = Core::Texture(magenta_texture_path.c_str());
}


void
Enemy::on_start()
{
  Core::Entity_ref ref = get_entity();
  
  ref.set_name("Enemy");
  ref.set_tags(Object_tags::enemy);
  ref.set_model(model);
  ref.set_material_id(texture_magenta.get_id());
  
  Core::Box_collider coll = Core::Box_collider_utils::create_with_half_extents(math::aabb_get_half_extents(model.get_model_aabb()));
  ref.set_collider(coll);
  
  Core::Rigidbody_properties rb_props;
  rb_props.set_collision_mask(Object_tags::enemy, Object_tags::bullet | Object_tags::player);
  
  ref.set_rigidbody_properties(rb_props);
  
  m_direction = 1;
  m_lifetime = 0;
  m_point_on_circle = static_cast<float>(rand() % 1000) / 10;
  m_depth = Level_funcs::get_bottom_of_level();
}


bool
Enemy::on_update(const float dt, World_objects &objs)
{
  switch(m_state)
  {
    case(State::alive):
      update_climber(*this, dt);
      break;
      
    case(State::dying):
      destroy();
      objs.push_object(new Explosion(get_world(),
                                     get_entity().get_transform().get_position()));
      m_state = State::dead;
      break;
      
    default: ;
  }

  return false;
}


void
Enemy::on_collision(Game_object::Game_object *other)
{
  m_state = State::dying;
}



} // ns