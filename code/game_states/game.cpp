#include <game_states/game.hpp>
#include <common/global_vars.hpp>
#include <common/level_functions.hpp>
#include <game_objects/world_objects.hpp>
#include <game_objects/player_ship.hpp>
#include <game_objects/powerup_pickup.hpp>
#include <game_objects/enemy.hpp>
#include <game_objects/explosion.hpp>
#include <game_objects/bullet.hpp>
#include <common/game_state.hpp>
#include <common/object_tags.hpp>
#include <core/world/world.hpp>
#include <core/context/context.hpp>
#include <core/physics/collision_pair.hpp>
#include <utilities/logging.hpp>
#include <assert.h>


void
game_init(Core::Context &ctx,
          Core::World &world)
{
}


namespace
{

constexpr float spawn_rate = 0.2f;
float spawn_timer = 0.f;

void
spawn_enemies(Core::World &world,
              const float dt,
              Game_object::World_objects &objs)
{
  spawn_timer += dt;
  
  // Max out spawns.
  if(world.get_entity_count_in_world() > Global::g_max_spawn_entities)
  {
    return;
  }

  if(spawn_timer > spawn_rate)
  {
    spawn_timer = 0;
    
    using Enemy_type = Game_object::Enemy::Type;
    Enemy_type what_to_spawn = (Enemy_type)(rand() % (uint32_t)Enemy_type::size);
    
    switch(what_to_spawn)
    {
      case(Enemy_type::climber):
      case(Enemy_type::breeder):
      case(Enemy_type::shooter):
        objs.push_object(new Game_object::Enemy(world, what_to_spawn));
        break;
        
      case(Enemy_type::egg):
        objs.push_object(new Game_object::Enemy(world, Enemy_type::breeder));
        break;
      
      default:
        assert(false);
    }
  }
}

}


Game_state
game_update(Core::Context &context,
            Core::World &world,
            Game_object::World_objects &objs,
            const float dt)
{
  world.get_overlapping_aabbs([&](const Core::Collision_pair pairs[], const uint32_t number_of_pairs)
  {
    for(uint32_t i = 0; i < number_of_pairs; ++i)
    {
      const Core::Entity_ref &ref_a = pairs[i].entity_a;
      const Core::Entity_ref &ref_b = pairs[i].entity_b;
      
      if(!ref_a.is_valid())
      {
        continue;
      }

      // Enemy collided with a bullet
      if(ref_b.has_tag(Object_tags::bullet) && ref_a.has_tag(Object_tags::enemy))
      {
        Game_object::Game_object *this_obj = reinterpret_cast<Game_object::Game_object*>(ref_a.get_user_data());
        Game_object::Game_object *that_obj = reinterpret_cast<Game_object::Game_object*>(ref_b.get_user_data());
        assert(this_obj);
        
        if(this_obj)
        {
          this_obj->on_collision(that_obj);
        }
      }
      
      if(ref_a.has_tag(Object_tags::player))
      {
        if(ref_b.has_tag(Object_tags::enemy) || ref_b.has_tag(Object_tags::bullet))
        {
          Game_object::Game_object *this_obj = reinterpret_cast<Game_object::Game_object*>(ref_a.get_user_data());
          Game_object::Game_object *that_obj = reinterpret_cast<Game_object::Game_object*>(ref_b.get_user_data());
          assert(this_obj);
          
          if(this_obj)
          {
            this_obj->on_collision(that_obj);
          }
        }
        
        if(ref_b.has_tag(Object_tags::powerup))
        {
          Game_object::Game_object *this_obj = reinterpret_cast<Game_object::Game_object*>(ref_a.get_user_data());
          Game_object::Game_object *that_obj = reinterpret_cast<Game_object::Game_object*>(ref_b.get_user_data());
          assert(this_obj);
          
          if(this_obj)
          {
            this_obj->on_collision(that_obj);
          }
          
          if(that_obj)
          {
            that_obj->on_collision(this_obj);
          }
        }
      }
    }
  });
    
  spawn_enemies(world, dt, objs);
  
  // If all player ships are dead move to game overscreen
  {
    Core::Entity_ref *out_players;
    size_t found_players = 0;
    
    world.find_entities_by_tag(Object_tags::player, &out_players, &found_players);
    
    if(found_players == 0)
    {
      return Game_state::game_over;
    }
  }
  
  return Game_state::game_mode;
}