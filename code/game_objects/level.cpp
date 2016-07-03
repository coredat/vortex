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
#include <core/common/directory.hpp>
#include <utilities/directory.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <stdio.h>


namespace
{
  struct Slider
  {
    Core::Entity entity;
    float velocity = 1.f;
    float spin_mul = 1.f;
    float spin_offset = 1.f;
  };
  
  constexpr uint32_t number_of_sliders = 18;
  Slider inner_sliders[number_of_sliders];

  Core::Entity level_slider[18];
  Core::Entity outter_level_sliders[4];
}


namespace
{
  inline void
  update_position(const float dt)
  {
    static float total_time = 0;
    total_time += dt;

    for(uint32_t i = 0; i < number_of_sliders; ++i)
    {
      auto &slider = inner_sliders[i];

      Core::Transform trans = slider.entity.get_transform();
      
      const float final_vel = slider.spin_mul * math::sin(slider.spin_offset + total_time * slider.velocity);
      
      math::quat rot = math::quat_init_with_axis_angle(0, 0, 1, final_vel);
      trans.set_rotation(rot);
      
      slider.entity.set_transform(trans);
    }
  }
}


namespace Game_object {


Level::Level(Core::World &world)
: Game_object(world)
{
  Core::Entity_ref ref = get_entity();

  // Create a material
  Core::Material level_material("Level");
  {
    const char * grey_texture_path = Core::Directory::resource_path("assets/textures/dev_grid_grey_512.png");
    Core::Texture texture(grey_texture_path);

    const char * shader_path = Core::Directory::resource_path("assets/shaders/vortex_level.ogl");
    Core::Shader shader(shader_path);
    
    level_material.set_shader(shader);
    level_material.set_map_01(texture);
    level_material.set_color(Core::Color_utils::magenta());
  }
  
  // Create the level bits
  for(uint32_t i = 0; i < number_of_sliders; ++i)
  {
    auto &slider = inner_sliders[i];
  
    slider.entity = Core::Entity(world);
    slider.entity.set_name("Level Inner Slider");
    
    slider.spin_mul = static_cast<float>(rand() % 50) / 100.f + 1.f;
    slider.spin_offset = static_cast<float>(rand() % 100);
    
    char level_model[2048];
    sprintf(level_model, "%sassets/models/inner_level_%02d.obj", util::dir::resource_path(), i + 1);

    Core::Model model(level_model);

    constexpr float offset = 1.1f;
    
    Core::Transform trans(math::vec3_init(0.f, 0.f, -3.f),
                          math::vec3_init(Level_funcs::get_radius() * offset, Level_funcs::get_radius() * offset, 20.f),
                          math::quat_init());
    
    Core::Material_renderer mat_renderer;
    mat_renderer.set_material(level_material);
    mat_renderer.set_model(model);
    
    slider.entity.set_renderer(mat_renderer);
  
    slider.entity.set_transform(trans);
    slider.entity.add_tag(Object_tags::world_cam);
  }
  
  update_position(0.f);
}


void
Level::on_update(const float dt, World_objects &objs)
{
  update_position(dt);
}


} // ns