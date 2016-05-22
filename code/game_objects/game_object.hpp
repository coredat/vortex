#ifndef GAME_OBJECT_INCLUDED_2CB14921_92BB_4CBD_8249_46751E7DBDD9
#define GAME_OBJECT_INCLUDED_2CB14921_92BB_4CBD_8249_46751E7DBDD9


#include <game_objects/game_objects_fwd.hpp>
#include <core/world/world_fwd.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>


namespace Game_object {


class Game_object
{
public:

  explicit            Game_object(Core::World &world);
  virtual             ~Game_object();
  
  virtual void        on_start()                                      {}
  virtual bool        on_update(const float dt, World_objects &world) { return false; }
  virtual void        on_end()                                        {}
  virtual void        on_collision(Game_object *other)                {}
  
  bool                should_destroy() const;
  
  Core::Entity_ref    get_entity() const;
  
protected:

  void                destroy();
  Core::World         &get_world() const;
  
private:

  Core::World         &m_world;
  Core::Entity        m_entity;
  bool                m_should_destroy = false;

};


} // ns


#endif // inc guard