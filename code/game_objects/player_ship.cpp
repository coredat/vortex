#include <game_objects/player_ship.hpp>
#include <game_objects/bullet.hpp>
#include <game_objects/explosion.hpp>
#include <game_objects/world_objects.hpp>
#include <common/level_functions.hpp>
#include <common/object_tags.hpp>
#include <common/event_ids.hpp>
#include <common/global_vars.hpp>
#include <core/world/world.hpp>
#include <core/context/context.hpp>
#include <core/entity/entity.hpp>
#include <core/input/controller.hpp>
#include <core/input/axis.hpp>
#include <core/input/buttons.hpp>
#include <core/transform/transform.hpp>
#include <core/model/model.hpp>
#include <core/resources/texture.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/box_collider.hpp>
#include <core/physics/box_collider_utils.hpp>
#include <core/physics/rigidbody.hpp>
#include <math/vec/vec2.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <math/general/general.hpp>
#include <math/geometry/aabb.hpp>
#include <core/common/directory.hpp>
#include <utilities/optimizations.hpp>
#include <utilities/logging.hpp>


namespace
{
  // General settings.
  constexpr float gun_cooldown_timer          = 0.05f;
  constexpr float move_speed_base             = 5.f;
  constexpr float momentum_falloff            = 0.90f;
  constexpr float powerup_durration           = 5.f;
  constexpr float powerup_time_dialation_rate = 0.9f;
  constexpr uint32_t gun_max_blast            = 5;
}


namespace Game_object {


Player_ship::Player_ship(Core::World &world,
                         Core::Context &ctx,
                         const uint32_t controller_id)
: Game_object(world)
, m_context(ctx)
, m_controller_id(controller_id)
, m_point_on_circle(0.f)
, m_gun_cooldown(0.f)
, m_jump_speed(0.f)
, m_jump_time(0.f)
, m_momentum(0.f)
, m_state(State::alive)
, m_powerup(Powerup::none)
, m_powerup_timer(0.f)
{
  const char *unit_cube_path = Core::Directory::volatile_resource_path("assets/models/ship_01.obj");
  Core::Model model(unit_cube_path);

  const char *green_texture_path = Core::Directory::volatile_resource_path("assets/textures/dev_grid_green_512.png");
  Core::Texture texture(green_texture_path);

  Core::Box_collider collider(0.7f, 0.7f, 0.7f);

  Core::Rigidbody rb;
  rb.set_collider(collider);
  rb.set_collision_mask(Object_tags::player, Object_tags::enemy | Object_tags::powerup);
  rb.set_is_trigger(true);
  
  // Setup entity.
  Core::Entity_ref ref = get_entity();
  {
    ref.set_name("Player");
    ref.set_tags(Object_tags::player | Object_tags::world_cam);
    ref.set_rigidbody(rb);
    
    Core::Transform trans = ref.get_transform();
    trans.set_position(math::vec3_init(math::get_x(trans.get_position()), math::get_y(trans.get_position()), Level_funcs::get_top_of_level()));
    ref.set_transform(trans);
  }
}


namespace
{
  /*
    Update the position of the entity.
  */
  inline void
  update_position(Core::Entity_ref ref,
                  const float point_on_circle,
                  const float momentum)
  {
    const math::vec2 new_point = Level_funcs::get_point_on_cirlce(point_on_circle);

    Core::Transform trans = ref.get_transform();
    const math::vec3 position = trans.get_position();

    const math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                               math::vec2_get_y(new_point),
                                               math::vec3_get_z(position));

    const math::quat y_rot = math::quat_init_with_axis_angle(0, 1, 0, math::quart_tau());
    const math::quat z_rot = math::quat_init_with_axis_angle(0, 0, 1, -point_on_circle + math::quart_tau() - momentum);

    trans.set_position(new_pos);
    trans.set_rotation(math::quat_multiply(y_rot, z_rot));

    ref.set_transform(trans);
  }
}


void
Player_ship::on_start()
{
  update_position(get_entity(), m_point_on_circle, m_momentum);
}


void
Player_ship::set_point_on_circle(const float point)
{
  m_point_on_circle = point;
}


