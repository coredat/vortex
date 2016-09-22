#include <game_states/game.hpp>
#include <game_states/game_over.hpp>
#include <common/global_vars.hpp>
#include <common/level_functions.hpp>
#include <game_objects/world_objects.hpp>
#include <game_objects/player_ship.hpp>
#include <game_objects/powerup_pickup.hpp>
#include <game_objects/enemy.hpp>
#include <game_objects/main_camera.hpp>
#include <game_objects/explosion.hpp>
#include <game_objects/bullet.hpp>
#include <common/game_state.hpp>
#include <common/object_tags.hpp>
#include <core/world/world.hpp>
#include <core/context/context.hpp>
#include <core/physics/collision_pair.hpp>
#include <core/physics/collision.hpp>
#include <utilities/logging.hpp>
#include <assert.h>


namespace {


constexpr float spawn_rate = 0.2f;


} // anon ns


namespace Game {


Game_screen::Game_screen(Game_object::World_objects &objs,
                         Core::World &world,
                         Core::Context &ctx)
: State(objs, world, ctx)
, m_camera(get_world().find_entity_by_name("Main Camera"))
, m_spawn_timer(0.f)
, m_game_timer(0.f)
{
  assert(m_camera);

  Core::Input::mouse_set_capture(get_ctx(), true);

  Game_object::Main_camera *main_camera = reinterpret_cast<Game_object::Main_camera*>(m_camera.get_user_data());
  assert(main_camera);

  main_camera->set_target_height(20.f);
  main_camera->set_target_speed(10.f);
}


std::unique_ptr<State>
Game_screen::on_update()
{
  get_world().set_collision_callback([](const Core::Collision_type type, const Core::Collision &collision)
  {
//    for(uint32_t i = 0; i < number_of_pairs; ++i)
    {
      const Core::Entity_ref &ref_a = collision.get_entity_a();
      const Core::Entity_ref &ref_b = collision.get_entity_b();
      
      if(!ref_a.is_valid() || !ref_b.is_valid())
      {
        assert(false);
//        return;
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
        if(ref_b.has_tag(Object_tags::enemy) || (ref_b.has_tag(Object_tags::enemy) && ref_b.has_tag(Object_tags::bullet)))
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
  
  // Spawn enemies
  {
    const float dt = get_world().get_delta_time();
  
    m_spawn_timer += dt;
    m_game_timer += dt;
    
    // Max out spawns.
    if(get_world().get_entity_count_in_world() < Global::g_max_spawn_entities)
    {
      if(m_spawn_timer > spawn_rate)
      {
        m_spawn_timer = 0;
        
        using Enemy_type = Game_object::Enemy::Type;
        
        const uint32_t level = math::min(math::to_uint(m_game_timer) + 1, (uint32_t)Enemy_type::size - 1);
        
        Enemy_type what_to_spawn = (Enemy_type)(rand() % level);
        
        switch(what_to_spawn)
        {
          case(Enemy_type::climber):
          case(Enemy_type::breeder):
          case(Enemy_type::shooter):
          case(Enemy_type::boomerang):
            get_world_objs().push_object(new Game_object::Enemy(get_world(), what_to_spawn));
            break;
            
          case(Enemy_type::egg):
            get_world_objs().push_object(new Game_object::Enemy(get_world(), Enemy_type::breeder));
            break;
          
          default:
            assert(false);
        }
      }
    }
  }

  // If all player ships are dead move to game overscreen
  {
    Core::Entity_ref *out_players;
    size_t found_players = 0;
    
    get_world().find_entities_by_tag(Object_tags::player, &out_players, &found_players);
    
    if(found_players == 0)
    {
      return std::unique_ptr<State>(new Game::Game_over_screen(get_world_objs(), get_world(), get_ctx()));
    }
  }
  
  return nullptr;
}


} // ns