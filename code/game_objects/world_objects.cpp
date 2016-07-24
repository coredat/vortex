#include <game_objects/world_objects.hpp>
#include <stdint.h>


namespace Game_object {


World_objects::World_objects()
{
  m_start_objects.reserve(2048);
  m_update_objects.reserve(2048);
  m_end_objects.reserve(2048);
}


void
World_objects::push_object(Game_object *object)
{
  m_start_objects.push_back(object);
}


void
World_objects::on_start()
{
  uint32_t i = 0;
  
  while(i < m_start_objects.size())
  {
    m_start_objects[i]->on_start();
    m_update_objects.push_back(m_start_objects[i]);
    m_start_objects.erase(std::begin(m_start_objects) + i);
  }
}


void
World_objects::on_update(const float dt)
{
  for(auto &obj : m_update_objects)
  {
    obj->on_update(dt, *this);
  }
  
  // Gather objects for destruction.
  
  uint32_t i = 0;
  
  while(i < m_update_objects.size())
  {
    if(m_update_objects[i]->should_destroy())
    {
      m_end_objects.push_back(m_update_objects[i]);
      m_update_objects.erase(std::begin(m_update_objects) + i);
    }
    else
    {
      ++i;
    }
  }
}


void
World_objects::on_destroy()
{
  for(auto &obj : m_end_objects)
  {
    obj->on_end();
    obj->get_entity().set_user_data(0);
    obj->get_entity().destroy();
    
    delete obj;
  }
  
  m_end_objects.clear();
}


void
World_objects::send_event(const uint32_t evt_id, void *data)
{
  for(auto &obj : m_update_objects)
  {
    obj->on_message(evt_id, data);
  }
}


} // ns