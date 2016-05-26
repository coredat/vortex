#ifndef EXPLOSION_INCLUDED_0377F28D_5ECA_4177_88FD_311040A61AC5
#define EXPLOSION_INCLUDED_0377F28D_5ECA_4177_88FD_311040A61AC5


#include <game_objects/game_object.hpp>
#include <core/entity/entity.hpp>
#include <core/world/world_fwd.hpp>
#include <math/vec/vec_fwd.hpp>
#include <stdint.h>


namespace Game_object {


class Explosion : public Game_object
{
public:
  
  explicit        Explosion(Core::World &world, const math::vec3 position);
  void            on_start() override;
  bool            on_update(const float dt, World_objects &objs) override;

private:

  float m_time = 0.f;

};


} // ns


#endif // inc guard