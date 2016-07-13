#include <game_states/title_screen.hpp>
#include <common/game_state.hpp>
#include <common/object_tags.hpp>
#include <core/context/context.hpp>
#include <core/world/world.hpp>
#include <core/resources/material.hpp>
#include <core/resources/texture.hpp>
#include <core/model/model.hpp>
#include <core/entity/entity.hpp>
#include <core/resources/shader.hpp>
#include <core/transform/transform.hpp>
#include <core/common/directory.hpp>
#include <core/input/controller.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <utilities/directory.hpp>
#include <math/quat/quat.hpp>
#include <utilities/logging.hpp>
#include <assert.h>


namespace
{
  Core::Entity title_screen;
  Core::Material title_material;
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
    title_screen.set_name("title_screen[title]");
    title_screen.set_tags(Object_tags::gui_cam);
    title_screen.set_renderer(mat_renderer);
    title_screen.set_transform(Core::Transform(
      math::vec3_zero(),
      math::vec3_init(512.f, 1.f, 128.f),
      math::quat_init_with_axis_angle(Core::Transform::get_world_left(), -math::quart_tau())
    ));
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

  for(const auto &ctrl : controllers)
  {
    if(ctrl.is_button_up_on_frame(Core::Button::button_4))
    {
      title_screen.destroy();
      return Game_state::selection;
    }
  }

  return Game_state::title_screen;
}