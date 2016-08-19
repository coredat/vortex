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
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/renderer.hpp>
#include <core/transform/transform.hpp>
#include <core/font/font.hpp>
#include <core/renderer/text_renderer.hpp>
#include <common/object_tags.hpp>


namespace {

Core::Model model;

} // ns


namespace Core {
namespace Lib {


Menu::Menu()
{
}


void
Menu::set_home(const math::vec2 location, const Core::Camera &camera)
{
  m_home = Core::Camera_utils::get_world_position_on_nearplane(camera, Core::Axis{math::get_x(location), math::get_y(location)});
  m_cursor = m_home;
  
  if(!model)
  {
//    const char *path = Core::Directory::volatile_resource_path("assets/models/unit_cube.obj");
//    model = Core::Model(path);
  }
}


void
Menu::add_button(Core::World &world, const Core::Material &mat)
{
  m_buttons.emplace_back(Button{mat, Core::Entity(world)});
  
  m_buttons.back().entity.set_name("[button]");
  m_buttons.back().entity.add_tag(Object_tags::gui_cam); // sink this arg
  
//  Core::Material_renderer renderer;
//  renderer.set_material(mat);
//  renderer.set_model(model);

  Core::Text_renderer renderer;
  renderer.set_font(Core::Font("/Users/PhilCK/Desktop/font/LiberationSerif-Bold.ttf"));
  renderer.set_text("foobar");
  
  m_buttons.back().entity.set_renderer(renderer);
  
  const Core::Transform trans(
    m_cursor,
    math::vec3_init(1,1,1),
    math::quat()
  );
  
  m_buttons.back().entity.set_transform(trans);
  
  m_cursor = math::vec3_subtract(m_cursor, math::vec3_init(0.f, math::get_y(trans.get_scale()) * 1.1f, 0.f));
}


void
Menu::clear()
{
}


void
Menu::think(Core::Context &ctx, Core::World &world, const Core::Camera &camera)
{
  const Core::Ray        viewport_ray    = Core::Camera_utils::get_ray_from_viewport(camera, Core::Input::mouse_get_coordinates(ctx));
  const Core::Entity_ref entity_from_ray = world.find_entity_by_ray(viewport_ray);
  
  if(entity_from_ray)
  {
    int i = 0;
  }
}


} // ns
} // ns