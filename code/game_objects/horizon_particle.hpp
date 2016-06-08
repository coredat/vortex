#ifndef HORIZON_PARTICLE_INCLUDED_85A5E903_76B5_4288_81B0_E5C83E774D4F
#define HORIZON_PARTICLE_INCLUDED_85A5E903_76B5_4288_81B0_E5C83E774D4F


#include <game_objects/game_objects_fwd.hpp>
#include <game_objects/game_object.hpp>
#include <core/context/context_fwd.hpp>
#include <core/world/world_fwd.hpp>


namespace Game_object {


class Horizon_particle : public Game_object
{
public:

  explicit            Horizon_particle(Core::World &world);

  void                on_start() override;
  void                on_update(const float dt, World_objects &objs) override;

private:

  const float         m_horizon_speed = 0.f;

};


} // ns


#endif // inc guard