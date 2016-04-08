#include "player.hpp"
#include <core/world/world.hpp>
#include <core/context/context.hpp>
#include <core/entity/entity.hpp>
#include <core/input/controller.hpp>
#include <core/transform/transform.hpp>
#include <math/vec/vec3.hpp>
#include <core/model/model.hpp>
#include <core/material/texture.hpp>
#include <math/vec/vec2.hpp>
#include <game_objects/bullet.hpp>
#include "level_functions.hpp"
#include <utilities/logging.hpp>


namespace Player_utils {


void
init_players(Core::World &world,
             Player players[],
             const uint32_t number_of_players)
{
  for(uint32_t i = 0; i < number_of_players; ++i)
  {
    players[i].entity = Core::Entity(world);
    
    Core::Model   model("/Users/PhilCK/Developer/core/assets/models/unit_cube.obj");
    Core::Texture texture("/Users/PhilCK/Developer/core/assets/textures/dev_grid_green_512.png");
    
    players[i].entity.set_name("Player");
    players[i].entity.set_model(model);
    players[i].entity.set_material_id(2);
  }
}


void
move_players(Core::Context &ctx,
             const float dt,
             Player players[],
             const uint32_t number_of_players)
{
  for(uint32_t i = 0; i < number_of_players; ++i)
  {
    Player &curr_player = players[i];
    
    Core::Input::Controller controller = Core::Input::Controller(ctx, curr_player.controller_id);
    
    // Movement
    {
      const float move_speed = (controller.get_axis(0).x * 10.f) * dt;
      curr_player.point_on_circle += move_speed;
      
      math::vec2 new_point = Level::get_point_on_cirlce(curr_player.point_on_circle);

      Core::Transform trans = curr_player.entity.get_transform();
      const math::vec3 position = trans.get_position();
      
      math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                           math::vec2_get_y(new_point),
                                           math::vec3_get_z(position));
      trans.set_position(new_pos);
      
      players[0].entity.set_transform(trans);
    }
  }
}


} // ns