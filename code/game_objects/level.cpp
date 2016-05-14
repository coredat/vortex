#include <game_objects/level.hpp>
#include <common/level_functions.hpp>
#include <core/world/world.hpp>
#include <core/resources/texture.hpp>
#include <core/model/model.hpp>
#include <core/transform/transform.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <utilities/directory.hpp>


namespace Level_utils {


void
init_level(Core::World &world, Level_container &level)
{
  for(uint32_t i = 0; i < level.size; ++i)
  {
    auto &level_data = level.level[i];
  
    level_data.entity = Core::Entity(world);
    level_data.entity.set_name("Level");
    
    const std::string level_path = util::get_resource_path() + "assets/models/unit_tube.obj";
    Core::Model model(level_path.c_str());

    const std::string grey_texture_path = util::get_resource_path() + "assets/textures/dev_grid_grey_512.png";
    Core::Texture texture(grey_texture_path.c_str());

    const float depth = Level::get_top_of_level() - Level::get_bottom_of_level();
    Core::Transform trans(math::vec3_init(0, 0, -depth * 0.5f),
                          math::vec3_init(Level::get_radius() * 2.1f, Level::get_radius() * 2.1f, math::abs(depth) * 0.9f),
                          math::quat_init());

    level_data.entity.set_model(model);
    level_data.entity.set_material_id(texture.get_id());
    level_data.entity.set_transform(trans);
  }
}


} // ns