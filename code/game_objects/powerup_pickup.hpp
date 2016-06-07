#ifndef POWERUP_PICKUP_INCLUDED_2CCA1A14_6DA1_497E_A3D9_1C1DFCA16DF9
#define POWERUP_PICKUP_INCLUDED_2CCA1A14_6DA1_497E_A3D9_1C1DFCA16DF9


#include <game_objects/game_object.hpp>
#include <core/world/world_fwd.hpp>


namespace Game_object {


class Powerup_pickup : public Game_object
{
public:

  explicit              Powerup_pickup(Core::World &world,
                                       const float point_on_cirlce,
                                       const float depth);
  
  void                  on_start() override;
  bool                  on_update(const float dt, World_objects &objs) override;
  void                  on_collision(Game_object *obj) override;
  
private:

  float                 m_point_on_circle = 0.f;
  float                 m_depth = 0.f;

};


} // ns


#endif // inc guard