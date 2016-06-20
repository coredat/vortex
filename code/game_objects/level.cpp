#include <game_objects/level.hpp>
#include <game_objects/world_objects.hpp>
#include <common/level_functions.hpp>
#include <common/object_tags.hpp>
#include <core/world/world.hpp>
#include <core/resources/texture.hpp>
#include <core/resources/material.hpp>
#include <core/resources/shader.hpp>
#include <core/model/model.hpp>
#include <core/color/color.hpp>
#include <core/color/color_predefined.hpp>
#include <core/transform/transform.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <math/general/general.hpp>
#include <utilities/directory.hpp>


namespace Game_object {


Level::Level(Core::World &world)
: Game_object(world)
{
  Core::Entity_ref ref = get_entity();

  ref.set_name("Level");
  
  const std::string level_path = util::get_resource_path() + "assets/models/level_tube.obj";
  Core::Model model(level_path.c_str());

  const float depth = Level_funcs::get_top_of_level() - Level_funcs::get_bottom_of_level();
  Core::Transform trans(math::vec3_init(0.f, 0.f, -100.f),
                        math::vec3_init(20.f),
                        math::quat_init());

  // Create a material
  Core::Material level_material("Level");
  {
    const std::string grey_texture_path = util::get_resource_path() + "assets/textures/dev_grid_grey_512.png";
    Core::Texture texture(grey_texture_path.c_str());

    const std::string shader_path = util::get_resource_path() + "assets/shaders/vortex_level.ogl";
    Core::Shader shader(shader_path.c_str());
    
    level_material.set_shader(shader);
    level_material.set_map_01(texture);
    level_material.set_color(Core::Color_utils::magenta());
  }

  ref.set_material(level_material);
  ref.set_model(model);
  ref.set_transform(trans);
  ref.add_tag(Object_tags::world_cam);
}


void
Level::on_update(const float dt, World_objects &objs)
{

}


} // ns