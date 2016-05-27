#ifndef BULLET_INCLUDED_992A35A6_21D1_4D51_BF4E_06917B3704D3
#define BULLET_INCLUDED_992A35A6_21D1_4D51_BF4E_06917B3704D3


#include <game_objects/game_object.hpp>
#include <game_objects/game_objects_fwd.hpp>
#include <stdint.h>


namespace Game_object {


class Bullet : public Game_object
{
public:

  explicit          Bullet(Core::World &world,
                           const float point,
                           const float depth,
                           const uint32_t direction);
  
  bool              on_update(const float dt, World_objects &world_objs) override;

private:

  int32_t           m_direction         = -1;
  float             m_speed             = 100.5f;
  float             m_point_on_circle   = 1;
  Core::Entity_ref  m_owner;

};


} // ns


#endif // inc guard