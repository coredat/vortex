#ifndef MENU_IMAGE_LIST_INCLUDED_D364E4E4_29D9_4EC0_8B1D_567D60C47D67
#define MENU_IMAGE_LIST_INCLUDED_D364E4E4_29D9_4EC0_8B1D_567D60C47D67


#include <core/common/core_fwd.hpp>
#include <core/entity/entity.hpp>
#include <core/resources/material.hpp>


namespace Core {
namespace Lib {
namespace Menu_list {


struct Image_button
{
  Core::Entity    entity;
  Core::Material  hot_material;
  Core::Material  cold_material;
  
  Image_button()
  {
  }
  
  Image_button(Image_button &&other)
  {
    entity = std::move(other.entity);
    hot_material = other.hot_material;
    cold_material = other.cold_material;
  }
  
  Image_button &operator=(Image_button &&other)
  {
    entity = std::move(other.entity);
    hot_material = other.hot_material;
    cold_material = other.cold_material;
    
    return *this;
  }
  
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
  Align buttons onto a plane
*/
void
align_to_camera(const Core::Camera &camera,
                const Image_button *buttons_arr,
                const uint32_t button_count);


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
mouse_over(const Image_button *button_arr,
           const uint32_t button_count);


} // ns
} // ns
} // ns


#endif // inc guard