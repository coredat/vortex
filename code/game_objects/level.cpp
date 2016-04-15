#include <game_objects/level.hpp>
#include <core/world/world.hpp>
#include <core/material/texture.hpp>
#include <core/model/model.hpp>
#include <core/transform/transform.hpp>
#include <common/level_functions.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>


namespace Level_utils {


void
init_level(Core::World &world, Level_container &level)
{
  for(uint32_t i = 0; i < level.size; ++i)
  {
    auto &level_data = level.level[i];
  
    level_data.entity = Core::Entity(world);
    
    Core::Model   model("/Users/PhilCK/Developer/core/assets/models/unit_tube.obj");
    Core::Texture texture("/Users/PhilCK/Developer/core/assets/textures/dev_grid_grey_512.png");

    const float depth = Level::get_top_of_level() - Level::get_bottom_of_level();
    Core::Transform trans(math::vec3_init(0, 0, -depth * 0.5f),
                          math::vec3_init(Level::get_radius() * 2.2f, Level::get_radius() * 2.2f, math::abs(depth)),
                          math::quat_init());

    level_data.entity.set_model(model);
    level_data.entity.set_material_id(texture.get_id());
    level_data.entity.set_transform(trans);
  }
}


} // ns