#include <lib/menu/menu.hpp>


namespace Core {
namespace Lib {


void
Button::set_image(Core::Texture &texture)
{
}


void
Button::set_hover_image(Core::Texture &texture)
{
}


void
Button::set_active_image(Core::Texture &texture)
{
}


void
State::push_button(const Button &button)
{
  m_buttons.emplace_back(button);
}


void
Menu::push_state(Core::Lib::State *state)
{
  m_pending_state.push_back(state);
}


void
Menu::update()
{
}


} // ns
} // ns