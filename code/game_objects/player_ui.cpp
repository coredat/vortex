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
  
  math::vec3 ui_ray_pos;
}



namespace Game_object {


Player_ui::Player_ui(Core::World &world,
                     Core::Context &ctx,
                     const Core::Camera &gui_cam,
                     const Core::Camera &world_cam,
                     const uint32_t controller_id)
: Game_object(world)
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
  
  // Some data
  const uint32_t screen_coords_count = 4;
  
  const math::vec3 direction[screen_coords_count]
  {
    math::vec3_init(+1.f, +1.f, 0.f),
    math::vec3_init(-1.f, +1.f, 0.f),
    math::vec3_init(+1.f, -1.f, 0.f),
    math::vec3_init(-1.f, -1.f, 0.f),
  };
  
  const float margin_x = ctx.get_width() * 0.05f;
  const float margin_y = ctx.get_height() * 0.05f;
  
  const math::vec3 screen_corners[screen_coords_count]
  {
    math::vec3_init(0.f + margin_x, 0.f + margin_y, 0.f),
    math::vec3_init(math::to_float(ctx.get_width()) - margin_x, 0.f + margin_y, 0.f),
    math::vec3_init(0.f + margin_x, math::to_float(ctx.get_height()) - margin_y, 0.f),
    math::vec3_init(math::to_float(ctx.get_width()) - margin_x, math::to_float(ctx.get_height()) - margin_y, 0.f),
  };
  
  const float texture_width = math::to_float(numbers[0].get_map_01().get_width()) * 0.5f;
  const float texture_height = math::to_float(numbers[0].get_map_01().get_height()) * 0.5f;
  const math::vec3 offset_size = math::vec3_init(texture_width, texture_height, 0.f);

  
  const uint32_t coord_index = controller_id - 1;
  assert(controller_id < screen_coords_count);
  
  const math::vec3 screen_corner_offset = math::vec3_multiply(offset_size, direction[coord_index]);
  const math::vec3 ray_pos = math::vec3_add(screen_corner_offset, screen_corners[coord_index]);
  
  ui_ray_pos = ray_pos;
  
  const Core::Ray ray = Core::Camera_utils::get_ray_from_viewport(gui_cam, Core::Axis{math::get_x(ray_pos), math::get_y(ray_pos)});
  const Core::Plane plane = Core::Camera_utils::get_near_plane(gui_cam);
  
  
  float out_distance = 0;
  const bool intersects = Core::Plane_utils::ray_intersects_with_plane(plane, ray, out_distance);
  assert(intersects); // should alwasy intersect
  
  const Core::Transform cam_trans = gui_cam.get_attached_entity().get_transform();
  const math::vec3 scale_fwd = math::vec3_scale(cam_trans.get_forward(), out_distance);
  const math::vec3 position  = math::vec3_add(ray.get_origin(), scale_fwd);
  
  
  // Set inital placement of the counters
  {
    float offset_x = 0;
    
    for(uint32_t i = 0; i < Units::size; ++i)
    {
      constexpr float scale = 3.f;
      constexpr float inv_scale = 1.f / scale;
    
      const Core::Transform trans_tens {
        math::vec3_add(position, math::vec3_init(offset_x, 0.f, 0.f)),
        math::vec3_init(texture_width * inv_scale),
        math::quat_init()
      };
      
      m_counters[i].set_transform(trans_tens);
      
      offset_x -= (texture_width * inv_scale);
    }
  }
  
  m_avatar = Core::Entity(world);
  m_avatar.set_tags(Object_tags::world_cam);
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
    // Some data
//    const uint32_t screen_coords_count = 4;
//    
//    const math::vec3 direction[screen_coords_count]
//    {
//      math::vec3_init(+1.f, +1.f, 0.f),
//      math::vec3_init(-1.f, +1.f, 0.f),
//      math::vec3_init(+1.f, -1.f, 0.f),
//      math::vec3_init(-1.f, -1.f, 0.f),
//    };
//    
//    const float margin_x = ctx.get_width() * 0.05f;
//    const float margin_y = ctx.get_height() * 0.05f;
//    
//    const math::vec3 screen_corners[screen_coords_count]
//    {
//      math::vec3_init(0.f + margin_x, 0.f + margin_y, 0.f),
//      math::vec3_init(math::to_float(ctx.get_width()) - margin_x, 0.f + margin_y, 0.f),
//      math::vec3_init(0.f + margin_x, math::to_float(ctx.get_height()) - margin_y, 0.f),
//      math::vec3_init(math::to_float(ctx.get_width()) - margin_x, math::to_float(ctx.get_height()) - margin_y, 0.f),
//    };
//
//    
//    const uint32_t coord_index = controller_id - 1;
//    assert(controller_id < screen_coords_count);
//    
//    const math::vec3 screen_corner_offset = math::vec3_multiply(offset_size, direction[coord_index]);
//    const math::vec3 ray_pos = math::vec3_add(screen_corner_offset, screen_corners[coord_index]);
  
    Core::Entity_ref *search_array = nullptr;
    size_t size_of_find = 0;
    get_world().find_entities_by_tag(Object_tags::camera, &search_array, &size_of_find);
    assert(size_of_find && search_array);
    
    Main_camera *camera = reinterpret_cast<Main_camera*>((void*)search_array[0].get_user_data());
    assert(camera);
    
    const Core::Ray ray2 = Core::Camera_utils::get_ray_from_viewport(camera->m_world_camera, Core::Axis{0,0});
    const Core::Plane plane2 = Core::Camera_utils::get_near_plane(camera->m_world_camera);
    const Core::Plane plane(math::vec3_zero(), math::vec3_scale(camera->m_world_camera.get_attached_entity().get_transform().get_forward(), -1.f));
    
    float dist = 0;
    const bool intersects = Core::Plane_utils::ray_intersects_with_plane(plane, ray2, dist);
    assert(intersects);
    
    const Core::Transform cam_trans2 = camera->m_world_camera.get_attached_entity().get_transform();
    
    const math::vec3 scale_fwd2 = math::vec3_scale(cam_trans2.get_forward(), dist * 1);
    const math::vec3 pos2  = math::vec3_add(ray2.get_origin(), scale_fwd2);
    
    Core::Transform avatar_trans = m_avatar.get_transform();
    avatar_trans.set_position(pos2);
    
    m_avatar.set_transform(avatar_trans);
  }
}


void
Player_ui::set_score(const uint32_t score)
{
  m_score = score;
}


} // ns