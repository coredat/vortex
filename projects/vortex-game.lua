

function get_proj_root_dir()
  if(os.get() == "macosx") then
    --return "$(SRCROOT)/../"
    return "../"
  else
    return "../"
  end
end

solution "VortexApplication"

   configurations {
     "Debug",
     "Release"
   }

   project "VortexGameCode"

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

      libdirs {
        "/usr/local/lib/",
      }

      linkoptions {
        "-lsdl2",
        "-lsdl2_mixer",
        "-lSOIL",
      }

      links {
        "enet",
        "OpenGL.framework",
        "CoreFoundation.framework",
        "Foundation.framework",
        "CoreEngine",
      }

      includedirs {
        "../../core/3rdparty/sdl_mixer/include/",
        "../../core/3rdparty/bullet/include/",
        "../../core/3rdparty/enet/include",
        "/usr/local/include",
        "../../core/code/",
        "../../core/code/external/math",
        "../../core/code/external/graphics_api",
        "../../core/code/external/utilities",
        get_proj_root_dir() .. "code/",
      }

      buildoptions {
        "-std=c++14",
        "-stdlib=libc++",
      } --, "-framework OpenGL", "-framework CoreFoundation"}

      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }

      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }


   dofile("../../core/projects/core.lua")
