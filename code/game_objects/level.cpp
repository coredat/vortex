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
init_level(Core::World &world, Level_data level_arr[], const uint32_t size_of_level)
{
  for(uint32_t i = 0; i < size_of_level; ++i)
  {
    level_arr[i].entity = Core::Entity(world);
    
    Core::Model   model("/Users/PhilCK/Developer/core/assets/models/unit_tube.obj");
    Core::Texture texture("/Users/PhilCK/Developer/core/assets/textures/dev_grid_red_512.png");

    const float depth = Level::get_top_of_level() - Level::get_bottom_of_level();
    Core::Transform trans(math::vec3_init(0, 0, -depth * 0.5f), math::vec3_init(Level::get_radius() * 2.f, Level::get_radius() * 2.f, math::abs(depth)), math::quat_init());

    level_arr[i].entity.set_model(model);
    level_arr[i].entity.set_material_id(texture.get_id());
    level_arr[i].entity.set_transform(trans);
  }
}


} // ns