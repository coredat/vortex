#include <game_states/title_screen.hpp>
#include <common/game_state.hpp>
#include <common/object_tags.hpp>
#include <core/context/context.hpp>
#include <core/world/world.hpp>
#include <core/resources/material.hpp>
#include <core/resources/texture.hpp>
#include <core/model/model.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/resources/shader.hpp>
#include <core/transform/transform.hpp>
#include <core/transform/transform_utils.hpp>
#include <core/common/directory.hpp>
#include <core/input/controller.hpp>
#include <core/input/buttons.hpp>
#include <core/physics/ray.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/box_collider.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/text_renderer.hpp>
#include <core/font/font.hpp>
#include <core/camera/camera.hpp>
#include <core/camera/camera_utils.hpp>
#include <utilities/directory.hpp>
#include <math/quat/quat.hpp>
#include <utilities/logging.hpp>
#include <assert.h>


namespace
{
  Core::Entity    title_screen;
  Core::Entity    title_text;
  Core::Material  title_material;
}


void
title_screen_init(Core::Context &ctx,
                  Core::World &world,
                  Core::Camera &camera)
{
  if(!title_material)
  {
    Core::Shader shader(Core::Directory::volatile_resource_path("assets/shaders/basic_fullbright.ogl"));
    assert(shader);
    
    Core::Texture texture(Core::Directory::volatile_resource_path("assets/textures/dev_grid_orange_512.png"));
    assert(texture);
    
    title_material = Core::Material("title-banner");
    title_material.set_shader(shader);
    title_material.set_map_01(texture);
  }

  if(!title_screen)
  {
    Core::Model model(Core::Directory::volatile_resource_path("assets/models/unit_plane.obj"));
    assert(model);
    
    Core::Material_renderer mat_renderer;
    mat_renderer.set_material(title_material);
    mat_renderer.set_model(model);
  
    title_screen = Core::Entity(world);
    
    Core::Rigidbody rb;
    rb.set_collider(Core::Box_collider(1,1,1));
    rb.set_is_dynamic(false);
    
    title_screen.set_rigidbody(rb);
    
    title_screen.set_name("title_screen[title]");
    title_screen.set_tags(Object_tags::gui_cam);
    title_screen.set_renderer(mat_renderer);
    title_screen.set_transform(Core::Transform(
      math::vec3_zero(),
      math::vec3_init(512.f, 1.f, 128.f),
      math::quat_init_with_axis_angle(Core::Transform::get_world_left(), -math::quart_tau())
    ));
    

  }
  
  if(!title_text)
  {
    title_text = Core::Entity(world);
    title_text.set_name("Test text entity");
    title_text.set_tags(Object_tags::level_cam);
    
    Core::Text_renderer text_renderer;
    Core::Font font("/Users/PhilCK/Desktop/font/LiberationSerif-Bold.ttf");
    {
      text_renderer.set_font(font);
      text_renderer.set_text("Vortex Defender");
    }
    
    title_text.set_renderer(text_renderer);
    
    Core::Transform text_trans;
    text_trans.set_position(math::vec3_init(0, 200, 0));
    
    title_text.set_transform(text_trans);
  }
  
}


Game_state
title_screen_update(Core::Context &ctx,
                    Core::World &world,
                    Core::Camera &camera,
                    Game_object::World_objects &objects,
                    const float dt)
{
  constexpr uint32_t number_of_controllers = 4;
  
  const Core::Controller controllers[number_of_controllers]
  {
    Core::Controller(ctx, 0),
    Core::Controller(ctx, 1),
    Core::Controller(ctx, 2),
    Core::Controller(ctx, 3),
  };
  
  title_screen.on_mouse_over([](Core::Entity_ref ref)
                             {
                               int i = 0;
                             });
  
  // Mouse Pick Test
  {
    // Norm coords
    Core::Axis mouse = Core::Input::mouse_get_coordinates(ctx);
//    mouse.x = (mouse.x / (ctx.get_width() * 0.5f)) - 1.f;
//    mouse.y = (mouse.y / (ctx.get_height() * 0.5f)) - 1.f;
    mouse.x = (2.f * mouse.x) / ctx.get_width() - 1.f;
    mouse.y = 1.f - (2.f * mouse.y) / ctx.get_height();
    
    
    const math::mat4 world_mat = Core::Transform_utils::get_world_matrix(title_screen.get_transform());
    const math::mat4 view_mat = Core::Camera_utils::camera_get_view_matrix(camera);
    const math::mat4 proj_mat = Core::Camera_utils::camera_get_projection_matrix(camera);
    const math::mat4 proj_view_mat = math::mat4_multiply(view_mat, proj_mat);
    
    const math::vec4 mouse_clip_pos = math::mat4_multiply(math::vec4_init(mouse.x, mouse.y, 0, 1), proj_view_mat);
    const math::vec4 mouse_world_pos =
    
    
    const math::mat4 wvp_mat = math::mat4_multiply(world_mat, view_mat, proj_mat);
    const math::mat4 wvp_inv_mat = math::mat4_get_inverse(wvp_mat);
    
    const math::vec4 near = math::mat4_multiply(math::vec4_init(mouse.x, mouse.y, 0, 1.f), wvp_inv_mat);
    const math::vec4 ray_start = math::vec4_init(math::mat4_get(wvp_inv_mat, 12),
                                                 math::mat4_get(wvp_inv_mat, 13),
                                                 math::mat4_get(wvp_inv_mat, 14),
                                                 math::mat4_get(wvp_inv_mat, 15));
    
    const math::vec4 ray_dir_unnorm = math::vec4_subtract(near, math::vec4_normalize(ray_start));
    const math::vec3 ray_dir = math::vec3_normalize(math::vec3_init(math::get_x(ray_dir_unnorm), math::get_y(ray_dir_unnorm), math::get_z(ray_dir_unnorm)));

    
    Core::Ray ray(world, math::vec3_init(math::get_x(ray_start), math::get_y(ray_start), math::get_z(ray_start)), ray_dir);

  
//    printf("Cam pos %f, %f, %f \n", math::get_x(cam_pos), math::get_y(cam_pos), math::get_z(cam_pos));
    printf("Ray ori %f, %f, %f \n", math::get_x(ray_start), math::get_y(ray_start), math::get_z(ray_start));
    printf("Ray dir %f, %f, %f \n", math::get_x(ray_dir), math::get_y(ray_dir), math::get_z(ray_dir));
    
    if(ray.has_hit())
    {
      auto mat = Core::Renderer_utils::cast_to_material_renderer(title_screen.get_renderer()).get_material();
      mat.set_map_01(Core::Texture(Core::Directory::volatile_resource_path("assets/textures/dev_grid_red_512.png")));
    }
    else
    {
      auto mat = Core::Renderer_utils::cast_to_material_renderer(title_screen.get_renderer()).get_material();
      mat.set_map_01(Core::Texture(Core::Directory::volatile_resource_path("assets/textures/dev_grid_orange_512.png")));
    }
    
    // Cast ray to the aabb
    
  }
  
  for(const auto &ctrl : controllers)
  {
    if(ctrl.is_button_up_on_frame(Core::Gamepad_button::button_start))
    {
      title_screen.destroy();
      title_text.destroy();
      return Game_state::selection;
    }
  }

  return Game_state::title_screen;
}