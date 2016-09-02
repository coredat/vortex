#include <game_objects/main_camera.hpp>
#include <game_objects/player_ship.hpp>
#include <core/world/world.hpp>
#include <core/context/context.hpp>
#include <core/transform/transform.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/camera/camera_properties.hpp>
#include <core/transform/transform.hpp>
#include <core/color/color_predefined.hpp>
#include <core/resources/render_target.hpp>
#include <core/resources/texture.hpp>
#include <core/resources/shader.hpp>
#include <core/camera/post_process.hpp>
#include <core/common/directory.hpp>
#include <math/vec/vec3.hpp>
#include <math/vec/vec2.hpp>
#include <common/level_functions.hpp>
#include <common/object_tags.hpp>


namespace Game_object {


Main_camera::Main_camera(Core::World &world, Core::Context &ctx)
: Game_object(world)
, m_world_camera(world)
, m_level_camera(world)
, m_gui_camera(world)
, m_final_post_camera(world)
, m_gui_camera_entity(world)
{
  Core::Entity_ref ref = get_entity();
  
  ref.set_name("Main Camera");
  ref.add_tag(Object_tags::camera);
  Core::Transform curr_trans = ref.get_transform();
  curr_trans.set_position(math::vec3_init(0.f, 0.f, 12.f));
  ref.set_transform(curr_trans);
  
  Core::Render_target render_target(ctx.get_width(), ctx.get_height(), Graphics_api::Pixel_format::rgb8);
  Core::Render_target blur_target(ctx.get_width(), ctx.get_height(), Graphics_api::Pixel_format::rgb8);
  
  const char *shader_file = Core::Directory::volatile_resource_path("assets/shaders/vortex_level_post.ogl");
  Core::Shader post_shader(shader_file);
  
  const char *b_mat = Core::Directory::volatile_resource_path("assets/textures/bayer_mat.png");
  Core::Texture bayer(b_mat);
  
  Core::Post_process post("post-process");
  post.set_shader(post_shader);
  post.set_input_01(render_target.get_texture());
  
  m_world_camera.set_attached_entity(ref);
  m_world_camera.set_width(ctx.get_width());
  m_world_camera.set_height(ctx.get_height());
  m_world_camera.set_type(Core::Camera_type::perspective);
  m_world_camera.set_clear_flags(Core::Camera_clear::color | Core::Camera_clear::depth);
  m_world_camera.set_clear_color(0x222222FF);
  m_world_camera.set_tags_to_render(Object_tags::world_cam);
  m_world_camera.set_render_target(render_target);
  m_world_camera.set_priority(4);
 
  m_level_camera.set_attached_entity(ref);
  m_level_camera.set_width(m_world_camera.get_width());
  m_level_camera.set_height(m_world_camera.get_height());
  m_level_camera.set_feild_of_view(m_world_camera.get_field_of_view());
  m_level_camera.set_near_plane(m_world_camera.get_near_plane());
  m_level_camera.set_far_plane(m_world_camera.get_far_plane());
  m_level_camera.set_type(m_world_camera.get_type());
  m_level_camera.set_render_target(render_target);
  m_level_camera.set_clear_flags(0);
  m_level_camera.set_tags_to_render(Object_tags::level_cam);
  m_level_camera.set_priority(3);
  
  m_gui_camera.set_attached_entity(m_gui_camera_entity);
  m_gui_camera.set_type(Core::Camera_type::orthographic);
  m_gui_camera.set_width(m_world_camera.get_width());
  m_gui_camera.set_height(m_world_camera.get_height());
  m_gui_camera.set_near_plane(m_world_camera.get_near_plane());
  m_gui_camera.set_far_plane(m_world_camera.get_far_plane());
  m_gui_camera.set_clear_flags(Core::Camera_clear::depth);
  m_gui_camera.set_render_target(render_target);
  m_gui_camera.set_tags_to_render(Object_tags::gui_cam);
  m_gui_camera.set_priority(2);
  
  // Set post process.
  m_final_post_camera.set_attached_entity(ref);
  m_final_post_camera.set_width(m_world_camera.get_width());
  m_final_post_camera.set_height(m_world_camera.get_height());
  m_final_post_camera.set_feild_of_view(m_world_camera.get_field_of_view());
  m_final_post_camera.set_near_plane(m_world_camera.get_near_plane());
  m_final_post_camera.set_far_plane(m_world_camera.get_far_plane());
  m_final_post_camera.set_clear_flags(Core::Camera_clear::depth | Core::Camera_clear::color);
  m_final_post_camera.set_post_process(post);
  m_final_post_camera.set_priority(1);
}


void
Main_camera::on_start()
{
  
}


void
Main_camera::set_target_height(const float height)
{
  m_target_height = height;
}


void
Main_camera::set_target_speed(const float speed)
{
  m_target_speed = speed;
}


void
Main_camera::on_update(const float dt, World_objects &world_objs)
{
  Core::Entity_ref ref = get_entity();
  Core::World &world = get_world();
  
  Core::Entity_ref *players;
  size_t ent_size = 0;
  
  world.find_entities_by_tag(Object_tags::player, &players, &ent_size);

  const float camera_distance_far = m_target_height;
  constexpr float camera_distance_near = 10.f;
  
  const math::vec3 camera_origin = math::vec3_init(0,0,Level_funcs::get_top_of_level());
  math::vec3 avg_accum = math::vec3_init(0.f, 0.f, Level_funcs::get_top_of_level());
  math::vec3 accum_target = avg_accum;

  if(ent_size > 0)
  {
    // We set the camera origin as the first point.
    
    // Go through the players and accumulate the target point.
    for(uint32_t i = 0; i < ent_size; ++i)
    {
      auto &player = players[i];
      
      if(!player.is_valid())
      {
        continue;
      }
      
      // Get the difference and add it to the accum.
      const math::vec3 player_pos  = player.get_transform().get_position();
      const math::vec3 diff        = math::vec3_subtract(player_pos, math::vec3_zero());
      const math::vec3 scaled_diff = math::vec3_scale(diff, 0.5f);
      
      accum_target = math::vec3_add(accum_target, scaled_diff);
    }
    
    avg_accum = math::vec3_divide(accum_target, math::vec3_init(3.f));
  }
  
  // The closer the target point is to the camera origin the
  // further we pull back because this means the players
  // are at oposite ends of the level.
  math::vec3 pullback_distance = math::vec3_init(0,0,camera_distance_far);
  {
    const math::vec3 pullback_near   = math::vec3_init(0,0,camera_distance_near);
    const math::vec3 pullback_far    = math::vec3_init(0,0,camera_distance_far);
    const math::vec2 cam_origin_vec2 = math::vec2_init(math::vec3_get_x(camera_origin), math::vec3_get_y(camera_origin));
    const math::vec2 accum_vec2      = math::vec2_init(math::vec3_get_x(avg_accum), math::vec3_get_y(avg_accum));
    const float length               = math::abs(math::vec2_length(math::vec2_subtract(cam_origin_vec2, accum_vec2)));
    const float norm_length          = length / Level_funcs::get_radius();
    
    pullback_distance = math::vec3_lerp(pullback_far, pullback_near, norm_length);
  }
  
  // Adjust pullback for players jumping
  {
    const math::vec3 accum_jump = math::vec3_init(0,0,math::vec3_get_z(accum_target));
  
    pullback_distance = math::vec3_add(pullback_distance, accum_jump);
  }
  
  // New target point.
  Core::Transform this_trans = ref.get_transform();
  math::vec3 new_pos = math::vec3_zero();
  {
    constexpr float player_influence  = 1.f;
    const float camera_move_speed = m_target_speed;
    
    const math::vec3 scaled_accum = math::vec3_scale(avg_accum, player_influence);
    m_target_point = math::vec3_add(scaled_accum, pullback_distance);
    
    const math::vec3 this_pos   = this_trans.get_position();
    const math::vec3 move_dir   = math::vec3_subtract(m_target_point, this_pos);
    const math::vec3 scaled_dir = math::vec3_scale(move_dir, dt * camera_move_speed);
    
    new_pos = math::vec3_add(this_pos, scaled_dir);
  }
  
  this_trans.set_position(new_pos);
  
  ref.set_transform(this_trans);
}


} // ns