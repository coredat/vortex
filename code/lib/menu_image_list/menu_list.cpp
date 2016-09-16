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
  int32_t margin_x = 10;
  int32_t margin_y = 0;

  for(uint32_t i = 0; i < button_count; ++i)
  {
    Image_button &button = buttons_arr[i];
    Core::Entity_ref ent_ref = button.entity;
    
    const Core::Texture texture = button.hot_material.get_map_01();
    const int32_t width = texture.get_width() >> 2;
    const int32_t height = texture.get_height() >> 2;
    
    const math::vec3 scale = math::vec3_init(math::to_float(width) * 0.5f,
                                             math::to_float(height) * 0.5f,
                                             0.f);
    
    const math::vec3 position = math::vec3_init(margin_x + (width * 0.25f), margin_y - height, 0);
    
    Core::Transform button_trans = ent_ref.get_transform();
    button_trans.set_scale(scale);
    button_trans.set_position(position);
    
    ent_ref.set_transform(button_trans);
    
    // Create renderer
    Core::Material_renderer mat_renderer;
    
    // Top button is hot
    const Core::Material mat = i ? button.cold_material : button.hot_material;
    
    mat_renderer.set_material(mat);
    mat_renderer.set_model(model);
    
    ent_ref.set_renderer(mat_renderer);
    
    margin_y -= height;
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
         Image_button *button_arr,
         const uint32_t button_count)
{
  assert(button_count);

  int dir = 0;

  // Choose direction.
  if(controller.is_button_down_on_frame(Core::Gamepad_button::button_dpad_up))
  {
    dir = -1;
  }
  else if(controller.is_button_down_on_frame(Core::Gamepad_button::button_dpad_down))
  {
    dir = +1;
  }
  
  // If the controller hasn't moved then return nothing.
  if(!dir)
  {
    // Only return a ref if we've changed order.
    return Core::Entity_ref();
  }
  
  // Set top button to cold.
  {
    Core::Entity_ref ref = button_arr[0].entity;
    
    Core::Material_renderer renderer = ref.get_renderer();
    assert(renderer);
    
    renderer.set_material(button_arr[0].cold_material);
    ref.set_renderer(renderer);
  }
  
  // Shuffle the array.
  // We want the top element the selected one.
  {
    Image_button temp = static_cast<Image_button&&>(button_arr[0]);
    
    for(int32_t i = 0; i < static_cast<int32_t>(button_count) - 1; ++i)
    {
      const int32_t index = (static_cast<int32_t>(i) * dir) % button_count;
      const int32_t next_index = (index + dir) % button_count;
    
      button_arr[index] = static_cast<Image_button&&>(button_arr[next_index]);
    }
    
    int32_t last = dir > 0 ? button_count - 1 : 1;
    
    button_arr[last] = static_cast<Image_button&&>(temp);
  }
  
  // Set the new top to hot.
  Core::Entity_ref ref = button_arr[0].entity;
  
  Core::Material_renderer renderer = ref.get_renderer();
  assert(renderer);
  
  renderer.set_material(button_arr[0].hot_material);
  ref.set_renderer(renderer);

  // Return currently selected element.
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