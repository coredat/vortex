#ifndef SCREEN_CAST_INCLUDED_F8D69308_B65B_4952_8D6C_0D2B2463A0A1
#define SCREEN_CAST_INCLUDED_F8D69308_B65B_4952_8D6C_0D2B2463A0A1


#include <core/common/core_fwd.hpp>


namespace Screen_cast {


/*
  Gets the position of a ray from the screen to a
  plane in the world.
*/
Core::Transform
intersect_screen_plane(const Core::Camera &cam,
                       const float ray_offset_x = 0,
                       const float ray_offset_y = 0);


} // ns


#endif // inc guard