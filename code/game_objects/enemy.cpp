#include <game_objects/enemy.hpp>
#include <core/model/model.hpp>
#include <core/material/texture.hpp>
#include <core/world/world.hpp>
#include <core/transform/transform.hpp>
#include <math/vec/vec2.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <common/level_functions.hpp>
#include <common/object_tags.hpp>


namespace Enemy_utils {


void
init_enemies(Core::World &world,
             Enemy *enemy_arr,
             const uint32_t number_of_entities)
{
  Core::Model   model("/Users/PhilCK/Developer/core/assets/models/unit_cube.obj");
  Core::Texture texture("/Users/PhilCK/Developer/core/assets/textures/dev_grid_orange_512.png");

  for(uint32_t i = 0; i < number_of_entities; ++i)
  {
    Enemy &enemy = enemy_arr[i];
    
    enemy.point_on_circle = static_cast<float>(rand() % 1000) / 10;
  
    enemy.entity = Core::Entity(world);
    enemy.entity.set_name("Enemy");
    enemy.entity.add_tag(Object_tags::enemy);
    enemy.entity.set_model(model);
    enemy.entity.set_material_id(texture.get_id());
    
    const Core::Transform trans(
      math::vec3_init(0, 0, Level::get_bottom_of_level()),
      math::vec3_one(),
      math::quat_init()
    );
    
    enemy.entity.set_transform(trans);
  }
}


void
update_enemies(const float dt,
               Enemy *enemy_arr,
               const uint32_t number_of_entities)
{
  for(uint32_t i = 0; i < number_of_entities; ++i)
  {
    Enemy &enemy = enemy_arr[i];
    
    //enemy.point_on_circle += dt;
  
    math::vec2 new_point = Level::get_point_on_cirlce(enemy.point_on_circle);
    
    Core::Transform trans = enemy.entity.get_transform();
    const math::vec3 position = trans.get_position();
    
    math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                         math::vec2_get_y(new_point),
                                         math::vec3_get_z(position));
    trans.set_position(new_pos);
    
    enemy.entity.set_transform(trans);
  }
}


void
hit_enemy(const Core::Entity_id id,
          Enemy *enemy_arr,
          const uint32_t number_of_entities)
{
  for(uint32_t i = 0; i < number_of_entities; ++i)
  {
    Enemy &enemy = enemy_arr[i];
    
    if(enemy.entity.get_id() == id)
    {
      enemy.entity.destroy();
    }
  }
}

} // ns