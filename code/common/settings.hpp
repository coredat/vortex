#ifndef SETTINGS_INCLUDED_4310DDAB_0D42_46E3_AB98_32AE40BD81FB
#define SETTINGS_INCLUDED_4310DDAB_0D42_46E3_AB98_32AE40BD81FB


#include <core/common/core_fwd.hpp>


namespace Common {


struct Settings
{
  uint32_t width     = 1024;
  uint32_t height    = 576;
  bool is_fullscreen = false;
  uint32_t monitor   = 0;
};


Settings
get_settings();


void
write_settings(const Settings &settings);


} // ns


#endif // inc guard