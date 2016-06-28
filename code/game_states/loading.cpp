#include <game_states/loading.hpp>
#include <core/context/context.hpp>
#include <core/world/world.hpp>
#include <core/resources/texture.hpp>
#include <core/model/model.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/camera/camera.hpp>
#include <core/camera/camera_utils.hpp>
#include <core/resources/material.hpp>
#include <core/resources/shader.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/renderer.hpp>
#include <core/transform/transform.hpp>
#include <utilities/directory.hpp>
#include <math/quat/quat.hpp>
#include <utilities/logging.hpp>


namespace
{
  Core::Texture     texture;
  Core::Model       model;
  Core::Entity      loading_entity;
  
  const float       max_timer = 10.f;  // Min time loading screen stays up
  float             curr_timer = 0.f;
}


void
loading_init(Core::Context &context, Core::World &world)
{
  LOG_TODO("This needs some fixing.");

  curr_timer = 0;
  
  const std::string asset_path = util::get_resource_path() + "assets/";
  
  if(!texture)
  {
    const std::string tex_01 = asset_path + "textures/ship_04.png";
    texture = Core::Texture(tex_01.c_str());
  }
  
  if(!model)
  {
    const std::string model_01 = asset_path + "models/unit_plane.obj";
    model = Core::Model(model_01.c_str());
  }
  
  Core::Material material = Core::Material("start-logo");
  
  const std::string orange_texture_path = util::get_resource_path() + "assets/textures/dev_grid_orange_512.png";
  Core::Texture texture(Core::Texture(orange_texture_path.c_str()));
  
  const std::string shader_path = util::get_resource_path() + "assets/shaders/basic_fullbright.ogl";
  Core::Shader shader(shader_path.c_str());
  
  material.set_shader(shader);
  material.set_map_01(texture);
  
  Core::Material_renderer mat_renderer;
  mat_renderer.set_model(model);
  mat_renderer.set_material(material);
  
  if(!loading_entity)
  {
    loading_entity = Core::Entity(world);
    loading_entity.set_name("Loading Screen Logo");
    
    // Rotate towards the screen.
    {
      math::quat rot = math::quat_init_with_axis_angle(1, 0, 0, math::quart_tau());
      
      Core::Transform trans;
      trans.set_rotation(rot);
      loading_entity.set_renderer(mat_renderer);
      loading_entity.set_transform(trans);
    }
  }
}


Game_state
loading_update(Core::Context &context,
               Core::World &world,
               Core::Camera &cam)
{
  curr_timer += world.get_delta_time();
  
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
  
  math::mat4 proj   = Core::Camera_utils::camera_get_projection_matrix(cam);
  math::mat4 view   = Core::Camera_utils::camera_get_view_matrix(cam);
  math::mat4 inv_vp = math::mat4_get_inverse(math::mat4_multiply(proj, view));
  
  const float offset = -3.f + (1 * 2.f);
  
  math::vec4 screen_pos = math::vec4_init(offset, 0.f, -1.f, 1.f);
  math::vec4 world_pos  = math::mat4_multiply(screen_pos, inv_vp);      
  math::vec3 world_pos3 = math::vec3_init(math::vec4_get_x(world_pos), math::vec4_get_y(world_pos), math::vec4_get_z(world_pos));
  math::vec3 dir        = math::vec3_normalize(world_pos3);
  
  const Core::Transform cam_trans = cam.get_attached_entity().get_transform();
  
  const math::vec3 fwd = math::vec3_scale(cam_trans.get_forward(), 0.2);
  const math::vec3 plane_pos = math::vec3_add(cam_trans.get_position(), fwd);
  
  float time;
  const bool did_intersect = intersect_plane(math::vec3_init(0, 0, -1),
                                             plane_pos,
                                             cam_trans.get_position(),
                                             dir,
                                             time);
  
  if(did_intersect)
  {
    math::vec3 final_scale = math::vec3_scale(dir, time);
    math::vec3 final_pos = math::vec3_add(cam_trans.get_position(), final_scale);

    // Place card
    Core::Transform trans;
    trans.set_position(math::vec3_init(math::vec3_get_x(final_pos), math::vec3_get_y(final_pos), math::vec3_get_z(final_pos)));
    constexpr float scale = 1;
    trans.set_scale(math::vec3_init(scale, 1, scale));
    trans.set_rotation(math::quat_init_with_axis_angle(1, 0, 0, -math::quart_tau()));
    
    loading_entity.set_transform(trans);
  }
  
  if(curr_timer > max_timer)
  {
    return Game_state::title_screen;
  }
  
  return Game_state::loading;
}