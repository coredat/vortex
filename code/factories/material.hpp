#ifndef MATERIAL_FACTORY_INCLUDED_AF00E602_C2B9_4423_8B37_50661549DE1D
#define MATERIAL_FACTORY_INCLUDED_AF00E602_C2B9_4423_8B37_50661549DE1D


#include <core/common/core_fwd.hpp>


/*
  Factory generates all the materials used in the game.
*/
namespace Factory {
namespace Material {


Core::Material      get_logo();

// Generic Menu
Core::Material      get_menu_back_cold();
Core::Material      get_menu_back_hot();
Core::Material      get_menu_continue_cold();
Core::Material      get_menu_continue_hot();

// Title Screen
Core::Material      get_title_menu_title();
Core::Material      get_title_menu_start_cold();
Core::Material      get_title_menu_start_hot();
Core::Material      get_title_menu_options_cold();
Core::Material      get_title_menu_options_hot();
Core::Material      get_title_menu_about_cold();
Core::Material      get_title_menu_about_hot();
Core::Material      get_title_menu_quit_cold();
Core::Material      get_title_menu_quit_hot();

// Settings Screen
Core::Material      get_settings_menu_title();
Core::Material      get_settings_menu_fullscreen_cold();
Core::Material      get_settings_menu_fullscreen_hot();

// About Screen
Core::Material      get_about_menu_title();

// Selection
Core::Material      get_selection_none_controller();
Core::Material      get_selection_choose_ship_controller();

// Players
Core::Material      get_ship_01_material();
Core::Material      get_ship_02_material();
Core::Material      get_ship_03_material();
Core::Material      get_ship_04_material();


} // ns
} // ns


#endif // inc guard