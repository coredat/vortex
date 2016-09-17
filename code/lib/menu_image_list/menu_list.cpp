#include <lib/menu_image_list/menu_list.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/resources/texture.hpp>
#include <core/physics/box_collider.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/renderer/renderer.hpp>
#include <core/input/controller.hpp>
#include <core/input/buttons.hpp>
#include <core/transform/transform.hpp>
#include <core/camera/camera.hpp>
#include <core/camera/camera_utils.hpp>
#include <core/world/world.hpp>
#include <core/common/ray.hpp>
#include <core/input/input.hpp>
#include <assert.h>


namespace Core {
namespace Lib {
namespace Menu_list {


void
inititalize(Image_button buttons_arr[],
            const uint32_t button_count,
            const Core::Model &model,
            const Core::Camera &camera)
{
  int32_t margin_x = 10;
  int32_t margin_y = 100;

  for(uint32_t i = 0; i < button_count; ++i)
  {
    Image_button &button = buttons_arr[i];
    Core::Entity_ref ent_ref = button.entity;
    
    const Core::Texture texture = button.cold_material.get_map_01();
    const int32_t width         = texture.get_width() >> 2;
    const int32_t height        = texture.get_height() >> 2;
    
    margin_y -= (height * 0.5f);
    
    // Transform
    {
      const math::vec3 position = math::vec3_init(margin_x + (width * 0.5f), margin_y, 0);
      const math::vec3 scale = math::vec3_init(math::to_float(width),
                                               math::to_float(height),
                                               0.f);
      
      ent_ref.set_transform(Core::Transform(
        position,
        scale,
        math::quat_init()
      ));
    }
    
    // Create renderer
    ent_ref.set_renderer(Core::Material_renderer(button.cold_material, model));

    // Create collider if it has a hot material.
    if(button.hot_material)
    {
      Core::Box_collider box(0.5f, 0.5f, 0.5f);
      Core::Rigidbody rb;
      rb.set_mass(0);
      rb.set_is_dynamic(false);
      rb.set_collider(box);
      
      ent_ref.set_rigidbody(rb);
    }
    
    margin_y -= (height * 0.5f);
  }
  
  // If the first item can be made hot, do so, otherwise we call navigate to find it.
  if(buttons_arr[0].hot_material)
  {
    buttons_arr[0].entity.set_renderer(Core::Material_renderer(buttons_arr[0].hot_material, model));
  }
  else
  {
    navigate(1, buttons_arr, button_count);
  }
}


namespace {


/*
  Shuffles the array elements up or down by one.
  If an element falls of the back it will be put onto the front.
*/
inline void
shuffle_array(Image_button button_arr[],
              const uint32_t button_count,
              const int32_t dir)
{
  assert((dir == +1) || (dir == -1));
  assert(button_count);

  // Set top item to cold
  {
    Core::Entity_ref ref = button_arr[0].entity;
    assert(ref);
    
    Core::Material_renderer renderer = ref.get_renderer();
    assert(renderer);
    
    renderer.set_material(button_arr[0].cold_material);
    ref.set_renderer(renderer);
  }
  
  // Shuffle the array.
  // We want the top element the selected one.
  {
    // Move out to temp shuffle elements.
    Image_button temp = static_cast<Image_button&&>(button_arr[0]);
    
    for(int32_t i = 0; i < static_cast<int32_t>(button_count) - 1; ++i)
    {
      const int32_t index      = math::mod((i * dir), button_count);
      const int32_t next_index = math::mod((index + dir), button_count);
    
      button_arr[index] = static_cast<Image_button&&>(button_arr[next_index]);
    }
    
    const int32_t last = dir > 0 ? button_count - 1 : 1;
    
    button_arr[last] = static_cast<Image_button&&>(temp);
  }
  
  // Set the new top to hot.
  if(button_arr[0].hot_material)
  {
    Core::Entity_ref ref = button_arr[0].entity;
    
    Core::Material_renderer renderer = ref.get_renderer();
    assert(renderer);
    
    renderer.set_material(button_arr[0].hot_material);
    ref.set_renderer(renderer);
  }
}


} // anon ns

Core::Entity_ref
navigate(uint32_t dir,
         Image_button button_arr[],
         const uint32_t button_count)
{
  if(dir == 0)
  {
    return Core::Entity_ref();
  }
  
  /*
    While true so we don't navigate onto something unselectable.
  */
  while(true)
  {
    shuffle_array(button_arr, button_count, dir);
    
    if(button_arr[0].hot_material)
    {
      break;
    }
  }
  
  return button_arr[0].entity;
}


Core::Entity_ref
navigate(const Core::Controller &controller,
         Image_button button_arr[],
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
  
  return navigate(dir, button_arr, button_count);
}


Core::Entity_ref
mouse_over(const Core::Camera &camera,
           const Core::World &world,
           const Core::Axis mouse_coords,
           Image_button button_arr[],
           const uint32_t button_count)
{
  assert(button_arr && button_count);
  
  /*
    Bail if invalid.
  */
  if(!button_arr || !button_count) {
    return Entity_ref();
  }

  const Core::Ray viewport_ray = Core::Camera_utils::get_ray_from_viewport(camera, mouse_coords);
  const Core::Entity_ref ent_from_ray = world.find_entity_by_ray(viewport_ray);

  /*
    Didn't get an entity so bail.
  */
  if(!ent_from_ray) {
    return Entity_ref();
  }
  
  /*
    We already have this entity as activated.
  */
  if(ent_from_ray == button_arr[0].entity) {
    return Entity_ref();
  }
  
  /*
    See if we can find the entity in our array.
  */
  int32_t shuffle_by = 0;
  
  for(int32_t i = 0; i < button_count; ++i)
  {
    const Image_button *button = &button_arr[i];
    assert(button);
  
    if(button->entity == ent_from_ray)
    {
      shuffle_by = i;
      break;
    }
  }
  
  // We want the top element the selected one.
  for(int j = 0; j < shuffle_by; ++j)
  {
    shuffle_array(button_arr, button_count, 1);
  }
  
  return Core::Entity_ref();
}


} // ns
} // ns
} // ns