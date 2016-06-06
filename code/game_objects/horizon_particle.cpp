#include <game_objects/horizon_particle.hpp>
#include <game_objects/world_objects.hpp>
#include <common/level_functions.hpp>
#include <core/transform/transform.hpp>
#include <math/vec/vec3.hpp>


namespace Game_object {


Horizon_particle::Horizon_particle(Core::World &world)
: Game_object(world)
{
  get_entity().set_name("Horizon-Particle");
}


void
Horizon_particle::on_start()
{
}


bool
Horizon_particle::on_update(const float dt, World_objects &objs)
{
  auto ref = get_entity();
  
  Core::Transform trans = ref.get_transform();
  const math::vec3 curr_pos = trans.get_position();
  
  const math::vec3 move_axis = math::vec3_init(0, 0, 1.f);
  const math::vec3 move_scale = math::vec3_scale(move_axis, (dt * m_horizon_speed));
  const math::vec3 moved = math::vec3_add(curr_pos, move_scale);
  
  trans.set_position(moved);
  ref.set_transform(trans);
  
  const float curr_depth = math::vec3_get_z(moved);
  
  if(40.f < curr_depth)
  {
    destroy();
  }
  
  return false;
}


} // ns