#include <lib/menu/button.hpp>
#include <common/object_tags.hpp>
#include <common/screen_cast.hpp>
#include <core/world/world.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/box_collider.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/transform/transform.hpp>
#include <core/transform/transform_utils.hpp>
#include <core/camera/camera.hpp>
#include <core/camera/camera_utils.hpp>
#include <core/input/buttons.hpp>
#include <core/common/ray.hpp>
#include <core/input/controller.hpp>
#include <core/model/model.hpp>
#include <core/world/world.hpp>
#include <core/input/input.hpp>
#include <core/resources/texture.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/resources/shader.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/renderer.hpp>
#include <core/input/axis.hpp>
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
    hot_material.set_shader(fb_shader);
    hot_material.set_map_01(hot_texture);
    
    m_hot_material = hot_material;
  }
  
  // Set Entity
  {
    m_entity = Core::Entity(world);
    ASSERT(m_entity);
    
    m_entity.set_name("[button]");
    m_entity.set_tags(Object_tags::gui_cam);
    
    const Core::Model model(Core::Directory::volatile_resource_path("assets/models/unit_cube.obj"));
    ASSERT(model);
    
    const Core::Material_renderer mat_renderer(m_cold_material, model);
    ASSERT(mat_renderer);
    
    m_entity.set_renderer(mat_renderer);
    
    // Transform
    const math::vec3 pos   = Core::Camera_utils::get_world_position_on_nearplane(camera, Core::Axis{math::get_x(position), math::get_y(position)});
    const math::vec3 scale = math::vec3_init(cold_texture.get_width() * 0.2f, cold_texture.get_height() * 0.2f, 0);

    m_entity.set_transform(Core::Transform(pos,
                                           scale,
                                           math::quat_init()));
    
    Core::Box_collider collider(0.5f, 0.5f, 0.5f);
    Core::Rigidbody rb;
    
    rb.set_collider(collider);
    rb.set_mass(0);
    rb.set_is_trigger(true);
    
    m_entity.set_rigidbody(rb);
  }
}


bool
Button::was_touched(Core::Camera &camera,
                    Core::World &world,
                    Core::Context &ctx)

{
  if(!m_entity)
  {
    return false;
  }

  Core::Controller p1_controller(ctx, 0);
  
  const uint32_t buttons = Core::Gamepad_button::button_start | Core::Gamepad_button::button_a;
  
  if(p1_controller.is_button_up_on_frame(buttons) && is_over(camera, world, ctx))
  {
    return true;
  }
  
  return false;
}


bool
Button::is_over(Core::Camera &camera,
                Core::World &world,
                Core::Context &ctx)
{
  if(!m_entity)
  {
    return false;
  }

  const Core::Ray        viewport_ray    = Core::Camera_utils::get_ray_from_viewport(camera, Core::Input::mouse_get_coordinates(ctx));
  const Core::Entity_ref entity_from_ray = world.find_entity_by_ray(viewport_ray);
  
  const Core::Material_renderer renderer = m_entity.get_renderer();
  
  if(entity_from_ray == m_entity)
  {
    const Core::Material_renderer hot_renderer(m_hot_material, renderer.get_model());
    m_entity.set_renderer(hot_renderer);
    
    return true;
  }
  
  const Core::Material_renderer cold_renderer(m_cold_material, renderer.get_model());
  m_entity.set_renderer(cold_renderer);
  
  return false;
}


bool
Button::is_valid() const
{
  return m_entity.is_valid();
}


Button::operator bool() const
{
  return is_valid();
}


} // ns
} // ns