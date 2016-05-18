#include <game_objects/game_object.hpp>
#include <core/world/world.hpp>


namespace Game_object {


Game_object::Game_object(Core::World &world)
: m_world(world)
, m_entity(m_world)
{
  m_entity.set_user_data((uintptr_t)this);
}


Game_object::~Game_object()
{
}


Core::Entity_ref
Game_object::get_entity() const
{
  return m_entity;
}


} // ns