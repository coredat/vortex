#include <common/settings.hpp>
#include <core/common/directory.hpp>
#include <utilities/directory.hpp>
#include <fstream>


namespace Common {


Settings
get_settings()
{
  Settings settings;
  {
    char file_path[2048];
    memset(file_path, 0, sizeof(file_path));
    strcat(file_path, util::dir::resource_path());
    strcat(file_path, "settings.txt");
  
    std::ifstream myfile(file_path);
    if (myfile.is_open())
    {
      std::string width;
      getline (myfile,width);
      if(!width.empty())
      {
        settings.width = std::stoi(width);
      }

      std::string height;
      getline (myfile,height);
      if(!height.empty())
      {
        settings.height = std::stoi(height);
      }
      
      std::string fullscreen;
      getline (myfile,fullscreen);
      if(!fullscreen.empty())
      {
        settings.is_fullscreen = !!std::stoi(fullscreen);
      }
      
      std::string monitor_pref;
      getline (myfile,monitor_pref);
      if(!monitor_pref.empty())
      {
        settings.monitor = std::stoi(monitor_pref);
      }

      myfile.close();
    }
  }
    
  return settings;
}


void
write_settings(const Settings &settings)
{
  char file_path[2048];
  memset(file_path, 0, sizeof(file_path));
  strcat(file_path, util::dir::resource_path());
  strcat(file_path, "settings.txt");

  FILE *log_file = fopen(file_path, "wb");
  
  if(log_file)
  {
    char settings_log[512];
    memset(settings_log, 0, sizeof(settings_log));
    
    sprintf(settings_log, "%d\n%d\n%d\n%d", settings.width, settings.height, settings.is_fullscreen, settings.monitor);
    
    fwrite(settings_log, sizeof(char), strlen(settings_log) * sizeof(char), log_file);
    fclose(log_file);
  }
}


} // ns