#ifndef BULLET_INCLUDED_992A35A6_21D1_4D51_BF4E_06917B3704D3
#define BULLET_INCLUDED_992A35A6_21D1_4D51_BF4E_06917B3704D3


#include <game_objects/game_object.hpp>
#include <game_objects/game_objects_fwd.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <math/vec/vec_types.hpp>
#include <stdint.h>


namespace Game_object {


class Bullet : public Game_object
{
public:


  explicit          Bullet(Core::World &world, const float point, const uint32_t direction);
  
  void              on_start() override;
  bool              on_update(const float dt, World_objects &world_objs) override;

private:

  int32_t           direction         = -1;
  float             speed             = 100.5f;
  float             point_on_circle   = 1;
  Core::Entity_ref  owner;

};


} // ns



struct Bullets_container
{
  struct Bullet
  {
    int32_t           direction         = -1;
    float             speed             = 100.5f;
    float             point_on_circle   = 1;
    Core::Entity      entity;
    Core::Entity_ref  owner;
  };
  
  Bullet bullet[128];
  const uint32_t size = 128;
};


namespace Bullet_utils {


void
init_bullets(Core::World &world,
             Bullets_container &bullets_container);


void
move_bullets(Core::World &world,
             const float dt,
             Bullets_container &bullets_container);


void
create_bullet(Core::World &world,
              const float position,
              const float depth,              
              const uint32_t direction,
              Bullets_container &bullets_container);

};


#endif // inc guard