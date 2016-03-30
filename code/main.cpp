#include <core/context/context.hpp>
#include <core/world/world.hpp>
#include <core/world/world_setup.hpp>
#include <core/camera/camera.hpp>
#include <core/entity/entity.hpp>
#include <core/color/color.hpp>
#include <core/color/color_predefined.hpp>
#include <core/renderer/mesh_renderer.hpp>


int
main()
{
  Core::Context context(800, 480, false, "Vortex");
  
  Core::World world(Core::World_setup{});
  Core::Mesh_renderer mesh_renderer;
  
  Core::Entity test_entity = world.create_entity();
  
  Core::Camera main_camera;
  main_camera.set_attached_entity(test_entity);
  main_camera.set_clear_flags(Core::Camera_clear::color | Core::Camera_clear::depth);
  main_camera.set_clear_color(Core::Color_utils::magenta());
  
  
  while(context)
  {
    mesh_renderer.render();
  }

  return 0;
}