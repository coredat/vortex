#include <game_objects/player.hpp>
#include <game_objects/bullet.hpp>
#include <game_objects/explosion.hpp>
#include <game_objects/world_objects.hpp>
#include <common/level_functions.hpp>
#include <common/object_tags.hpp>
#include <core/world/world.hpp>
#include <core/context/context.hpp>
#include <core/entity/entity.hpp>
#include <core/input/controller.hpp>
#include <core/transform/transform.hpp>
#include <core/model/model.hpp>
#include <core/resources/texture.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/box_collider.hpp>
#include <core/physics/box_collider_utils.hpp>
#include <core/physics/rigidbody_properties.hpp>
#include <math/vec/vec2.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <math/geometry/aabb.hpp>
#include <utilities/directory.hpp>
#include <utilities/optimizations.hpp>


namespace
{
  // General settings.
  constexpr float gun_cooldown_timer          = 0.1f;
  constexpr float move_speed_base             = 5.f;
  constexpr float momentum_falloff            = 0.95f;
  constexpr float powerup_durration           = 5.f;
  constexpr float powerup_time_dialation_rate = 0.9f;
}


namespace Game_object {


Player::Player(Core::World &world,
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
  const std::string unit_cube_path = util::get_resource_path() + "assets/models/ship_01.obj";
  Core::Model model(unit_cube_path.c_str());

  const std::string green_texture_path = util::get_resource_path() + "assets/textures/dev_grid_green_512.png";
  Core::Texture texture(green_texture_path.c_str());

  Core::Box_collider collider = Core::Box_collider_utils::create_with_full_extents(math::vec3_one());
  
  Core::Rigidbody_properties rb_props;
  rb_props.set_collision_mask(Object_tags::player, Object_tags::enemy | Object_tags::powerup);
  
  // Setup entity.
  Core::Entity_ref ref = get_entity();
  {
    ref.set_name("Player");
    ref.set_tags(Object_tags::player);
    ref.set_model(model);
    ref.set_material_id(texture.get_id());
    ref.set_collider(collider);
    ref.set_rigidbody_properties(rb_props);
  }
}


void
Player::on_start()
{

}


void
Player::on_update(const float dt, World_objects &world_objs)
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
      
      Core::Input::Controller controller = Core::Input::Controller(m_context, m_controller_id);
      
      // Lateral Movement
      {
        const float move_axis  = controller.get_axis(0).x * move_speed_base;
        const float move_speed = move_axis * movement_dt;
        
        if(move_speed)
        {
          m_momentum += (move_speed * 5.f);
        }
        
        m_momentum *= momentum_falloff;
        
        m_point_on_circle += move_speed;
        
        const math::vec2 new_point = Level_funcs::get_point_on_cirlce(m_point_on_circle);

        Core::Transform trans = ref.get_transform();
        const math::vec3 position = trans.get_position();
        
        const math::vec3 new_pos = math::vec3_init(math::vec2_get_x(new_point),
                                                   math::vec2_get_y(new_point),
                                                   math::vec3_get_z(position));
        
        const math::quat y_rot = math::quat_init_with_axis_angle(0, 1, 0, math::quart_tau());
        const math::quat z_rot = math::quat_init_with_axis_angle(0, 0, 1, -m_point_on_circle + math::quart_tau() - m_momentum);

        trans.set_position(new_pos);
        trans.set_rotation(math::quat_multiply(y_rot, z_rot));
        
        ref.set_transform(trans);
      }
      
      // New Jump
      if(controller.is_button_down(Core::Input::Button::button_0) && m_jump_speed == 0.f)
      {
        m_jump_speed = 50.5f;
        m_jump_time = 0.f;
      }
      
      // Jump Movement
      if(m_jump_speed)
      {
        m_jump_time += (movement_dt * 7.f);
        const float jump_velocity = (m_jump_speed + (-m_jump_time * m_jump_time * m_jump_time)) * (movement_dt);

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
           (controller.get_trigger(0) || controller.get_trigger(1) || controller.is_button_down(Core::Input::Button::button_3)))
        {
          auto bullet = new Bullet(get_world(),
                                   math::vec2_init(math::vec3_get_z(ref.get_transform().get_position()), m_point_on_circle),
                                   math::vec2_init(0, -1),
                                   100);
          
          world_objs.push_object(bullet);
          
          m_gun_cooldown = gun_cooldown_timer;
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
      break;
    }
    
    default:
      break;
  }
}


void
Player::on_collision(Game_object *obj)
{
  if(obj && obj->get_entity().has_tag(Object_tags::enemy))
  {
    m_state = State::dying;
  }
  
  else if(obj && obj->get_entity().has_tag(Object_tags::powerup))
  {
    if(m_powerup == Powerup::none)
    {
      obj->destroy();
      
      const uint32_t number_of_powerups = (uint32_t)Powerup::size;
      const uint32_t selected_powerup   = rand() % number_of_powerups;
      
      m_powerup = (Powerup)selected_powerup;
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