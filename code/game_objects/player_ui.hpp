#ifndef PLAYER_UI_INCLUDED_A1BC6961_A792_4B3F_BCAA_2B756D69BA6C
#define PLAYER_UI_INCLUDED_A1BC6961_A792_4B3F_BCAA_2B756D69BA6C


#include <game_objects/game_object.hpp>
#include <game_objects/game_objects_fwd.hpp>


namespace Game_object {


namespace Units {
enum ENUM {
  single = 0,
  tens,
  hundreds,
  
  size,
};
}


class Player_ui : public Game_object
{
public:

enum class Ui_type {
  in_game_ui,
  game_over_ui,
};


  explicit              Player_ui(Core::World &world,
                                  Core::Context &ctx,
                                  const Core::Camera &gui_cam,
                                  const Core::Camera &world_cam,
                                  const uint32_t controller_id,
                                  const Ui_type ui_type = Ui_type::in_game_ui);
  
  void                  on_start() override;
  void                  on_end() override;
  void                  on_update(const float dt, World_objects &objs) override;
  void                  set_score(const uint32_t score);
  
//private:

  Core::Entity          m_counters[Units::size];
  Core::Entity          m_avatar;
  uint32_t              m_score = 0;
  uint32_t              m_corner = 0;
  math::vec3            m_screen_pos = math::vec3_zero();
  Ui_type               m_ui_type;
  

}; // class


} // ns


#endif // inc guard