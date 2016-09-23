#include <factories/material.hpp>
#include <core/resources/material.hpp>
#include <core/resources/shader.hpp>
#include <core/resources/texture.hpp>
#include <core/common/directory.hpp>


namespace {


/*
  Most textures in this game are just shader and texture
*/
inline Core::Material
generate_material(const char *mat_name,
                  const char *shader_path,
                  const char *texture_path)
{
  Core::Material material = Core::Material(mat_name);
  assert(material);
  
  Core::Shader shader(Core::Directory::volatile_resource_path(shader_path));
  assert(shader);
  material.set_shader(shader);
  
  Core::Texture texture(Core::Directory::volatile_resource_path(texture_path));
  assert(texture);
  material.set_map_01(texture);
  
  return material;
}


} // ns


namespace Factory {
namespace Material {


void
load_all()
{
  // Other
  get_logo();

  // Generic Menu
  get_menu_back_cold();
  get_menu_back_hot();
  get_menu_continue_cold();
  get_menu_continue_hot();

  // Title Screen
  get_title_menu_title();
  get_title_menu_start_cold();
  get_title_menu_start_hot();
  get_title_menu_options_cold();
  get_title_menu_options_hot();
  get_title_menu_about_cold();
  get_title_menu_about_hot();
  get_title_menu_quit_cold();
  get_title_menu_quit_hot();

  // Settings Screen
  get_settings_menu_title();
  get_settings_menu_fullscreen_cold();
  get_settings_menu_fullscreen_hot();

  // About Screen
  get_about_menu_title();

  // Selection
  get_selection_none_controller();
  get_selection_choose_ship_controller();

  // Players
  get_ship_01_material();
  get_ship_02_material();
  get_ship_03_material();
  get_ship_04_material();
}


Core::Material
get_logo()
{
  static Core::Material material;
  
  if(!material)
  {
    material = generate_material("[loading]logo",
                                 "assets/shaders/basic_fullbright.ogl",
                                 "assets/textures/repofa.png");
  }
  
  return material;
}


// Generic Menu
Core::Material
get_menu_back_cold()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[menu]back_cold",
                                 "assets/shaders/basic_fullbright.ogl",
                                 "assets/textures/button_back_cold.png");
  }

  return material;
}


Core::Material
get_menu_back_hot()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[menu]back_hot",
                                 "assets/shaders/basic_fullbright.ogl",
                                 "assets/textures/button_back_hot.png");
  }

  return material;
}


Core::Material
get_menu_continue_cold()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[menu]continue_cold",
                                 "assets/shaders/basic_fullbright.ogl",
                                 "assets/textures/title.png");
  }

  return material;
}


Core::Material
get_menu_continue_hot()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[menu]continue_hot",
                                 "assets/shaders/basic_fullbright.ogl",
                                 "assets/textures/title.png");
  }

  return material;
}


// Title Screen
Core::Material
get_title_menu_title()
{
  static Core::Material material;
  
  if(!material)
  {
    material = generate_material("[title_menu]title",
                                 "assets/shaders/basic_fullbright.ogl",
                                 "assets/textures/title.png");
  }
  
  return material;
}


Core::Material
get_title_menu_start_cold()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[title_menu]start_cold",
                                 "assets/shaders/basic_fullbright.ogl",
                                 "assets/textures/button_start_game_cold.png");
  }

  return material;
}


Core::Material
get_title_menu_start_hot()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[title_menu]start_hot",
                                 "assets/shaders/basic_fullbright.ogl",
                                 "assets/textures/button_start_game_hot.png");
  }

  return material;
}


Core::Material
get_title_menu_options_cold()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[title_menu]options_colds",
                                 "assets/shaders/basic_fullbright.ogl",
                                 "assets/textures/button_options_cold.png");
  }

  return material;
}


Core::Material
get_title_menu_options_hot()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[title_menu]options_hot",
                                 "assets/shaders/basic_fullbright.ogl",
                                 "assets/textures/button_options_hot.png");
  }

  return material;
}


Core::Material
get_title_menu_about_cold()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[title_menu]about_cold",
                                 "assets/shaders/basic_fullbright.ogl",
                                 "assets/textures/button_about_cold.png");
  }

  return material;
}


Core::Material
get_title_menu_about_hot()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[title_menu]about_hot",
                                 "assets/shaders/basic_fullbright.ogl",
                                 "assets/textures/button_about_hot.png");
  }

  return material;
}


Core::Material
get_title_menu_quit_cold()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[title_menu]quit_cold",
                                 "assets/shaders/basic_fullbright.ogl",
                                 "assets/textures/button_quit_cold.png");
  }

  return material;
}


Core::Material
get_title_menu_quit_hot()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[title_menu]quit_hot",
                                 "assets/shaders/basic_fullbright.ogl",
                                 "assets/textures/button_quit_hot.png");
  }

  return material;
}


// Settings Screen
Core::Material
get_settings_menu_title()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[settings_menu]settings_title",
                                 "assets/shaders/basic_fullbright.ogl",
                                 "assets/textures/button_options_cold.png");
  }

  return material;
}


Core::Material
get_settings_menu_fullscreen_cold()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[settings_menu]fullscreen_cold",
                                 "assets/shaders/basic_fullbright.ogl",
                                 "assets/textures/button_fullscreen_cold.png");
  }

  return material;
}


Core::Material
get_settings_menu_fullscreen_hot()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[settings_menu]fullscreen_hot",
                                 "assets/shaders/basic_fullbright.ogl",
                                 "assets/textures/button_fullscreen_hot.png");
  }

  return material;
}


// About Screen
Core::Material
get_about_menu_title()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[about_menu]about_title",
                                 "assets/shaders/basic_fullbright.ogl",
                                 "assets/textures/button_about_cold.png");
  }

  return material;
}



// Selection
Core::Material
get_selection_none_controller()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[selection]none",
                                 "assets/shaders/basic_fullbright.ogl",
                                 "assets/textures/to_join.png");
  }

  return material;
}


Core::Material
get_selection_choose_ship_controller()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[selection]choose",
                                 "assets/shaders/basic_fullbright.ogl",
                                 "assets/textures/select_ship.png");
  }

  return material;
}


// Players
Core::Material
get_ship_01_material()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[player]01",
                                 "assets/shaders/vortex_dir_light.ogl",
                                 "assets/textures/ship_01.png");
  }

  return material;
}


Core::Material
get_ship_02_material()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[player]02",
                                 "assets/shaders/vortex_dir_light.ogl",
                                 "assets/textures/ship_02.png");
  }

  return material;
}


Core::Material
get_ship_03_material()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[player]03",
                                 "assets/shaders/vortex_dir_light.ogl",
                                 "assets/textures/ship_03.png");
  }

  return material;
}


Core::Material
get_ship_04_material()
{
  static Core::Material material;

  if(!material)
  {
    material = generate_material("[player]04",
                                 "assets/shaders/vortex_dir_light.ogl",
                                 "assets/textures/ship_04.png");
  }

  return material;
}


} // ns
} // ns