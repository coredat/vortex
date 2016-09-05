#ifndef BUTTON_INCLUDED_CC4D8545_A90F_4CB9_A805_DF1456329A01
#define BUTTON_INCLUDED_CC4D8545_A90F_4CB9_A805_DF1456329A01


#include <core/common/core_fwd.hpp>
#include <core/resources/material.hpp>
#include <core/entity/entity.hpp>


namespace Core {
namespace Lib {


class Button
{
public:

  explicit            Button(Core::World &world,
                             Core::Context &ctx,
                             const math::vec3 position,
                             const Core::Texture &hot_texture,
                             const Core::Texture &cold_texture);
  
  bool                was_touched();
  bool                is_over();
  
private:

  Core::Material      m_cold_material;
  Core::Material      m_hot_material;
  Core::Entity        m_entity;

}; // class


} // ns
} // ns


#endif // inc guard