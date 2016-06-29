#include <common/screen_cast.hpp>
#include <core/camera/camera.hpp>
#include <core/camera/camera_utils.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/transform/transform.hpp>


namespace Screen_cast {


Core::Transform
intersect_screen_plane(const Core::Camera &cam)
{
  auto intersect_plane = [](const math::vec3 plane_normal,
                            const math::vec3 plane_position,
                            const math::vec3 ray_start,
                            const math::vec3 ray_dir,
                            float &time) -> bool
  {
    // assuming vectors are all normalized
    float denom = math::vec3_dot(plane_normal, ray_dir);
    if (denom > 1e-6)
    {
      math::vec3 p0l0 = math::vec3_subtract(plane_position, ray_start);
      time = math::vec3_dot(p0l0, plane_normal) / denom;
      return (time >= 0);
    } 
 
    return false; 
  };
  
  const math::mat4 proj   = Core::Camera_utils::camera_get_projection_matrix(cam);
  const math::mat4 view   = Core::Camera_utils::camera_get_view_matrix(cam);
  const math::mat4 inv_vp = math::mat4_get_inverse(math::mat4_multiply(proj, view));
  
  const float offset = 0.f;
  
  const math::vec4 screen_pos = math::vec4_init(offset, 0.f, -1.f, 1.f);
  const math::vec4 world_pos  = math::mat4_multiply(screen_pos, inv_vp);
  const math::vec3 world_pos3 = math::vec3_init(math::vec4_get_x(world_pos),
                                                math::vec4_get_y(world_pos),
                                                math::vec4_get_z(world_pos));
  const math::vec3 dir        = math::vec3_normalize(world_pos3);
  
  const Core::Transform cam_trans = cam.get_attached_entity().get_transform();
  
  const math::vec3 fwd = math::vec3_scale(cam_trans.get_forward(), 0.2f);
  const math::vec3 plane_pos = math::vec3_add(cam_trans.get_position(), fwd);
  
  float time;
  const bool did_intersect = intersect_plane(math::vec3_init(0, 0, -1),
                                             plane_pos,
                                             cam_trans.get_position(),
                                             dir,
                                             time);
  
  if(did_intersect)
  {
    const math::vec3 final_scale = math::vec3_scale(dir, time);
    math::vec3 final_pos = math::vec3_add(cam_trans.get_position(), final_scale);

    // Place card
    Core::Transform trans;
    trans.set_position(math::vec3_init(math::vec3_get_x(final_pos),
                                       math::vec3_get_y(final_pos),
                                       math::vec3_get_z(final_pos)));
    constexpr float scale = 0.05f;
    trans.set_scale(math::vec3_init(scale, 1, scale));
    trans.set_rotation(math::quat_init_with_axis_angle(1, 0, 0, -math::quart_tau()));
    
    return trans;
  }
  
  return Core::Transform();
}


} // ns