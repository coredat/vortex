#include <common/level_functions.hpp>
#include <math/vec/vec2.hpp>


namespace Level {


math::vec2
get_point_on_cirlce(const float a)
{
  const float radius = get_radius();
  const float origin_x = 0.f;
  const float origin_y = 0.f;
  
  const float x = origin_x + radius * math::cos(a);
  const float y = origin_y + radius * math::sin(a);
  
  return math::vec2_init(x, y);
}


} // ns