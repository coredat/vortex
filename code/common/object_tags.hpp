#ifndef OBJECT_TAGS_INCLUDED_1570E972_0088_4423_B0B5_A532A6D97B50
#define OBJECT_TAGS_INCLUDED_1570E972_0088_4423_B0B5_A532A6D97B50


#include <utilities/bits.hpp>


/*
  We use this to help identify objects for things like
  aabb overlap etc.
*/


namespace Object_tags {
enum ENUM {

  bullet     = BIT(0),
  enemy      = BIT(1),
  player     = BIT(2),
  camera     = BIT(0),
  explosion  = BIT(4),
  powerup    = BIT(5),
  multiplier = BIT(6),
  gui_cam    = BIT(7),
  world_cam  = BIT(8),
  level_cam  = BIT(9),
  

}; // enum
} // ns


#endif // inc guard