#ifndef HORIZON_INCLUDED_41CA6683_6510_4C31_904D_F75B3F8B7A9C
#define HORIZON_INCLUDED_41CA6683_6510_4C31_904D_F75B3F8B7A9C


#include <game_objects/game_object.hpp>


namespace Game_object {


class Horizon : public Game_object
{
public:

  explicit        Horizon(Core::World &world);
  
  void            on_update(const float dt,
                            World_objects &objs) override;

private:

  float           m_horizon_timer   = 5.5f;
  float           m_curr_horz_timer = 0.f;

};


} // ns


#endif // inc guard