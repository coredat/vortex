#include <factories/material.hpp>
#include <core/resources/material.hpp>
#include <core/resources/shader.hpp>
#include <core/resources/texture.hpp>
#include <core/common/directory.hpp>


namespace {


inline Core::Material
generate_material(const char *mat_name, const char *shader_path, const char *texture_path)
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


Core::Material
get_logo()
{
  static Core::Material material;
  
  if(!material)
  {
    material = generate_material("logo",
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


} // ns
} // ns