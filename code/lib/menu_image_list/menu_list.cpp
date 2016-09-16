#include <lib/menu_image_list/menu_list.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/resources/texture.hpp>
#include <core/renderer/renderer.hpp>
#include <core/input/controller.hpp>
#include <core/input/buttons.hpp>
#include <core/transform/transform.hpp>
#include <assert.h>


namespace Core {
namespace Lib {
namespace Menu_list {


void
inititalize(Image_button *buttons_arr,
            const uint32_t button_count,
            const Core::Model &model,
            const Core::Camera &camera)
{
  uint32_t margin_x = 10;
  uint32_t margin_y = 100;

  for(uint32_t i = 0; i < button_count; ++i)
  {
    Image_button &button = buttons_arr[i];
    Core::Entity_ref ent_ref = button.entity;
    
    const Core::Texture texture = button.hot_material.get_map_01();
    const uint32_t width = texture.get_width() >> 2;
    const uint32_t height = texture.get_height() >> 2;
    
    margin_y += height;
    
    const math::vec3 scale = math::vec3_init(math::to_float(width) * 0.5f,
                                             math::to_float(height) * 0.5f,
                                             1.f);
    
    const math::vec3 position = math::vec3_init(margin_x + (width * 0.25f), margin_y, 0);
    
    Core::Transform button_trans = ent_ref.get_transform();
    button_trans.set_scale(scale);
    button_trans.set_position(position);
    
    ent_ref.set_transform(button_trans);
    
    // Create renderer
    Core::Material_renderer mat_renderer;
    mat_renderer.set_material(button.cold_material);
    mat_renderer.set_model(model);
    
    ent_ref.set_renderer(mat_renderer);
  }
}


void
align_to_camera(const Core::Camera &camera,
                const Image_button *buttons_arr,
                const uint32_t button_count)
{
  
}


Core::Entity_ref
navigate(const Core::Controller &controller,
         const Image_button *button_arr,
         const uint32_t button_count)
{
  assert(button_count);

  int dir = 0;

  // Choose direction.
  if(controller.is_button_down_on_frame(Core::Gamepad_button::button_dpad_up))
  {
    dir = +1;
  }
  else if(controller.is_button_down_on_frame(Core::Gamepad_button::button_dpad_down))
  {
    dir = -1;
  }
  
  /*
    If the controller hasn't moved then return nothing.
  */
  if(!dir)
  {
    return Core::Entity_ref();
  }
  
  // Set top button to cold.
  {
    
  }
  
  // Shuffle the array.
  {
    
  }

  return button_arr[0].entity;
}


Core::Entity_ref
mouse_over(const Image_button *button_arr,
           const uint32_t button_count)
{
  return Core::Entity_ref();
}



} // ns
} // ns
} // ns