#ifndef LEVEL_INCLUDED_C38EF587_A418_4228_8DEC_FC3C9E1307A6
#define LEVEL_INCLUDED_C38EF587_A418_4228_8DEC_FC3C9E1307A6


#include <game_objects/game_object.hpp>


namespace Game_object {


class Level : public Game_object
{
public:

  explicit        Level(Core::World &world);
  
  bool            on_update(const float dt, World_objects &objs) override;

private:

  float           m_horizon_timer   = 0.2f;
  float           m_curr_horz_timer = 0.f;

};


} // ns


#endif // inc guard