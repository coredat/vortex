#include <core/context/context.hpp>
#include <core/world/world.hpp>

#include <graphics_api/clear.hpp>


int
main()
{
  Core::Context context(800, 480, false, "Vortex");
  
  Core::World_setup setup;
  Core::World world(setup);
  
  Graphics_api::clear_color_set(1, 1, 0);
  
  while(context)
  {
    Graphics_api::clear(Graphics_api::Clear_flag::color | Graphics_api::Clear_flag::depth);  
  }

  return 0;
}