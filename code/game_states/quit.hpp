#ifndef QUIT_INCLUDED_
#define QUIT_INCLUDED_


#include <game_states/game_state.hpp>


namespace Game {


class Quit : public State
{
public:

  explicit          Quit(Game_object::World_objects &objs,
                         Core::World &world,
                         Core::Context &ctx);
  
  const char *      get_name() const override { return "Quit"; }

};


} // ns



#endif // inc guard