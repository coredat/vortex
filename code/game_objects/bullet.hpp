#ifndef BULLET_INCLUDED_992A35A6_21D1_4D51_BF4E_06917B3704D3
#define BULLET_INCLUDED_992A35A6_21D1_4D51_BF4E_06917B3704D3


#include <game_objects/game_object.hpp>
#include <game_objects/game_objects_fwd.hpp>
#include <core/world/world_fwd.hpp>
#include <math/vec/vec2.hpp>
#include <stdint.h>


namespace Game_object {


class Bullet : public Game_object
{
public:

  explicit          Bullet(Core::World &world,
                           const math::vec2 depth_point,
                           const math::vec2 direction,
                           const float speed,
                           const uint32_t collision_flags);
  
  void              on_start() override;
  void              on_update(const float dt,
                              World_objects &world_objs) override;

private:

  float             m_depth     = 0.f;
  float             m_point     = 0.f;
  const math::vec2  m_direction = math::vec2_init(0.f, 1.f);
  const float       m_speed     = 0.f;

};


} // ns


#endif // inc guard