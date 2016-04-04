#include <core/entity/entity_ref.hpp>
#include <core/context/context.hpp>
#include <core/world/world.hpp>
#include <core/world/world_setup.hpp>
#include <core/camera/camera.hpp>
#include <core/camera/camera_properties.hpp>
#include <core/entity/entity.hpp>
#include <core/color/color.hpp>
#include <core/color/color_predefined.hpp>
#include <core/renderer/mesh_renderer.hpp>
#include <core/transform/transform.hpp>
#include <core/input/input.hpp>
#include <core/input/controller.hpp>
#include <math/vec/vec3.hpp>

// Temp Resource loading.
#include <core/model/model.hpp>
#include <core/model/mesh.hpp>
#include <core/material/texture.hpp>
#include <data/resource_data/resource_data.hpp>
#include <utilities/logging.hpp>


Resource_data::Resources resources;

int
main()
{
  Resource_data::resources_init(&resources);
  
  Core::Context context(800, 480, false, "Vortex");
  
  Core::Model model("/Users/PhilCK/Developer/wired/assets/models/unit_cube.obj");
  assert(model.get_id());
  
  Core::Texture texture("/Users/PhilCK/Developer/wired/assets/textures/dev_grid_green_512.png");
  assert(texture.get_id());
  
  Core::World world(Core::World_setup{});
  Core::Mesh_renderer mesh_renderer;
  
  Core::Entity camera_entity = world.create_entity();
  Core::Camera main_camera;
  {
    camera_entity.set_name("Main Camera");
    Core::Transform curr_trans = camera_entity.get_transform();
    curr_trans.set_position(math::vec3_init(0.f, 1.f, 5.f));
    camera_entity.set_transform(curr_trans);
    
    main_camera.set_width(800);
    main_camera.set_height(480);

    main_camera.set_attached_entity(camera_entity);
    main_camera.set_type(Core::Camera_type::perspective);
    main_camera.set_clear_flags(Core::Camera_clear::color | Core::Camera_clear::depth);
    main_camera.set_clear_color(Core::Color_utils::pink());
  }
  
  Core::Entity cube_entity = world.create_entity();
  {
    cube_entity.set_name("Cube");
    cube_entity.set_model(model);
    cube_entity.set_material_id(1);
  }
  
  Core::Input::Controller controller = Core::Input::Controller(context, 0);
  
  while(context.is_open())
  {
    if((controller.get_axis(1).x > 0) || (controller.is_button_down(Core::Input::Button::button_0)))
    {
      assert(false);
    }
  
    mesh_renderer.render();
  }

  return 0;
}