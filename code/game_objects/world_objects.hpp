#ifndef WORLD_OBJECTS_CONTAINER_INCLUDED_67C5A3E9_D1B5_4FF1_BA75_95A2158C5D48
#define WORLD_OBJECTS_CONTAINER_INCLUDED_67C5A3E9_D1B5_4FF1_BA75_95A2158C5D48


#include <game_objects/game_object.hpp>
#include <vector>


namespace Game_object {


class World_objects
{
public:

  explicit                    World_objects();
  
  void                        push_object(Game_object *object);
  
  void                        on_start();
d  void                        on_destroy();
  
  
//private:

  std::vector<Game_object*>   m_start_objects;
  std::vector<Game_object*>   m_update_objects;
  std::vector<Game_object*>   m_end_objects;

};


} // ns


#endif // inc guard