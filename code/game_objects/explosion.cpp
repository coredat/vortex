#include <game_objects/explosion.hpp>
#include <common/object_tags.hpp>
#include <core/model/model.hpp>
#include <core/resources/texture.hpp>
#include <core/resources/material.hpp>
#include <core/resources/shader.hpp>
#include <core/transform/transform.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <math/general/general.hpp>
#include <utilities/directory.hpp>


namespace
{
  Core::Model model;
  Core::Texture texture;
  
  Core::Material material;
}


namespace Game_object {

  
Explosion::Explosion(Core::World &world, const math::vec3 position)
: Game_object(world)
{
  // Load missing assets
  {
    if(!material)
    {
      const std::string grid_texture_path = util::get_resource_path() + "assets/textures/dev_colored_squares_512.png";
      Core::Texture texture(grid_texture_path.c_str());
      
      const std::string shader_path = util::get_resource_path() + "assets/shaders/basic_fullbright.ogl";
      Core::Shader shader(shader_path.c_str());
      
      material = Core::Material("Explosion");
      material.set_shader(shader);
      material.set_map_01(texture);
    }
  
    if(!model)
    {
      const std::string unit_cube_path = util::get_resource_path() + "assets/models/unit_cube.obj";
      model = Core::Model(unit_cube_path.c_str());
    }
  }

  // Setup entity
  {
    Core::Entity_ref ref = get_entity();

    ref.set_name("Explosion");
    ref.add_tag(Object_tags::explosion | Object_tags::world_cam);
    
    Core::Material_renderer mat_renderer;
    mat_renderer.set_model(model);
    mat_renderer.set_material(material);
    
    ref.set_renderer(mat_renderer);
    
    const Core::Transform transform(
      position,
      math::vec3_zero(),
      math::quat_init()
    );
    
    ref.set_transform(transform);
  }
  
  // Other
  m_time = 0;
}


void
Explosion::on_update(const float dt, World_objects &objs)
{
  m_time += dt * 10.f;
  
  const float new_scale = 0.5f + math::sin(m_time);
  
  if(new_scale < 0)
  {
    destroy();
  }
  
  // Update the scale.
  {
    Core::Entity_ref ref = get_entity();
  
    const math::vec3 scale = math::vec3_init(new_scale);
    Core::Transform trans = ref.get_transform();
    trans.set_scale(scale);
    
    ref.set_transform(trans);
  }
}


} // ns