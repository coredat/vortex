#ifndef QUIT_INCLUDED_71EC62E2_A589_4EDA_8196_AED9FD345A5C
#define QUIT_INCLUDED_71EC62E2_A589_4EDA_8196_AED9FD345A5C


#include <game_states/game_state.hpp>


namespace Game {


class Quit : public State
{
public:

  explicit          Quit(Game_object::World_objects &objs,
                         Core::World &world,
                         Core::Context &ctx);
  
  const char *      get_name() const override;

};


} // ns



#endif // inc guard