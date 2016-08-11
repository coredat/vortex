#include <game_objects/player.hpp>
#include <game_objects/player_ship.hpp>
#include <common/event_ids.hpp>
#include <common/object_tags.hpp>
#include <core/world/world.hpp>
#include <core/context/context.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/text_renderer.hpp>
#include <core/font/font.hpp>
#include <core/transform/transform.hpp>


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
    Player_ship *ship = new Player_ship(get_world(), ctx, m_controller_id - 1);

    Core::Material_renderer renderer;
    renderer.set_material(m_curr_material);
    renderer.set_model(m_curr_model);
    
    ship->get_entity().set_renderer(renderer);
    
    // Setup the renderer
    {
      Core::Font font("/Users/PhilCK/Desktop/font/LiberationSerif-Bold.ttf");
      
      Core::Text_renderer renderer;
      renderer.set_text("<>*");
      renderer.set_font(font);
      
      Core::Transform text_transform;
      
      m_counter.set_tags(Object_tags::gui_cam);
      m_counter.set_transform(text_transform);
      m_counter.set_renderer(renderer);
    }
    
    return ship;
  }
  
  return nullptr;
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