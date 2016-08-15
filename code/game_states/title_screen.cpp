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
    Core::Model model(Core::Directory::volatile_resource_path("assets/models/unit_cube.obj"));
    assert(model);
    
    Core::Material_renderer mat_renderer;
    mat_renderer.set_material(title_material);
    mat_renderer.set_model(model);
  
    title_screen = Core::Entity(world);
    
    Core::Rigidbody rb;
//    rb.set_collider(Core::Box_collider(1.15, 0.5, 0.3));
    rb.set_collider(Core::Box_collider(2,2,2));
    rb.set_is_dynamic(false);
    rb.set_is_trigger(true);
    title_screen.set_rigidbody(rb);
    
    title_screen.set_name("title_screen[title]");
    title_screen.set_tags(Object_tags::gui_cam);
    title_screen.set_renderer(mat_renderer);
    title_screen.set_transform(Core::Transform(
      math::vec3_zero(),
      math::vec3_init(100, 100, 0.5),
      math::quat()
//      math::quat_init_with_axis_angle(Core::Transform::get_world_left(), 0.f)//-math::quart_tau())
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
    Core::Axis mouse_coords = Core::Input::mouse_get_coordinates(ctx);
    mouse_coords.x = (2.f * mouse_coords.x) / ctx.get_width() - 1.f;
    mouse_coords.y = 1.f - (2.f * mouse_coords.y) / ctx.get_height();
    
    const math::vec4 mouse_clip = math::vec4_init(mouse_coords.x, mouse_coords.y, 0.f, 1.f);

//    mouse_worldspace = inverse(C) * inverse(P) * mouse_clip
    const math::mat4 world_mat = Core::Transform_utils::get_world_matrix(camera.get_attached_entity().get_transform());
    const math::mat4 world_inv_mat = math::mat4_get_inverse(world_mat);
    
    const math::mat4 proj_mat = Core::Camera_utils::camera_get_projection_matrix(camera);
    const math::mat4 proj_inv_mat = math::mat4_get_inverse(proj_mat);
    
    const math::mat4 world_proj_inv_mat = math::mat4_multiply(world_inv_mat, proj_inv_mat);
    const math::vec4 mouse_worldspace = math::mat4_multiply(mouse_clip, world_proj_inv_mat);
    
//    const math::vec4 ray_dir = math::vec4_normalize(mouse_worldspace);
//    const math::vec3 norm_dir = math::vec3_init(math::get_x(ray_dir), math::get_y(ray_dir), math::get_z(ray_dir));
  
    const math::vec3 cam_pos = camera.get_attached_entity().get_transform().get_position();
    const math::vec3 mouse_worldspace_norm = math::vec3_normalize(math::vec3_init(math::get_x(mouse_worldspace), math::get_y(mouse_worldspace), math::get_z(mouse_worldspace)));
  
    const math::vec3 mouse_pos = math::vec3_normalize(math::vec3_subtract(mouse_worldspace_norm, cam_pos));

    Core::Ray ray(world, cam_pos, mouse_worldspace_norm);

  
//    printf("Cam pos %f, %f, %f \n", math::get_x(cam_pos), math::get_y(cam_pos), math::get_z(cam_pos));
//    printf("Ray ori %f, %f, %f \n", math::get_x(ray_start), math::get_y(ray_start), math::get_z(ray_start));
//    printf("Ray dir %f, %f, %f \n", math::get_x(ray_dir), math::get_y(ray_dir), math::get_z(ray_dir));
    
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