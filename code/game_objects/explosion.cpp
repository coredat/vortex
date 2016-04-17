#include <game_objects/explosion.hpp>
#include <common/object_tags.hpp>
#include <core/model/model.hpp>
#include <core/material/texture.hpp>
#include <core/transform/transform.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <math/general/general.hpp>
#include <utilities/directory.hpp>


namespace
{
  Core::Model model;
  Core::Texture texture;
}


namespace Explosion_utils {


void
init_explosions(const Core::World &world,
                Explosions_container &explosions_container)
{
  const std::string unit_cube_path = util::get_resource_path() + "assets/models/unit_cube.obj";
  model = Core::Model(unit_cube_path.c_str());

  const std::string grid_texture_path = util::get_resource_path() + "assets/textures/dev_colored_squares_512.png";
  texture = Core::Texture(grid_texture_path.c_str());
}


void
update_explosions(const Core::World &world,
                  const float dt,
                  Explosions_container &explosions_container)
{
  for(uint32_t i = 0; i < explosions_container.size; ++i)
  {
    auto &exp = explosions_container.explosion[i];
    
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
                 Explosions_container &explosions_container)
{
  for(uint32_t i = 0; i < explosions_container.size; ++i)
  {
    auto &exp = explosions_container.explosion[i];
    
    if(!exp.entity)
    {
      exp.entity = Core::Entity(world);
      exp.entity.set_name("Explosion");
      exp.entity.add_tag(Object_tags::explosion);
      
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