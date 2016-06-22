#include <game_objects/level.hpp>
#include <game_objects/world_objects.hpp>
#include <common/level_functions.hpp>
#include <common/object_tags.hpp>
#include <core/world/world.hpp>
#include <core/resources/texture.hpp>
#include <core/resources/material.hpp>
#include <core/resources/shader.hpp>
#include <core/model/model.hpp>
#include <core/color/color.hpp>
#include <core/color/color_predefined.hpp>
#include <core/transform/transform.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <math/general/general.hpp>
#include <utilities/directory.hpp>
#include <stdio.h>


namespace
{
  Core::Entity level_slider[18];
  Core::Entity outter_level_sliders[4];
}


namespace Game_object {


Level::Level(Core::World &world)
: Game_object(world)
{
  Core::Entity_ref ref = get_entity();

  // Create a material
  Core::Material level_material("Level");
  {
    const std::string grey_texture_path = util::get_resource_path() + "assets/textures/dev_grid_grey_512.png";
    Core::Texture texture(grey_texture_path.c_str());

    const std::string shader_path = util::get_resource_path() + "assets/shaders/vortex_level.ogl";
    Core::Shader shader(shader_path.c_str());
    
    level_material.set_shader(shader);
    level_material.set_map_01(texture);
    level_material.set_color(Core::Color_utils::magenta());
  }
  
  // Create the level bits
  for(uint32_t i = 0; i < 18; ++i)
  {
    level_slider[i] = Core::Entity(world);
    
//    Core::Entity_ref slider_ref = level_slider[i];
  
    level_slider[i].set_name("Level");
    
    char level_number[2];
    
    sprintf(level_number, "%02d", i + 1);
    
    const std::string level_path = util::get_resource_path() + "assets/models/inner_level_" + level_number + ".obj";
    Core::Model model(level_path.c_str());

    constexpr float scale = 20.5f;
    
    Core::Transform trans(math::vec3_init(0.f, 0.f, -50.f),
                          math::vec3_init(scale, scale, scale * 2.f),
                          math::quat_init());
    
    level_slider[i].set_material(level_material);
    level_slider[i].set_model(model);
    level_slider[i].set_transform(trans);
    level_slider[i].add_tag(Object_tags::world_cam);
  }
  
  // Create the level bits
  for(uint32_t i = 0; i < 4; ++i)
  {
    outter_level_sliders[i] = Core::Entity(world);
    
//    Core::Entity_ref slider_ref = level_slider[i];
  
    outter_level_sliders[i].set_name("Level");
    
    char level_number[2];
    
    sprintf(level_number, "%02d", i + 1);
    
    const std::string level_path = util::get_resource_path() + "assets/models/outter_level_" + level_number + ".obj";
    Core::Model model(level_path.c_str());

    constexpr float scale = 45.5f;
    
    Core::Transform trans(math::vec3_init(0.f, 0.f, -70.f),
                          math::vec3_init(scale, scale, scale * 2.f),
                          math::quat_init());
    
    outter_level_sliders[i].set_material(level_material);
    outter_level_sliders[i].set_model(model);
    outter_level_sliders[i].set_transform(trans);
    outter_level_sliders[i].add_tag(Object_tags::world_cam);
  }
}


void
Level::on_update(const float dt, World_objects &objs)
{
  static float total_time = 0;
  total_time += dt;

  for(uint32_t i = 0; i < 18; ++i)
  {
    Core::Transform trans = level_slider[i].get_transform();
    
    srand(i);
    const float vel = static_cast<float>((rand() % 100) - 50) * total_time * 0.04f;
    
    math::quat rot = math::quat_init_with_axis_angle(0, 0, 1, vel);
    trans.set_rotation(rot);
    
    level_slider[i].set_transform(trans);
  }
  
  for(uint32_t i = 0; i < 4; ++i)
  {
    Core::Transform trans = outter_level_sliders[i].get_transform();
    
    srand(i);
    const float vel = static_cast<float>((rand() % 100) - 50) * total_time * 0.01f;
    
    math::quat rot = math::quat_init_with_axis_angle(0, 0, 1, vel);
    trans.set_rotation(rot);
    
    outter_level_sliders[i].set_transform(trans);
  }
  
  srand(total_time);
}


} // ns