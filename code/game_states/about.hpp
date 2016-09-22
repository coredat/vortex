#ifndef ABOUT_INCLUDED_A339A585_0152_4012_888E_FA26EE6DA61D
#define ABOUT_INCLUDED_A339A585_0152_4012_888E_FA26EE6DA61D


#include <game_states/game_state.hpp>
#include <common/game_state.hpp>
#include <core/common/core_fwd.hpp>
#include <game_objects/world_objects.hpp>
#include <core/entity/entity_ref.hpp>


namespace Game {


class About_screen : public State
{
public:

  explicit          About_screen(Game_object::World_objects &objs,
                                 Core::World &world,
                                 Core::Context &ctx);
  
  Game_state        on_update() override;

private:

  Core::Entity_ref  m_camera;
  
};


} // ns


#endif // inc guard