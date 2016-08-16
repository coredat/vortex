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
#include <core/camera/camera_properties.hpp>
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
#include <renderer/debug_line.hpp>
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
    rb.set_collider(Core::Box_collider(0.5, 0.5, 0.5));
    rb.set_is_dynamic(false);
    rb.set_is_trigger(true);
    title_screen.set_name("title_screen[title]");
    title_screen.set_tags(Object_tags::gui_cam);
    title_screen.set_renderer(mat_renderer);
    title_screen.set_transform(Core::Transform(
      math::vec3_zero(),
      math::vec3_init(512, 128, 10),
      math::quat()
    ));
    
    title_screen.set_rigidbody(rb);
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

    const Core::Ray viewport_ray = Core::Camera_utils::viewport_to_ray(camera,
                                                                       world,
                                                                       Core::Input::mouse_get_coordinates(ctx),
                                                                       Core::Axis{math::to_float(ctx.get_width()),
                                                                                  math::to_float(ctx.get_height())
                                                                                  });

    if(viewport_ray.has_hit())
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