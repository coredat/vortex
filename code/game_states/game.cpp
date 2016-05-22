#include <game_states/game.hpp>
#include <game_objects/player.hpp>
#include <game_objects/powerup_pickup.hpp>
#include <game_objects/enemy.hpp>
#include <game_objects/explosion.hpp>
#include <game_objects/bullet.hpp>
#include <common/game_state.hpp>
#include <common/object_tags.hpp>
#include <core/world/world.hpp>
#include <core/context/context.hpp>
#include <core/physics/collision_pair.hpp>


void
game_init(Core::Context &ctx,
          Core::World &world)
{
}


Game_state
game_update(Core::Context &context,
            Core::World &world,
            Enemies_container &enemies_container,
            Explosions_container &explosions_container,
            Powerups_container &powerups_container,
            const float dt)
{
  world.get_overlapping_aabbs([&](const Core::Collision_pair pairs[], const uint32_t number_of_pairs)
  {
    for(uint32_t i = 0; i < number_of_pairs; ++i)
    {
      const Core::Entity_ref &ref_a = pairs[i].entity_a;
      const Core::Entity_ref &ref_b = pairs[i].entity_b;

      // Enemy collided with a bullet
      if(ref_b.has_tag(Object_tags::bullet) && ref_a.has_tag(Object_tags::enemy))
      {
        Enemy_utils::hit_enemy(world,
                               ref_a,
                               enemies_container,
                               explosions_container,
                               powerups_container);
      }
      
      if(ref_a.has_tag(Object_tags::player))
      {
        if(ref_b.has_tag(Object_tags::enemy))
        {
          Game_object::Game_object *go_obj    = reinterpret_cast<Game_object::Game_object*>(ref_a.get_user_data());
          Game_object::Game_object *other_obj = reinterpret_cast<Game_object::Game_object*>(ref_b.get_user_data());
          
          go_obj->on_collision(other_obj);
        
//          Player_utils::hit_player(world,
//                                   ref_a,
//                                   players_container,
//                                   explosions_container);
        }
        
        if(ref_b.has_tag(Object_tags::powerup))
        {
//          Player_utils::power_up(world,
//                                 ref_a,
//                                 players_container);
        }
      }
    }
  });
    
  Enemy_utils::spawn_enemies(world, dt, enemies_container);

//  if(Player_utils::all_dead(players_container))
//  {
//    return Game_state::game_over;
//  }
  
  return Game_state::game_mode;
}