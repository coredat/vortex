#ifndef PLAYER_INCLUDED_27EF33BB_69F2_4B56_909A_1FA47B263E56
#define PLAYER_INCLUDED_27EF33BB_69F2_4B56_909A_1FA47B263E56


#include <game_objects/game_object.hpp>
#include <core/entity/entity.hpp>
#include <core/world/world_fwd.hpp>
#include <core/context/context_fwd.hpp>
#include <core/resources/resources_fwd.hpp>
#include <game_objects/game_objects_fwd.hpp>
#include <stdint.h>


namespace Game_object {


class Player : public Game_object::Game_object
{
public:
  
  explicit          Player(Core::World &world, Core::Context &ctx, const uint32_t controller_id);
  
  void              on_start() override;
  bool              on_update(const float dt, World_objects &world_objs) override;
  void              on_end() override;
  void              on_collision(Game_object *obj) override;

private:

  Core::Context     &m_context;
  uint32_t          m_controller_id   = 0;
  float             point_on_circle = 0.f;
  float             gun_cooldown    = 0.f;
  float             jump_speed      = 0.f;
  float             jump_time       = 0.f;
  float             power_up_timer  = 0.f;
  float             momentum        = 0.f;

};


} // ns


#endif // inc guard