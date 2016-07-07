#include <game_objects/enemy.hpp>
#include <game_objects/world_objects.hpp>
#include <game_objects/explosion.hpp>
#include <game_objects/powerup_pickup.hpp>
#include <game_objects/enemy_egg.hpp>
#include <game_objects/bullet.hpp>
#include <game_objects/enemy_breeder.hpp>
#include <game_objects/enemy_climber.hpp>
#include <game_objects/enemy_shooter.hpp>
#include <common/level_functions.hpp>
#include <common/event_ids.hpp>
#include <common/object_tags.hpp>
#include <core/model/model.hpp>
#include <core/transform/transform.hpp>
#include <core/resources/texture.hpp>
#include <core/resources/material.hpp>
#include <core/resources/shader.hpp>
#include <core/world/world.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/box_collider.hpp>
#include <core/physics/box_collider_utils.hpp>
#include <core/physics/rigidbody_properties.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <math/vec/vec2.hpp>
#include <math/vec/vec3.hpp>
#include <math/geometry/aabb.hpp>
#include <math/quat/quat.hpp>
#include <core/common/directory.hpp>

namespace
{
  Core::Model   model;
  Core::Material generic_material;
  
  constexpr uint32_t  chances_of_powerup = 10; // to 1
}


namespace
{

/*
  Choose update logic for type.
*/
void
update(Game_object::Enemy &enemy, const float dt, Game_object::World_objects &objs)
{
  switch(enemy.m_type)
  {
    case(Game_object::Enemy::Type::climber):
      Enemy_logic::climber_update(enemy, dt, objs);
      break;
      
    case(Game_object::Enemy::Type::breeder):
      Enemy_logic::breeder_update(enemy, dt, objs);
      break;
      
    case(Game_object::Enemy::Type::egg):
      Enemy_logic::egg_update(enemy, dt, objs);
      break;
      
    case(Game_object::Enemy::Type::shooter):
      Enemy_logic::shooter_update(enemy, dt, objs);
      break;
      
    default:
      break;
  }
}

} // anon ns


namespace Game_object {


Enemy::Enemy(Core::World &world, Type type)
: Game_object(world)
, m_type(type)
{
  if(!generic_material)
  {
    generic_material = Core::Material("Enemy-generic");
    
    const char * orange_texture_path = Core::Directory::volatile_resource_path("assets/textures/dev_grid_orange_512.png");
    Core::Texture texture(orange_texture_path);
    
    const char * shader_path = Core::Directory::volatile_resource_path("assets/shaders/basic_fullbright.ogl");
    Core::Shader shader(shader_path);
    
    generic_material.set_shader(shader);
    generic_material.set_map_01(texture);
  }

  const char * unit_cube_path = Core::Directory::volatile_resource_path("assets/models/unit_cube.obj");
  model = Core::Model(unit_cube_path);
  
  m_direction = 1;
  m_lifetime = 0;
  srand((unsigned int)world.get_time_running());
  m_point_on_circle = static_cast<float>(rand() % 1000) / 10;
  m_depth = Level_funcs::get_bottom_of_level();
}


void
Enemy::on_start()
{
  Core::Entity_ref ref = get_entity();
  
  ref.set_name("Enemy-Unkown");
  ref.set_tags(Object_tags::enemy | Object_tags::world_cam);

  Core::Material_renderer mat_renderer;
  mat_renderer.set_model(model);
  mat_renderer.set_material(generic_material);
  
  ref.set_renderer(mat_renderer);
  
  Core::Box_collider coll = Core::Box_collider_utils::create_with_half_extents(math::aabb_get_half_extents(model.get_model_aabb()));
  ref.set_collider(coll);
  
  Core::Rigidbody_properties rb_props;
  rb_props.set_collision_mask(Object_tags::enemy, Object_tags::bullet | Object_tags::player);
  
  ref.set_rigidbody_properties(rb_props);
  
  switch(m_type)
  {
    case(Enemy::Type::shooter):
      Enemy_logic::shooter_setup(*this);
      break;
  
    case(Enemy::Type::climber):
      Enemy_logic::climber_setup(*this);
      break;
      
    case(Enemy::Type::breeder):
      Enemy_logic::breeder_setup(*this);
      break;
      
    case(Enemy::Type::egg):
      Enemy_logic::egg_setup(*this);
      break;
      
    default:
      assert(false);
  }
}


void
Enemy::on_update(const float dt, World_objects &objs)
{
  m_lifetime += dt;

  switch(m_state)
  {
    case(State::alive):
      update(*this, dt, objs);
      break;
    
    case(State::dying_with_powerup_chance):
    
      if(m_destroyed_by != UINT32_MAX)
      {
        objs.send_event(Event_id::player_destroyed_enemy, &m_destroyed_by);
      }
    
      // Roll dice to see if we drop a power up.
      if(!(rand() % chances_of_powerup))
      {
        objs.push_object(new Powerup_pickup(get_world(),
                                            m_point_on_circle,
                                            m_depth));
      }
    
    // Fall through on purpose here!
    case(State::dying):
      destroy();
          objs.push_object(new Explosion(get_world(),
                                         get_entity().get_transform().get_position()));
          
          
      m_state = State::dead;
      break;
      
    default:
      break;
  }
}


void
Enemy::on_collision(Game_object *other)
{
  if(other)
  {
    Bullet *bullet = reinterpret_cast<Bullet*>(other);
    
    const uint32_t bullet_owner = bullet->get_owner_id();
    m_destroyed_by = bullet_owner;
    
    m_state = State::dying_with_powerup_chance;
  }
}


void
Enemy::on_message(const uint32_t id, void *data)
{
  if(id == Event_id::destroy_all_enemies)
  {
    m_state = State::dying;
  }
}


namespace Enemy_utils {


void
update_position(Core::Entity_ref ref,
                const float position_on_circle,
                const float depth)
{
  Core::Transform trans = ref.get_transform();

  const math::vec2 new_point = Level_funcs::get_point_on_cirlce(position_on_circle);
  const math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                             math::vec2_get_y(new_point),
                                             depth);
  trans.set_position(new_pos);
  
  ref.set_transform(trans);
}


} // ns


} // ns