#include <game_objects/horizon.hpp>
#include <game_objects/horizon_particle.hpp>
#include <game_objects/world_objects.hpp>
#include <common/level_functions.hpp>
#include <common/object_tags.hpp>
#include <core/world/world.hpp>
#include <core/resources/texture.hpp>
#include <core/resources/material.hpp>
#include <core/resources/shader.hpp>
#include <core/model/model.hpp>
#include <core/transform/transform.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <math/general/general.hpp>
#include <utilities/directory.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/common/directory.hpp>


namespace
{
  Core::Material  ring_material;
  Core::Model     horizon_model;
}


namespace Game_object {


Horizon::Horizon(Core::World &world)
: Game_object(world)
{
  Core::Entity_ref ref = get_entity();
  
  if(!horizon_model)
  {
    const char *path = Core::Directory::volatile_resource_path("assets/models/bullet.obj");
    Core::Model model(path);
    
    horizon_model = model;
  }
  
  if(!ring_material)
  {
    ring_material = Core::Material("outter-ring");
  
    const char *shader_path = Core::Directory::volatile_resource_path("assets/shaders/vortex_outter_level.ogl");
    Core::Shader shader(shader_path);
    
    ring_material.set_shader(shader);
  }

  ref.set_name("Horizon Particle Factory");
}


void
Horizon::on_update(const float dt, World_objects &objs)
{
  m_curr_horz_timer += dt;
  
  while (m_curr_horz_timer > 0.3f)
  {
    m_curr_horz_timer = 0;
    
    constexpr uint32_t number_to_spawn = 5;
    
    for(uint32_t i = 0; i < number_to_spawn; ++i)
    {
      Horizon_particle *particle = new Horizon_particle(get_world(), math::rand_range(10.f, 60.f));
      Core::Entity_ref ref = particle->get_entity();
            
      const float length = math::rand_range(10.f, 25.f);
      const float angle  = math::rand_range(0.f, math::tau());
      
      const float x_pos = math::sin(angle) * length;
      const float y_pos = math::cos(angle) * length;
      const float z_pos = -200.f + (m_curr_horz_timer * 10);
      
      const math::quat rot = math::quat_init_with_axis_angle(0, 1, 0, math::quart_tau());
      
      Core::Transform trans;
      trans.set_position(math::vec3_init(x_pos, y_pos, z_pos));
      trans.set_scale(math::vec3_init(0.8f));
      trans.set_rotation(rot);
      ref.set_transform(trans);

      Core::Material_renderer mat_renderer;
      mat_renderer.set_material(ring_material);
      mat_renderer.set_model(horizon_model);
      ref.set_renderer(mat_renderer);
      ref.set_tags(Object_tags::world_cam);

      objs.push_object(particle);
    }
  }
}


} // ns