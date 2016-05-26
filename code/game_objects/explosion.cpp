#include <game_objects/explosion.hpp>
#include <common/object_tags.hpp>
#include <core/model/model.hpp>
#include <core/resources/texture.hpp>
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


namespace Game_object {


  
Explosion::Explosion(Core::World &world, const math::vec3 position)
: Game_object(world)
{
  auto ref = get_entity();

  const Core::Transform transform(
    position,
    math::vec3_zero(),
    math::quat_init()
  );
  
  ref.set_transform(transform);
}


void
Explosion::on_start()
{
  const std::string unit_cube_path = util::get_resource_path() + "assets/models/unit_cube.obj";
  model = Core::Model(unit_cube_path.c_str());

  const std::string grid_texture_path = util::get_resource_path() + "assets/textures/dev_colored_squares_512.png";
  texture = Core::Texture(grid_texture_path.c_str());
  
  {
    auto ref = get_entity();
  
    ref.set_name("Explosion");
    ref.add_tag(Object_tags::explosion);
    
    ref.set_model(model);
    ref.set_material_id(texture.get_id());
    
    m_time = 0;
  }
}


bool
Explosion::on_update(const float dt, World_objects &objs)
{
  m_time += dt * 10.f;
  const float new_scale = 0.5f + math::sin(m_time);
  
  if(new_scale < 0)
  {
    //get_entity().destroy();
    destroy();
    return false;
  }
  
  if(get_entity().is_valid())
  {
    const math::vec3 scale = math::vec3_init(new_scale);
    Core::Transform trans = get_entity().get_transform();
    trans.set_scale(scale);
    
    get_entity().set_transform(trans);
  }

  return false;
}


} // ns