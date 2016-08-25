#include <game_objects/player_ui.hpp>
#include <common/object_tags.hpp>
#include <core/resources/material.hpp>
#include <core/resources/texture.hpp>
#include <core/model/model.hpp>
#include <core/resources/shader.hpp>
#include <core/common/directory.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/transform/transform.hpp>


namespace Game_object {


Player_ui::Player_ui(Core::World &world, Core::Context &ctx)
: Game_object(world)
{
  get_entity().set_name("player_ui");
  get_entity().set_tags(Object_tags::gui_cam);
}


void
Player_ui::on_start()
{
  Core::Material ui_mat = Core::Material("player_ui_mat");
  
  const char * orange_texture_path = Core::Directory::volatile_resource_path("assets/textures/dev_grid_orange_512.png");
  Core::Texture texture(orange_texture_path);
  assert(texture);
  
  const char * shader_path = Core::Directory::volatile_resource_path("assets/shaders/vortex_dir_light.ogl");
  Core::Shader shader(shader_path);
  assert(shader);
  
  ui_mat.set_shader(shader);
  ui_mat.set_map_01(texture);

  const char * unit_cube_path = Core::Directory::volatile_resource_path("assets/models/unit_cube.obj");
  Core::Model model = Core::Model(unit_cube_path);
  assert(model);
  
  const Core::Material_renderer mat_renderer(ui_mat, model);
  
  Core::Transform trans;
  trans.set_position(math::vec3_init(0,0,0));
  trans.set_scale(math::vec3_init(128));
  
  get_entity().set_transform(trans);
  get_entity().set_renderer(mat_renderer);
}


void
Player_ui::on_update(const float dt, World_objects &objs)
{
  
}


} // ns