#include <lib/menu/button.hpp>
#include <common/object_tags.hpp>
#include <common/screen_cast.hpp>
#include <core/world/world.hpp>
#include <core/transform/transform.hpp>
#include <core/transform/transform_utils.hpp>
#include <core/camera/camera.hpp>
#include <core/model/model.hpp>
#include <core/resources/texture.hpp>
#include <core/resources/shader.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/renderer.hpp>
#include <core/common/directory.hpp>
#include <utilities/assert.hpp>


namespace Core {
namespace Lib {


Button::Button()
{
}


Button::Button(Core::World &world,
               Core::Context &ctx,
               const char *button_name,
               const math::vec2 position,
               const Core::Camera &camera,
               const Core::Texture &hot_texture,
               const Core::Texture &cold_texture)
{
  // Generate materials
  {
    Core::Shader fb_shader(Core::Directory::volatile_resource_path("assets/shaders/basic_fullbright.ogl"));
    ASSERT(fb_shader);
    
    char material_name[1024];
    memset(material_name, 0, sizeof(material_name));
    
    strcat(material_name, button_name);
    strcat(material_name, "_cold");
    
    Core::Material cold_material(button_name);
    cold_material.set_shader(fb_shader);
    cold_material.set_map_01(cold_texture);
    
    m_cold_material = cold_material;
    
    strcat(material_name, button_name);
    strcat(material_name, "_hot");
    
    Core::Material hot_material(button_name);
    cold_material.set_shader(fb_shader);
    cold_material.set_map_01(hot_texture);
    
    m_hot_material = hot_material;
  }
  
  // Set Entity
  {
    m_entity = Core::Entity(world);
    ASSERT(m_entity);
    
    m_entity.set_name("[button]");
    m_entity.set_tags(Object_tags::gui_cam);
    
    const Core::Model model(Core::Directory::volatile_resource_path("assets/models/unit_plane.obj"));
    ASSERT(model);
    
    const Core::Material_renderer mat_renderer(m_cold_material, model);
    ASSERT(mat_renderer);
    
    m_entity.set_renderer(mat_renderer);
    
    // Transform
    const math::vec3 pos   = Screen_cast::intersect_screen_plane(camera, math::get_x(position), math::get_y(position));
    const math::quat rot   = math::quat_init_with_axis_angle(Core::Transform::get_world_left(), -math::quart_tau());
    const math::vec3 scale = math::vec3_init(cold_texture.get_width() * 0.2f, 1.f, cold_texture.get_height() * 0.2f);

    m_entity.set_transform(Core::Transform(pos,
                                           scale,
                                           rot));
    
    
  }
}


bool
Button::was_touched()
{
  return false;
}


bool
Button::is_over()
{
  return false;
}


} // ns
} // ns