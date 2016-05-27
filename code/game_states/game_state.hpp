#ifndef GAME_STATE_INCLUDED_FFA56966_4356_4752_B9F0_B830784BE167
#define GAME_STATE_INCLUDED_FFA56966_4356_4752_B9F0_B830784BE167


namespace Game_state {


class Game_state
{
public:

  explicit          Game_state();
  
  virtual void      on_enter();
  virtual void      on_exit();
  virtual void      on_update(const float dt);

};


} // ns


#endif // inc guard