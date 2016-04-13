#include <game_objects/powerup_pickup.hpp>
#include <common/level_functions.hpp>
#include <core/world/world.hpp>
#include <core/model/model.hpp>
#include <core/material/texture.hpp>
#include <core/transform/transform.hpp>
#include <math/vec/vec3.hpp>
#include <random>


namespace
{
  Core::Model model;
  Core::Texture texture;
}


namespace Powerup_utils {


void
init_powerups(Core::World &world,
              Powerups_container &powerups_container)
{
  model = Core::Model("/Users/PhilCK/Developer/core/assets/models/unit_cube.obj");
  texture = Core::Texture("/Users/PhilCK/Developer/core/assets/textures/dev_squares_512.png");
}


void
update_powerups(Core::World &world,
                Powerups_container &powerups_container,
                const float dt)
{
  for(uint32_t i = 0; i < powerups_container.size; ++i)
  {
    auto &powerup = powerups_container.powerup[i];
    
    if(powerup.entity)
    {
      // Depth
      {
        const float delta_depth = (10.f * dt);
        
        Core::Transform trans = powerup.entity.get_transform();
        const math::vec3 old_pos = trans.get_position();
        
        const math::vec3 new_pos = math::vec3_init(
          math::vec3_get_x(old_pos),
          math::vec3_get_y(old_pos),
          math::vec3_get_z(old_pos) + delta_depth
        );
        
        trans.set_position(new_pos);
        powerup.entity.set_transform(trans);
        
        if(math::vec3_get_z(new_pos) > Level::get_near_death_zone())
        {
          powerup.entity.destroy();
        }
      }
    }
  }
}


void
create_powerup(Core::World &world,
               const math::vec3 position,
               Powerups_container &powerups_container)
{
  // Do we create a powerup?
  {
    const uint32_t dice_roll = rand() % 5;
    
    if(dice_roll != 0)
    {
      return; // No dice!
    }
  }

  // Create if we have a free slot
  for(uint32_t i = 0; i < powerups_container.size; ++i)
  {
    auto &powerup = powerups_container.powerup[i];
    
    if(!powerup.entity)
    {
      powerup.entity = Core::Entity(world);
      powerup.entity.set_name("Powerup");
      powerup.entity.set_model(model);
      powerup.entity.set_material_id(texture.get_id());
      
      Core::Transform trans = powerup.entity.get_transform();
      trans.set_position(position);
      
      powerup.entity.set_transform(trans);
      
      return; // Created
    }
  }
}


void
destroy_powerup(Core::World &world,
                Powerups_container &powerups_container)
{
}


} // ns
