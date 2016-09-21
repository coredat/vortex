#ifndef MATERIAL_FACTORY_INCLUDED_AF00E602_C2B9_4423_8B37_50661549DE1D
#define MATERIAL_FACTORY_INCLUDED_AF00E602_C2B9_4423_8B37_50661549DE1D


#include <core/common/core_fwd.hpp>


namespace Factory {
namespace Material {


Core::Material      get_logo();

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



} // ns
} // ns


#endif // inc guard