void
Player_ship::on_update(const float dt, World_objects &world_objs)
{
  float move_multiplier = 1.f;

  // Update powerups
  switch(m_powerup)
  {
    case(Powerup::time_dialation):
    {
      const float norm_time = m_powerup_timer / powerup_durration;
      const float radians   = math::half_tau() * norm_time;
      const float offset    = math::sin(radians);
      move_multiplier       -= (offset * powerup_time_dialation_rate);
      break;
    }
    
    default:
      break;
  }

  const float movement_dt = dt * move_multiplier;
  m_powerup_timer += dt;
  
  // End the powerup.
  if(m_powerup_timer > powerup_durration)
  {
    m_powerup_timer = 0;
    m_powerup = Powerup::none;
  }

  switch(m_state)
  {
    case(State::alive):
    {
      Core::Entity_ref ref = get_entity();

      m_gun_cooldown -= dt;
      
      auto id = m_controller_id - 1;
      Core::Controller controller = Core::Controller(m_context, id);
      
      // Lateral Movement
      {
        const float axis = Global::g_auto_play ? 1.f : math::max_length(controller.get_axis(0).x, controller.get_axis(1).x);
        const float move_axis  = axis * move_speed_base;
        const float move_speed = move_axis * movement_dt;
        
        if(move_speed)
        {
          m_momentum += (move_speed * 500.f * dt);
          m_momentum = math::clamp(m_momentum, -0.75f, +0.75f);
        }
        
        m_momentum *= momentum_falloff;
        
        m_point_on_circle += move_speed;
        
        update_position(get_entity(), m_point_on_circle, m_momentum);
      }
      
      // New Jump
      if(controller.is_button_down(Core::Gamepad_button::b) && m_jump_speed == 0.f)
      {
        m_jump_speed = 50.5f;
        m_jump_time = 0.f;
      }
      
      // Jump Movement
      if(m_jump_speed)
      {
        m_jump_time += (movement_dt * 13.f);
        const float jump_velocity = (m_jump_speed + (-m_jump_time * m_jump_time)) * (movement_dt);

        Core::Transform trans = ref.get_transform();
        const math::vec3 pos  = trans.get_position();
        
        const float new_depth   = math::vec3_get_z(pos) + jump_velocity;
        const float final_depth = math::max(new_depth, Level_funcs::get_top_of_level());
        
        if(new_depth < Level_funcs::get_top_of_level())
        {
          m_jump_speed = 0.f;
          m_jump_time  = 0.f;
        }
        
        const math::vec3 new_pos = math::vec3_init(math::vec3_get_x(pos), math::vec3_get_y(pos), final_depth);
        trans.set_position(new_pos);
        
        ref.set_transform(trans);
      }
      
      // Fire
      {
        const float multipler = dt * 1.f;
        const float timer     = m_gun_cooldown;
        
        if(timer < (0.f + multipler) &&
           (controller.get_trigger(0) || controller.get_trigger(1) || Global::g_auto_play) &&
           m_gun_overheat < gun_max_blast
          )
        {
          m_gun_overheat += 1;
        
          if(m_powerup == Powerup::cross_fire)
          {
            auto bullet1 = new Bullet(get_world(),
                                     math::vec2_init(math::vec3_get_z(ref.get_transform().get_position()), m_point_on_circle),
                                     math::vec2_init(+1, -1),
                                     70.7f,
                                     Object_tags::enemy,
                                     m_controller_id);
            
            auto bullet2 = new Bullet(get_world(),
                                     math::vec2_init(math::vec3_get_z(ref.get_transform().get_position()), m_point_on_circle),
                                     math::vec2_init(-1, -1),
                                     70.7f,
                                     Object_tags::enemy,
                                     m_controller_id);
            
            world_objs.push_object(bullet1);
            world_objs.push_object(bullet2);
          }
          else if(m_powerup == Powerup::sidewinder)
          {
            auto bullet = new Bullet(get_world(),
                                     math::vec2_init(math::vec3_get_z(ref.get_transform().get_position()), m_point_on_circle),
                                     math::vec2_init(-1, -0.25),
                                     70.7f,
                                     Object_tags::enemy,
                                     m_controller_id);
            
            world_objs.push_object(bullet);
          }
          else
          {
            auto bullet = new Bullet(get_world(),
                                     math::vec2_init(math::vec3_get_z(ref.get_transform().get_position()), m_point_on_circle),
                                     math::vec2_init(0, -1),
                                     100.7f,
                                     Object_tags::enemy,
                                     m_controller_id);
            
            world_objs.push_object(bullet);
          }

          m_gun_cooldown = gun_cooldown_timer;
        }
        else if(timer < (0.f + multipler))
        {
          m_gun_overheat = 0;
          m_gun_cooldown = 0.2f;
        }
      }
      
      break;
    }
    
    case(State::dying):
    {
      destroy();
      world_objs.push_object(new Explosion(get_world(),
                                           get_entity().get_transform().get_position()));
      m_state = State::dead;
      
      world_objs.send_event(Event_id::player_died, (void*)&m_controller_id);
      
      break;
    }
    
    default:
      break;
  }
}


void
Player_ship::on_collision(Game_object *obj)
{
  if(!obj)
  {
    LOG_ERROR("Look at this")
    return;
  }
  const auto ref = obj->get_entity();

  if(obj && (ref.has_tag(Object_tags::enemy) || ref.has_tag(Object_tags::bullet)))
  {
    if(!Global::g_god_mode)
    {
      m_state = State::dying;
    }
  }
  
  else if(obj && ref.has_tag(Object_tags::powerup))
  {
    if(m_powerup == Powerup::none)
    {
      obj->destroy();
      
      // 1 offsets so we don't get Powerup::none;
      
      const uint32_t number_of_powerups = (uint32_t)Powerup::size - 1;
      const uint32_t selected_powerup   = rand() % number_of_powerups;
      
      m_powerup = (Powerup)(1 + selected_powerup);
      m_powerup_timer = 0;
      
      // Setup powerup
      switch(m_powerup)
      {
        default:
          break;
      }
    }
  }
}


} // ns