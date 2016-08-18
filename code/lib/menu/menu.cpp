#include <lib/menu/menu.hpp>
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
  
  if(!model)
  {
    const char *path = Core::Directory::volatile_resource_path("assets/models/unit_cube.obj");
    model = Core::Model(path);
  }
}


void
Menu::add_button(Core::World &world, const Core::Material &mat)
{
  m_buttons.emplace_back(Button{mat, Core::Entity(world)});
  
  m_buttons.back().entity.set_name("[button]");
  m_buttons.back().entity.add_tag(Object_tags::gui_cam); // sink this arg
  
  Core::Material_renderer renderer;
  renderer.set_material(mat);
  renderer.set_model(model);
  
  m_buttons.back().entity.set_renderer(renderer);
  
  const Core::Transform trans(
    math::vec3_add(m_home, math::vec3_init(0, 0, -100)),
    math::vec3_init(100, 100, 1028),
    math::quat()
  );
  
  m_buttons.back().entity.set_transform(trans);
}


void
Menu::clear()
{
}


void
Menu::on_update(Core::Context &ctx, Core::World &world)
{
  
}


} // ns
} // ns