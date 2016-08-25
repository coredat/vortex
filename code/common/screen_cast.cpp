#include <common/screen_cast.hpp>
#include <core/camera/camera.hpp>
#include <core/camera/camera_utils.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/transform/transform.hpp>
#include <core/common/plane.hpp>
#include <core/common/plane_utils.hpp>
#include <core/common/ray.hpp>
#include <core/input/axis.hpp>
#include <utilities/assert.hpp>


namespace Screen_cast {


math::vec3
intersect_screen_plane(const Core::Camera &cam,
                       const float ray_offset_x,
                       const float ray_offset_y)
{
  const math::vec3 cam_pos    = cam.get_attached_entity().get_transform().get_position();
  const math::vec3 cam_fwd    = cam.get_attached_entity().get_transform().get_forward();
  const math::vec3 scaled_fwd = math::vec3_scale(cam_fwd, 20.f);
  const math::vec3 plane_pos  = math::vec3_add(cam_pos, scaled_fwd);
  const math::vec3 plane_norm = math::vec3_scale(cam_fwd, -1.f);
  const Core::Plane plane(plane_pos, plane_norm);

  const Core::Ray ray      = Core::Camera_utils::get_ray_from_viewport(cam, {ray_offset_x, ray_offset_y});

  float out_distance = 0;
  const bool intersection = Core::Plane_utils::ray_intersects_with_plane(plane, ray, out_distance);
  ASSERT(intersection); // This ray should never miss.

  const math::vec3 scale_ray = math::vec3_scale(ray.get_direction(), out_distance);
  const math::vec3 point     = math::vec3_add(cam_pos, scale_ray);
  
  return point;
}


} // ns