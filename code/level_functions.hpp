#ifndef LEVEL_FUNCTIONS_INCLUDED_D0B01A8A_FC00_4F3C_858A_18ED3C51D6D8
#define LEVEL_FUNCTIONS_INCLUDED_D0B01A8A_FC00_4F3C_858A_18ED3C51D6D8


#include <math/vec/vec_fwd.hpp>


namespace Level {


math::vec2
get_point_on_cirlce(const float a);


constexpr float
get_radius() { return 5.f; }


constexpr float
get_bottom_of_level() { return -30.f; }


constexpr float
get_top_of_level() { return 0.f; }


constexpr float
get_far_death_zone() { return get_bottom_of_level() * 2; }


constexpr float
get_near_death_zones() { return 10.f; }


} // ns



#endif // inc guard