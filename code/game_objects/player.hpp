#ifndef PLAYER_INCLUDED_02A2DF2F_C236_421D_B50C_649D07B407A4
#define PLAYER_INCLUDED_02A2DF2F_C236_421D_B50C_649D07B407A4


#include <game_objects/game_object.hpp>
#include <game_objects/game_objects_fwd.hpp>
#include <core/common/core_fwd.hpp>
#include <core/resources/material.hpp>
#include <core/model/model.hpp>



namespace Game_object {


class Player : public Game_object
{
public:

  explicit        Player(Core::World &world);
  
  void            on_start() override;
  void            on_message(const uint32_t id, void *data) override;
  
  uint32_t        get_score() const;
  Player_ship*    spawn_ship(Core::Context &ctx);
  
  void            set_material(const Core::Material &mat);
  Core::Material  get_material() const;
  
  void            set_model(const Core::Model &model);
  Core::Model     get_model();
  
  void            set_controller(const uint32_t id);
  
  bool            is_valid() const;
  
private:

  uint32_t        m_score           = 0;
  uint32_t        m_controller_id   = UINT32_MAX;
  bool            m_is_active       = false;
  Core::Material  m_curr_material   = Core::Material("none");
  Core::Model     m_curr_model      = Core::Model();
  Core::Entity    m_counter         = Core::Entity();

}; // ns


} // ns


#endif // inc guard