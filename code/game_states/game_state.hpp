#ifndef GAME_STATE_INCLUDED_
#define GAME_STATE_INCLUDED_


namespace Game_state {


class Game_state
{
public:

  explicit          Game_state();
  
  virtual void      on_update(const float dt);
  
  virtual void      on_enter();
  virtual void      on_exit();

};


} // ns


#endif // inc guard