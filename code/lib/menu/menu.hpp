#ifndef MENU_INCLUDED_F752A89C_24A7_49AB_82C8_0E9C5985347E
#define MENU_INCLUDED_F752A89C_24A7_49AB_82C8_0E9C5985347E


#include <core/entity/entity.hpp>
#include <core/world/world.hpp>
#include <core/resources/texture.hpp>


namespace Core {
namespace Lib {


class Button
{
public:
  
  void set_image(Core::Texture &texture);
  void set_hover_image(Core::Texture &texture);
  void set_active_image(Core::Texture &texture);
  
private:

  Core::Texture m_image;
  Core::Texture m_hover_image;
  Core::Texture m_active_image;

};


class Menu;

typedef void (*State_interaction)(Menu *menu);


class State
{
public:

  void push_button(const Button &button);
  
  void on_interaction(const State_interaction *callback);
  
private:

  std::vector<Button> m_buttons;
  State_interaction *interaction_callback = nullptr;

};


class Menu
{
public:

  void push_state(State *state);
  
  void update();
  
private:

  std::vector<State*>   m_current_state;
  std::vector<State*>   m_pending_state;

};


} // ns
} // ns


#endif // inc guard