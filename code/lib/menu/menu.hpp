#ifndef MENU_INCLUDED_752C3FED_D46D_488C_90B1_68B84E42DDCA
#define MENU_INCLUDED_752C3FED_D46D_488C_90B1_68B84E42DDCA


#include <core/common/core_fwd.hpp>
#include <core/entity/entity.hpp>
#include <core/resources/material.hpp>
#include <math/vec/vec_types.hpp>
#include <vector>


namespace Core {
namespace Lib {


class Menu
{
public:

  explicit          Menu();
  
  void              set_home(const math::vec2 location, const Core::Camera &camera);
  void              add_button(Core::World &world, const Core::Material &mat);
  void              clear();
  
  void              think(Core::Context &ctx, Core::World &world, const Core::Camera &camera);


private:


  struct Button
  {
    Core::Material  norm_material;
    Core::Material  hot_material;
    Core::Entity    entity;
  };

  math::vec3            m_home = math::vec3_zero();
  math::vec3            m_cursor = math::vec3_zero();
  std::vector<Button>   m_buttons;

};


} // ns
} // ns


#endif // inc guard