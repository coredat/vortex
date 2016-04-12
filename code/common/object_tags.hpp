#ifndef OBJECT_TAGS_INCLUDED_1570E972_0088_4423_B0B5_A532A6D97B50
#define OBJECT_TAGS_INCLUDED_1570E972_0088_4423_B0B5_A532A6D97B50


/*
  We use this to help identify objects for things like
  aabb overlap etc.
*/


namespace Object_tags {
enum ENUM {


  bullet    = 1 << 0,
  enemy     = 1 << 1,
  player    = 1 << 2,
  camera    = 1 << 3,
  explosion = 1 << 4,


}; // enum
} // ns


#endif // inc guard