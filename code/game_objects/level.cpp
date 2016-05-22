#include <game_objects/level.hpp>
#include <common/level_functions.hpp>
#include <core/world/world.hpp>
#include <core/resources/texture.hpp>
#include <core/model/model.hpp>
#include <core/transform/transform.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <utilities/directory.hpp>


namespace Game_object {


Level::Level(Core::World &world)
: Game_object(world)
{
}


void
Level::on_start()
{
  auto ref = get_entity();

  ref.set_name("Level");
  
  const std::string level_path = util::get_resource_path() + "assets/models/unit_tube.obj";
  Core::Model model(level_path.c_str());

  const std::string grey_texture_path = util::get_resource_path() + "assets/textures/dev_grid_grey_512.png";
  Core::Texture texture(grey_texture_path.c_str());

  const float depth = Level_funcs::get_top_of_level() - Level_funcs::get_bottom_of_level();
  Core::Transform trans(math::vec3_init(0, 0, -depth * 0.5f),
                        math::vec3_init(Level_funcs::get_radius() * 2.1f, Level_funcs::get_radius() * 2.1f, math::abs(depth) * 0.9f),
                        math::quat_init());

  ref.set_model(model);
  ref.set_material_id(texture.get_id());
  ref.set_transform(trans);
}


bool
Level::on_update(const float dt, World_objects &objs)
{
  return true;
}


} // ns