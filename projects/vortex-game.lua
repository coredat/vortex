

function get_solution_root_dir()
  if(os.get() == "macosx") then
    --return "$(SRCROOT)/../"
    return "../"
  else
    return "../"
  end
end


solution "VortexApplication"
  location("./")

   configurations {
     "Debug",
     "Release"
   }

   project "VortexGameCode"
      location("./")

      kind "WindowedApp"
      language "C++"

      files {
        "../code/**.hpp",
        "../code/**.cpp",
        "../code/**.mm",
        "../code/**.h",
        "../code/**.c",
        "../code/**.m",
        "../code/**.md",
      }

      if(os.get() == "macosx") then
        libdirs {
          "/usr/local/lib/",
          -- "../../core/3rdparty/SDL/lib/win32/",
          -- "../../core/3rdparty/GLEW/lib/win32/",
          -- "../../core/3rdparty/SOIL/lib/win32/",
        }
      else
      libdirs {
        "/usr/local/lib/",
        "../../core/3rdparty/sdl/lib/win32/",
        "../../core/3rdparty/glew/lib/win32/",
        "../../core/3rdparty/soil/lib/win32/",
      }
      end

      if(os.get() == "macosx") then
      linkoptions {
        "-lsdl2",
        "-lsdl2_mixer",
        "-lSOIL",
      }
      end

      if(os.get() == "macosx") then
      links {
        "OpenGL.framework",
        "CoreFoundation.framework",
        "Foundation.framework",
        "CoreEngine",
      }
      else
      links {
        "CoreEngine",
        "glew32.lib",
        "sdl2.lib",
        "SOIL.lib",
        "opengl32.lib",
      }
      end

      includedirs {
        "../../core/3rdparty/sdl_mixer/include/",
        "../../core/3rdparty/bullet/include/",
        "../../core/3rdparty/enet/include",
        "/usr/local/include",
        "../../core/code/",
        "../../core/code/external/math",
        "../../core/code/external/graphics_api",
        "../../core/code/external/utilities",
        get_solution_root_dir() .. "code/",
      }

      if(os.get() == "macosx") then
      buildoptions {
        "-std=c++14",
        "-stdlib=libc++",
      }
      end

      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols", "Unicode"}

      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize", "Unicode" }


   dofile("../../core/projects/core.lua")
