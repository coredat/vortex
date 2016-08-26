#ifndef PLAYER_UI_INCLUDED_A1BC6961_A792_4B3F_BCAA_2B756D69BA6C
#define PLAYER_UI_INCLUDED_A1BC6961_A792_4B3F_BCAA_2B756D69BA6C


#include <game_objects/game_object.hpp>
#include <game_objects/game_objects_fwd.hpp>


namespace Game_object {


class Player_ui : public Game_object
{
public:

  explicit              Player_ui(Core::World &world,
                                  Core::Context &ctx,
                                  const Core::Camera &cam,
                                  const uint32_t controller_id);
  
  void                  on_start() override;
  void                  on_update(const float dt, World_objects &objs) override;
  
private:

}; // class


} // ns


#endif // inc guard