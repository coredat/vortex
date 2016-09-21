#include <game_states/about.hpp>
#include <game_states/title_screen.hpp>
#include <factories/material.hpp>
#include <common/game_state.hpp>
#include <common/object_tags.hpp>
#include <core/context/context.hpp>
#include <core/world/world.hpp>
#include <core/resources/material.hpp>
#include <core/resources/texture.hpp>
#include <core/model/model.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/resources/shader.hpp>
#include <core/transform/transform.hpp>
#include <core/transform/transform_utils.hpp>
#include <core/common/directory.hpp>
#include <core/camera/camera_properties.hpp>
#include <core/input/controller.hpp>
#include <core/input/buttons.hpp>
#include <core/common/ray.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/box_collider.hpp>
#include <core/renderer/renderer.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/text_renderer.hpp>
#include <core/font/font.hpp>
#include <core/camera/camera.hpp>
#include <core/camera/camera_utils.hpp>
#include <utilities/directory.hpp>
#include <math/quat/quat.hpp>
#include <utilities/logging.hpp>
#include <renderer/debug_line.hpp>
#include <lib/menu_image_list/menu_list.hpp>
#include <assert.h>


namespace {

constexpr uint32_t buttons_count = 2;
Core::Lib::Menu_list::Image_button buttons[buttons_count];

} // anon ns


void
about_init(Core::Context &context,
           Core::World &world,
           Core::Camera &camera)
{
  // Button
  {
    buttons[0].entity         = Core::Entity(world);
    buttons[0].entity.set_name("button_title");
    buttons[0].entity.set_tags(Object_tags::gui_cam);
    buttons[0].cold_material  = Factory::Material::get_about_menu_title();
    buttons[0].hot_material   = Core::Material();
    
    buttons[1].entity         = Core::Entity(world);
    buttons[1].entity.set_name("button_back");
    buttons[1].entity.set_tags(Object_tags::gui_cam);
    buttons[1].cold_material  = Factory::Material::get_menu_back_cold();
    buttons[1].hot_material   = Factory::Material::get_menu_back_hot();
    
    const Core::Model model(Core::Directory::volatile_resource_path("assets/models/unit_cube.obj"));
  
    Core::Lib::Menu_list::inititalize(buttons, buttons_count, model, camera);
  }
}



Game_state
about_update(Core::Context &ctx,
             Core::World &world,
             Core::Camera &camera,
             Game_object::World_objects &objs)
{
  const Core::Controller controller = Core::Controller(ctx, 0);
 
  Core::Lib::Menu_list::navigate(controller, buttons, buttons_count);
  Core::Lib::Menu_list::mouse_over(camera, world, Core::Input::mouse_get_coordinates(ctx), buttons, buttons_count);
  
  const Core::Entity_ref selected_button = buttons[0].entity;
  
  constexpr uint32_t button_start = Core::Gamepad_button::button_a | Core::Gamepad_button::button_start;

  if(selected_button && strcmp(selected_button.get_name(), "button_back") == 0)
  {
    if(controller.is_button_up_on_frame(button_start))
    {
      Core::Lib::Menu_list::clear(buttons, buttons_count);
      return Game_state::title_screen;
    }
  }
  
  // Generic if back then go back
  if(controller.is_button_up_on_frame(Core::Gamepad_button::button_back))
  {
    Core::Lib::Menu_list::clear(buttons, buttons_count);
    return Game_state::title_screen;
  }

  return Game_state::about;
}