#include <lib/menu/menu.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/world/world.hpp>
#include <core/context/context.hpp>
#include <core/camera/camera.hpp>
#include <core/camera/camera_utils.hpp>
#include <core/input/axis.hpp>
#include <core/common/ray.hpp>
#include <core/model/model.hpp>
#include <core/world/world.hpp>
#include <core/common/directory.hpp>
#include <core/resources/material.hpp>
#include <core/resources/texture.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/renderer.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/box_collider.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/transform/transform.hpp>
#include <core/font/font.hpp>
#include <core/renderer/text_renderer.hpp>
#include <common/object_tags.hpp>


namespace {

Core::Model model;
constexpr float paddiny_y = 10.f; // padding between buttons

} // ns


namespace Core {
namespace Lib {


Menu::Menu()
{
}


namespace {


/*
  Adds an item to the menu list.
  returns the cursor offset for the next item.
*/
math::vec3
create_menu_item(Core::Entity_ref entity,
                 const Core::Material &mat,
                 const math::vec3 cursor_position,
                 const bool add_rb)
{
  if(!model)
  {
    const char *path = Core::Directory::volatile_resource_path("assets/models/unit_cube.obj");
    model = Core::Model(path);
  }
  
  Core::Material_renderer renderer;
  renderer.set_material(mat);
  renderer.set_model(model);
  
  const Core::Texture texture = mat.get_map_01();
  const float width = math::to_float(texture.get_width() >> 2);
  const float height = math::to_float(texture.get_height() >> 2);
  
  const Core::Transform trans(
    math::vec3_init(math::get_x(cursor_position) + (width * 0.5f),
                    math::get_y(cursor_position) - (height * 0.5f),
                    0.f),
    math::vec3_init(width,
                    height,
                    1.f),
    math::quat()
  );
  
  entity.set_renderer(renderer);
  entity.set_transform(trans);

  if(add_rb)
  {
    Core::Rigidbody rb;
    rb.set_collider(Core::Box_collider(0.5f, 0.5f, 0.5f));
    rb.set_is_dynamic(false);
    rb.set_is_trigger(true);
    entity.set_rigidbody(rb);
  }
  
  return math::vec3_subtract(cursor_position, math::vec3_init(0.f, height + paddiny_y, 0.f));;
}


}


void
Menu::set_home(const math::vec2 location,
               Core::World &world,
               const Core::Camera &camera,
               const Core::Material &title_mat)
{
  m_home = Core::Camera_utils::get_world_position_on_nearplane(camera, Core::Axis{math::get_x(location), math::get_y(location)});
  m_cursor = m_home;
  
  m_title = Core::Entity(world);
  m_title.set_name("[menu]title");
  m_title.set_tags(Object_tags::gui_cam);
  m_cursor = create_menu_item(m_title, title_mat, m_cursor, false);
}


void
Menu::add_button(const char *name, Core::World &world, const Core::Material &hot, const Core::Material &cold)
{
  m_buttons.emplace_back(Button{cold, hot, Core::Entity(world)});
  
  Core::Entity_ref button = m_buttons.back().entity;
  
  button.set_name(name);
  button.add_tag(Object_tags::gui_cam); // sink this arg

  m_cursor = create_menu_item(button, cold, m_cursor, true);
}


void
Menu::clear()
{
  if(m_title)
  {
    m_title.destroy();
  }

  for(uint32_t i = 0; i < m_buttons.size(); ++i)
  {
    if(m_buttons[i].entity)
    {
      m_buttons[i].entity.destroy();
    }
  }
  
  m_buttons.clear();
}


void
Menu::think(Core::Context &ctx, Core::World &world, const Core::Camera &camera)
{
  const Core::Ray        viewport_ray    = Core::Camera_utils::get_ray_from_viewport(camera, Core::Input::mouse_get_coordinates(ctx));
  const Core::Entity_ref entity_from_ray = world.find_entity_by_ray(viewport_ray);
  
  m_curr_button_selected = Core::Entity_ref();
  
  for(uint32_t i = 0; i < m_buttons.size(); ++i)
  {
    Button *button = &m_buttons[i];
  
    Core::Material_renderer mat_renderer = button->entity.get_renderer();
    
    if(entity_from_ray && (button->entity == entity_from_ray))
    {
      mat_renderer.set_material(button->hot_material);
      button->entity.set_renderer(mat_renderer);
      m_curr_button_selected = button->entity;
    }
    else
    {
      mat_renderer.set_material(button->norm_material);
      button->entity.set_renderer(mat_renderer);
    }
  }
}


Core::Entity_ref
Menu::current_button_selected() const
{
  return m_curr_button_selected;
}


} // ns
} // ns