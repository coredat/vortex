#include <game_objects/player.hpp>
#include <game_objects/bullet.hpp>
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
#include <utilities/logging.hpp>



namespace
{
  constexpr float gun_cooldown_timer = 0.1f;
  constexpr float move_speed_base = 5.f;
}


namespace Game_object {


Player::Player(Core::World &world, Core::Context &ctx, const uint32_t controller_id)
: Game_object(world)
, m_context(ctx)
, m_controller_id(controller_id)
{
  Core::Entity_ref ref = get_entity();
  
  const std::string unit_cube_path = util::get_resource_path() + "assets/models/ship.obj";
  Core::Model model(unit_cube_path.c_str());

  const std::string green_texture_path = util::get_resource_path() + "assets/textures/dev_grid_green_512.png";
  Core::Texture texture(green_texture_path.c_str());

  Core::Box_collider collider = Core::Box_collider_utils::create_with_half_extents(math::aabb_get_half_extents(model.get_model_aabb()));
  
  Core::Rigidbody_properties rb_props;
  rb_props.set_collision_mask(Object_tags::player, Object_tags::enemy);
  
  ref.set_name("Player");
  ref.set_tags(Object_tags::player);
  ref.set_model(model);
  ref.set_material_id(texture.get_id());
  ref.set_collider(collider);
  ref.set_rigidbody_properties(rb_props);
  
    const math::quat y_rot = math::quat_init_with_axis_angle(0, 1, 0, math::quart_tau());
    const math::quat z_rot = math::quat_init_with_axis_angle(0, 0, 1, math::quart_tau());
    const math::quat rot = math::quat_multiply(y_rot, z_rot);
  
    uint32_t i = 0;
  
    Core::Transform trans(
      math::vec3_init(-4.f + (i * (8.f / 4.f)), 0, 0),
      math::vec3_one(),
      rot
    );
    
    ref.set_transform(trans);
}


void
Player::on_start()
{

}


bool
Player::on_update(const float dt, World_objects &world_objs)
{
  Core::Entity_ref ref = get_entity();

  power_up_timer -= dt;
  gun_cooldown -= dt;
  
  Core::Input::Controller controller = Core::Input::Controller(m_context, m_controller_id);
  
  // Lateral Movement
  {
    const float move_speed = (controller.get_axis(0).x * move_speed_base) * dt;
    
    if(move_speed)
    {
      momentum += (move_speed * 5.f);
    }
    
    momentum *= 0.95f;
    
    point_on_circle += move_speed;
    
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
  
  // Jump
  if(controller.is_button_down(Core::Input::Button::button_0) && jump_speed == 0.f)
  {
    jump_speed = 50.5f;
    jump_time = 0.f;
  }
  
  // Jump movement
  if(jump_speed)
  {
    jump_time += (dt * 7.f);
    float offset = (jump_speed + (-jump_time * jump_time * jump_time)) * (dt);

    Core::Transform trans = ref.get_transform();
    const math::vec3 pos = trans.get_position();
    
    float new_depth = math::vec3_get_z(pos) + offset;
    
    if(new_depth < Level_funcs::get_top_of_level())
    {
      new_depth = Level_funcs::get_top_of_level();
      jump_speed = 0.f;
      jump_time = 0.f;
    }
    
    const math::vec3 new_pos = math::vec3_init(math::vec3_get_x(pos), math::vec3_get_y(pos), new_depth);
    trans.set_position(new_pos);
    
    ref.set_transform(trans);
  }
  
  // Fire
  {
    const math::vec3 pos = ref.get_transform().get_position();
  
    const float multipler = power_up_timer > 0 ? dt * 15.f : 0.f;
    const float timer = gun_cooldown;
    
    if(timer < (0.f + multipler) && (controller.get_trigger(0) || controller.get_trigger(1) || controller.is_button_down(Core::Input::Button::button_3)))
    {
      auto bullet = new Bullet(get_world(),
                               point_on_circle,
                               math::vec3_get_z(pos),
                               -1);
      world_objs.push_object(bullet);
      
//        Bullet_utils::create_bullet(world,
//                                    point_on_circle,
//                                    math::vec3_get_z(pos),                                    
//                                    -1,
//                                    bullets_container);
      gun_cooldown = gun_cooldown_timer;
    }
  }

  return true;
}


void
Player::on_end()
{
}


void
Player::on_collision(Game_object::Game_object *obj)
{
  this->destroy();
}


} // ns