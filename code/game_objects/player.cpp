#include <game_objects/player.hpp>
#include <game_objects/player_ui.hpp>
#include <game_objects/player_ship.hpp>
#include <common/event_ids.hpp>
#include <common/object_tags.hpp>
#include <core/world/world.hpp>
#include <core/context/context.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/text_renderer.hpp>
#include <core/font/font.hpp>
#include <core/entity/entity.hpp>
#include <core/transform/transform.hpp>
#include <utilities/logging.hpp>


namespace Game_object {


Player::Player(Core::World &world)
: Game_object(world)
, m_counter(world)
{
}


void
Player::on_start()
{

}


void
Player::on_message(const uint32_t id, void *data)
{
  if(id == Event_id::player_destroyed_enemy)
  {
    if(*reinterpret_cast<uint32_t*>(data) == m_controller_id)
    {
      ++m_score;
    }
  }
}


void
Player::set_controller(const uint32_t id)
{
  m_controller_id = id + 1;
}


Player_ship*
Player::spawn_ship(Core::Context &ctx)
{
  if(m_controller_id)
  {
    Player_ship *ship = new Player_ship(get_world(), ctx, m_controller_id);
    
    m_player_ref = ship->get_entity();

    Core::Material_renderer renderer;
    renderer.set_material(m_curr_material);
    renderer.set_model(m_curr_model);
    
    ship->get_entity().set_renderer(renderer);
    
    // Setup the renderer
    {
      //Core::Font font("/Users/PhilCK/Desktop/font/LiberationSerif-Bold.ttf");

      //LOG_ERROR("Renderer not getting unset");
    }
    
    return ship;
  }
  
  return nullptr;
}

Player_ui*
Player::spawn_ui(Core::Context &ctx, Core::Camera &cam)
{
  if(m_controller_id)
  {
    Player_ui *ui = new Player_ui(get_world(), ctx, cam, m_controller_id);
    
    m_ui_ref = ui->get_entity();
    
    return ui;
  }
  
  return nullptr;
}


void
Player::clear_ui_and_ship()
{
  m_ui_ref.destroy();
  m_player_ref.destroy();
}


void
Player::set_material(const Core::Material &mat)
{
  m_curr_material = mat;
}


Core::Material
Player::get_material() const
{
  return m_curr_material;
}


void
Player::set_model(const Core::Model &model)
{
  m_curr_model = model;
}


Core::Model
Player::get_model()
{
  return m_curr_model;
}


bool
Player::is_valid() const
{
  return m_controller_id != UINT32_MAX;
}


} // ns