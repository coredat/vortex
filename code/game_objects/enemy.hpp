#ifndef ENEMY_INCLUDED_C60D1C26_65C3_4BF9_AAA6_23A27358D168
#define ENEMY_INCLUDED_C60D1C26_65C3_4BF9_AAA6_23A27358D168


#include <game_objects/game_object.hpp>
#include <game_objects/game_objects_fwd.hpp>
#include <core/world/world_fwd.hpp>
#include <core/entity/entity.hpp>
#include <stdint.h>


namespace Game_object {


class Enemy : public Game_object
{
public:

  enum class Type
  {
    climber,  // Climbs up and down.
    breeder,  // Climbs to the top and drops eggs.
    egg,      // Spaned by the breader.
    
    size // nothing under here.
  };
  
  explicit        Enemy(Core::World &world, Type type);
  void            on_start() override;
  bool            on_update(const float dt, World_objects &objs) override;
  void            on_collision(Game_object::Game_object *other) override;

//private:

  float           m_point_on_circle = 0;
  float           m_depth = 0;
  float           m_lifetime = 0;
  int32_t         m_direction = 1;
  Type            m_type = Type::climber;
  
  bool            go_boom = false;

};


} // ns


#endif // inc guard