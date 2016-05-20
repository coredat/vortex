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


bool
Game_object::should_destroy() const
{
  return m_should_destroy;
}


void
Game_object::destroy()
{
  m_should_destroy = true;
  get_entity().destroy();
}


Core::World&
Game_object::get_world() const
{
  return m_world;
}


} // ns