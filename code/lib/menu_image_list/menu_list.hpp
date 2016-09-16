#ifndef MENU_IMAGE_LIST_INCLUDED_D364E4E4_29D9_4EC0_8B1D_567D60C47D67
#define MENU_IMAGE_LIST_INCLUDED_D364E4E4_29D9_4EC0_8B1D_567D60C47D67


#include <core/common/core_fwd.hpp>
#include <core/entity/entity.hpp>
#include <core/resources/material.hpp>


namespace Core {
namespace Lib {
namespace Menu_list {


/*
  Useage
  --
  Keep an array of buttons, these methods will keep arrange it
  so that the first element in the array is the active element.
  
  Its up to the calling code to know when a button has been activated.
*/
struct Image_button
{
  Core::Entity    entity;
  Core::Material  hot_material;
  Core::Material  cold_material;

  Image_button()                                = default;
  Image_button(Image_button &&)                 = default;
  Image_button(const Image_button &)            = default;
  Image_button &operator=(Image_button &&)      = default;
  Image_button &operator=(const Image_button &) = default;
};


/*
  Helper method to generate the material renderers for the buttons.
  It will apply the cold material to all the buttons.
*/
void
inititalize(Image_button *buttons_arr,
            const uint32_t button_count,
            const Core::Model &model,
            const Core::Camera &camera);


/*
  Will return a valid entity if menu item was confirmed on.
  This will also shuffle the array with the current selected item at the front.
*/
Core::Entity_ref
navigate(const Core::Controller &controller,
         Image_button *button_arr,
         const uint32_t button_count);


/*
  Will return a valid entity if the menu item was clicked on.
*/
Core::Entity_ref
mouse_over(const Core::Camera &camera,
           const Core::World &world,
           const Core::Axis mouse_coords,
           Image_button *button_arr,
           const uint32_t button_count);


} // ns
} // ns
} // ns


#endif // inc guard