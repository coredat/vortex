#include <game_objects/explosion.hpp>
#include <common/object_tags.hpp>
#include <core/model/model.hpp>
#include <core/material/texture.hpp>
#include <core/transform/transform.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <math/general/general.hpp>


namespace
{
  Core::Model model;
  Core::Texture texture;
}


namespace Explosion_utils {


void
init_explosions(const Core::World &world,
                Explosion explosions_arr[],
                const uint32_t number_of_explosions)
{
  model = Core::Model("/Users/PhilCK/Developer/core/assets/models/unit_cube.obj");
  texture = Core::Texture("/Users/PhilCK/Developer/core/assets/textures/dev_colored_squares_512.png");
}


void
update_explosions(const Core::World &world,
                  const float dt,
                  Explosion explosions[],
                  const uint32_t number_of_explosions)
{
  for(uint32_t i = 0; i < number_of_explosions; ++i)
  {
    Explosion &exp = explosions[i];
    
    exp.time += dt * 10.f;
    const float new_scale = 0.5f + math::sin(exp.time);
    
    if(new_scale < 0)
    {
      exp.entity.destroy();
      continue;
    }
    
    const math::vec3 scale = math::vec3_init(new_scale);
    Core::Transform trans = exp.entity.get_transform();
    trans.set_scale(scale);
    
    exp.entity.set_transform(trans);
  }
}


void
create_explosion(const Core::World &world,
                 const math::vec3 position,
                 Explosion explosions[],
                 const uint32_t number_of_explosions)
{
  for(uint32_t i = 0; i < number_of_explosions; ++i)
  {
    Explosion &exp = explosions[i];
    
    if(!exp.entity)
    {
      exp.entity = Core::Entity(world);
      exp.entity.set_name("Explosion");
      exp.entity.add_tag(Object_tags::explosion);
      exp.entity.add_tag(1);
      
      exp.entity.set_model(model);
      exp.entity.set_material_id(texture.get_id());
      
      exp.time = 0;
      
      const Core::Transform transform(
        position,
        math::vec3_zero(),
        math::quat_init()
      );
      
      exp.entity.set_transform(transform);
      
      break;
    }
  }
}


} // ns