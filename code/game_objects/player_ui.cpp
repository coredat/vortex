#include <game_objects/player_ui.hpp>
#include <game_objects/main_camera.hpp>
#include <common/screen_cast.hpp>
#include <common/object_tags.hpp>
#include <core/resources/material.hpp>
#include <core/resources/texture.hpp>
#include <core/world/world.hpp>
#include <core/model/model.hpp>
#include <core/resources/shader.hpp>
#include <core/common/directory.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/transform/transform.hpp>
#include <core/camera/camera.hpp>
#include <core/camera/camera_utils.hpp>
#include <core/common/ray.hpp>
#include <core/common/plane.hpp>
#include <core/context/context.hpp>
#include <core/common/plane_utils.hpp>
#include <core/input/axis.hpp>


namespace
{
  Core::Material numbers[10];
  Core::Model model;
  
  float spin = 0.f;
}


namespace Game_object {


namespace {


inline math::vec3
get_position(const Core::Context &ctx, const uint32_t player, const Player_ui::Ui_type type)
{
  const uint32_t screen_coords_count = 4;
  
  if(type == Player_ui::Ui_type::in_game_ui)
  {
    const float margin_x = ctx.get_width() * 0.1f;
    const float margin_y = ctx.get_height() * 0.2f;
    
    const math::vec3 screen_corners[screen_coords_count]
    {
      math::vec3_init(0.f + margin_x, 0.f + margin_y, 0.f),
      math::vec3_init(math::to_float(ctx.get_width()) - margin_x, 0.f + margin_y, 0.f),
      math::vec3_init(0.f + margin_x, math::to_float(ctx.get_height()) - margin_y, 0.f),
      math::vec3_init(math::to_float(ctx.get_width()) - margin_x, math::to_float(ctx.get_height()) - margin_y, 0.f),
    };
    
    return screen_corners[player];
  }
  else
  {
    const float steps = math::to_float(ctx.get_width()) / 5.f;
    const float offset_x = steps + (steps * player);
    const float offset_y = math::to_float(ctx.get_height()) / 3.f;
    
    return math::vec3_init(offset_x, offset_y, 0.f);
  }
}


}


Player_ui::Player_ui(Core::World &world,
                     Core::Context &ctx,
                     const Core::Camera &gui_cam,
                     const Core::Camera &world_cam,
                     const uint32_t controller_id,
                     const Ui_type ui_type)
: Game_object(world)
, m_corner(controller_id - 1)
{
  const Core::Shader num_shader(Core::Directory::volatile_resource_path("assets/shaders/basic_fullbright.ogl"));

  // Load up numbers for the first run
  if(!numbers[0])
  {
    char buffer[2048];
  
    for(uint32_t i = 0; i < 10; ++i)
    {
      memset(buffer, 0, sizeof(char) * 2048);
      sprintf(buffer, "%s/num_%d.png", Core::Directory::volatile_resource_path("assets/textures"), i);
      
      Core::Texture texture(buffer);
      assert(texture);
      
      char mat_name[128];
      memset(mat_name, 0, sizeof(char) * 128);
      sprintf(mat_name, "[player_ui]number_%d", i);
      
      Core::Material material(mat_name);
      assert(material);
      
      material.set_shader(num_shader);
      material.set_map_01(texture);
      
      numbers[i] = material;
    }
  }
  
  // Load up the plane model.
  if(!model)
  {
    const char * unit_cube_path = Core::Directory::volatile_resource_path("assets/models/unit_cube.obj");
    model = Core::Model(unit_cube_path);
    assert(model);
  }
  
  // setup base entity
  {
    auto ref = get_entity();

    ref.set_name("player_ui");
  }
  
  // Create counter entitiets
  // These actually render the numbers
  for(auto &counter : m_counters)
  {
    counter = Core::Entity(world);
    counter.set_name("[player_ui]counter");
    counter.set_tags(Object_tags::gui_cam);
  }

  const Core::Material_renderer mat_renderer(numbers[0], model);
  
  m_screen_pos = get_position(ctx, controller_id - 1, ui_type);
  
//  const math::vec3 position = Screen_cast::intersect_screen_plane(gui_cam, math::get_x(screen_pos), math::get_y(screen_pos));
  const math::vec3 position = Core::Camera_utils::get_world_position_on_nearplane(gui_cam, Core::Axis{math::get_x(m_screen_pos), math::get_y(m_screen_pos) + 30.f});
  const float texture_width = math::to_float(numbers[0].get_map_01().get_width()) * 0.5f;
  
  // Set inital placement of the counters
  {
    float offset_x = 0;
    
    for(uint32_t i = 0; i < Units::size; ++i)
    {
      constexpr float scale = 3.f;
      constexpr float inv_scale = 1.f / scale;
    
      const Core::Transform trans {
        math::vec3_add(position, math::vec3_init(offset_x, 0.f, 0.f)),
        math::vec3_init(texture_width * inv_scale),
        math::quat_init()
      };
      
      m_counters[i].set_transform(trans);
      
      offset_x -= (texture_width * inv_scale);
    }
  }
  
  m_avatar = Core::Entity(world);
  m_avatar.set_tags(Object_tags::world_cam);
  
  // Dumb copy and paste
  // to stop flicker, Phil this code is dying!
  {
    Core::Entity_ref *search_array = nullptr;
    size_t size_of_find = 0;
    get_world().find_entities_by_tag(Object_tags::camera, &search_array, &size_of_find);
    assert(size_of_find && search_array);
    
    
    Main_camera *camera = reinterpret_cast<Main_camera*>((void*)search_array[0].get_user_data());
    assert(camera);
    
    const Core::Ray ray2 = Core::Camera_utils::get_ray_from_viewport(camera->m_world_camera, Core::Axis{0,0});
    const Core::Plane plane = Core::Camera_utils::get_near_plane(camera->m_world_camera);
    
//    const Core::Plane plane(math::vec3_init(0, 0, 10), math::vec3_scale(camera->m_world_camera.get_attached_entity().get_transform().get_forward(), -1.f));
    
    float dist = 0;
    const bool intersects = Core::Plane_utils::ray_intersects_with_plane(plane, ray2, dist);
    assert(intersects);
    
    auto point = Screen_cast::intersect_screen_plane(camera->m_world_camera, math::get_x(m_screen_pos), math::get_y(m_screen_pos));
    
    Core::Transform avatar_trans = m_avatar.get_transform();
    avatar_trans.set_position(point);
    avatar_trans.set_scale(math::vec3_init(0.7f));
    
    m_avatar.set_transform(avatar_trans);
  }
}


void
Player_ui::on_start()
{
}


void
Player_ui::on_end()
{
  for(auto &count : m_counters)
  {
    if(count)
    {
      count.destroy();
    }
  }
}


namespace {

void
update_renderer(Core::Entity_ref ref,
                const uint32_t index)
{
  assert(index < 10);
  assert(ref);
  
  Core::Material_renderer renderer;
  renderer.set_material(numbers[index]);
  renderer.set_model(model);
  
  ref.set_renderer(renderer);
}

} // anon ns


void
Player_ui::on_update(const float dt, World_objects &objs)
{
  // Calculate the individual units
  const uint32_t hundreds = (m_score / 100) % 10;
  const uint32_t tens = (m_score / 10) % 10;
  const uint32_t ones = m_score % 10;
  
  update_renderer(m_counters[Units::single], ones);
  
  if(m_score >= 10)
  {
    update_renderer(m_counters[Units::tens], tens);
  }
  
  if(m_score >= 100)
  {
    update_renderer(m_counters[Units::hundreds], hundreds);
  }
  
  // Update avatar
  {
    Core::Entity_ref *search_array = nullptr;
    size_t size_of_find = 0;
    get_world().find_entities_by_tag(Object_tags::camera, &search_array, &size_of_find);
    assert(size_of_find && search_array);
    
    Main_camera *camera = reinterpret_cast<Main_camera*>((void*)search_array[0].get_user_data());
    assert(camera);
    
    const Core::Ray ray2 = Core::Camera_utils::get_ray_from_viewport(camera->m_world_camera, Core::Axis{0,0});
    const Core::Plane plane = Core::Camera_utils::get_near_plane(camera->m_world_camera);
    
//    const Core::Plane plane(math::vec3_init(0, 0, 10), math::vec3_scale(camera->m_world_camera.get_attached_entity().get_transform().get_forward(), -1.f));
    
    float dist = 0;
    const bool intersects = Core::Plane_utils::ray_intersects_with_plane(plane, ray2, dist);
    assert(intersects);
    
    auto point = Screen_cast::intersect_screen_plane(camera->m_world_camera, math::get_x(m_screen_pos), math::get_y(m_screen_pos));
    
    Core::Transform avatar_trans = m_avatar.get_transform();
    avatar_trans.set_position(point);
    avatar_trans.set_scale(math::vec3_init(1.7f));
    
    m_avatar.set_transform(avatar_trans);
  }
  
  spin += dt;
  
  const math::quat rot_side = math::quat_init_with_axis_angle(0, 1, 0, spin * 0.5f);
  const math::quat rot_down = math::quat_init_with_axis_angle(0, 0, 1, -0.15f);
  const math::quat rot = math::quat_multiply(rot_down, rot_side);
  
  Core::Transform trans = m_avatar.get_transform();
  trans.set_rotation(rot);
  auto old_pos = trans.get_position();
  
  m_avatar.set_transform(trans);
}


void
Player_ui::set_score(const uint32_t score)
{
  m_score = score;
}


} // ns