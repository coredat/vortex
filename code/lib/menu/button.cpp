#include <lib/menu/button.hpp>
#include <common/object_tags.hpp>
#include <core/world/world.hpp>
#include <core/resources/shader.hpp>
#include <core/common/directory.hpp>


namespace Core {
namespace Lib {


Button::Button(Core::World &world,
               Core::Context &ctx,
               const math::vec3 position,
               const Core::Texture &hot_texture,
               const Core::Texture &cold_texture)
{
  m_entity = Core::Entity(world);
  m_entity.set_name("[button]");
  m_entity.set_tags(Object_tags::gui_cam);
  
  // Generate materials
  {
    Core::Shader fb_shader(Core::Directory::volatile_resource_path("assets/shaders/basic_fullbright.ogl"));
    
    
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