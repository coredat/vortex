#include <game_states/loading.hpp>
#include <game_objects/world_objects.hpp>
#include <game_objects/level.hpp>
#include <game_objects/horizon.hpp>
#include <factories/material.hpp>
#include <common/object_tags.hpp>
#include <core/world/world.hpp>
#include <core/resources/texture.hpp>
#include <core/resources/material.hpp>
#include <core/model/model.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/renderer.hpp>
#include <core/transform/transform.hpp>
#include <core/input/input.hpp>
#include <core/common/directory.hpp>


namespace
{
  #ifndef NDEBUG
  constexpr float   loading_max_timer = 0.f; // Min time loading screen stays up
  #else
  constexpr float   loading_max_timer = 2.f;
  #endif
}


namespace Game {


Loading_screen::Loading_screen(Game_object::World_objects &objs,
                               Core::World &world,
                               Core::Context &ctx)
: State(objs, world, ctx)
, m_loading_entity(get_world())
, m_timer(0.f)
{
  Core::Input::mouse_set_capture(get_ctx(), false);
  
  Core::Material logo_material = Factory::Material::get_logo();
  const Core::Texture texture(logo_material.get_map_01());
  
  // Create an entity for the logo.
  {
    m_loading_entity.set_name("Loading Screen Logo");
    m_loading_entity.set_tags(Object_tags::gui_cam);
    
    const Core::Transform trans(math::vec3_init(0, 0, 0),
                                math::vec3_init(math::to_float(texture.get_width()),
                                                1,
                                                math::to_float(texture.get_height())),
                                math::quat_init_with_axis_angle(Core::Transform::get_world_left(), -math::quart_tau()));
    
    m_loading_entity.set_transform(trans);
  }
  
  // Apply a renderer to the logo.
  {
    const Core::Model model = Core::Model(Core::Directory::volatile_resource_path("assets/models/unit_plane.obj"));
    const Core::Material_renderer mat_renderer(logo_material, model);
    
    m_loading_entity.set_renderer(mat_renderer);
  }
}


Game_state
Loading_screen::on_update()
{
  m_timer += get_world().get_delta_time();

  if(m_timer > loading_max_timer)
  {
    m_loading_entity.destroy();
    get_world_objs().push_object(new Game_object::Horizon(get_world()));
    get_world_objs().push_object(new Game_object::Level(get_world()));

    return Game_state::title_screen;
  }
  
  return Game_state::loading;
}


} // ns