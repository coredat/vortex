#ifndef PLAYER_SHIP_INCLUDED_27EF33BB_69F2_4B56_909A_1FA47B263E56
#define PLAYER_SHIP_INCLUDED_27EF33BB_69F2_4B56_909A_1FA47B263E56


#include <game_objects/game_object.hpp>
#include <core/entity/entity.hpp>
#include <core/context/context_fwd.hpp>
#include <stdint.h>


namespace Game_object {


class Player_ship : public Game_object
{
public:
  
  explicit          Player_ship(Core::World &world,
                                Core::Context &ctx,
                                const uint32_t controller_id);
  
  void              on_start() override;
  void              on_update(const float dt,
                              World_objects &world_objs) override;
  void              on_collision(Game_object *obj) override;

  void              set_point_on_circle(const float point);

private:

  Core::Context     &m_context;
  const uint32_t    m_controller_id     = 0;
  float             m_point_on_circle   = 0.f;
  float             m_gun_cooldown      = 0.f;
  float             m_gun_overheat      = 0.f;
  float             m_jump_speed        = 0.f;
  float             m_jump_time         = 0.f;
  float             m_momentum          = 0.f;
  
  enum class State
  {
    alive,
    dying,
    dead,
  };
  
  enum class Powerup
  {
    none,
    time_dialation,
    cross_fire,
    sidewinder,
    
    size,
  };
  
  State        m_state         = State::alive;
  Powerup      m_powerup       = Powerup::none;
  float        m_powerup_timer = 0.f;
};


} // ns


#endif // inc guard