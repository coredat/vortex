

function get_proj_root_dir()
  if(os.get() == "macosx") then
    --return "$(SRCROOT)/../"
    return "../"
  else
    return "../"
  end
end

solution "Vortex Application"

   configurations {
     "Debug",
     "Release"
   }

   project "Vortex GameCode"

      kind "WindowedApp"
      language "C++"

      files {
        "../code/**.hpp",
        "../code/**.cpp",
        "../code/**.mm",
        "../code/**.h",
        "../code/**.c",
        "../code/**.m",
      }

      libdirs {
        "../../wired/code/",
        "/usr/local/lib/",
        "../3rdparty/bullet/lib/osx/",
        "../3rdparty/enet/lib/osx/",
      }

      linkoptions {
        "-lsdl2",
        "-lsdl2_mixer",
        "-lSOIL",
        "-lsimple_renderer",
        "-lbulletdynamics",
        "-lbulletcollision",
        "-llinearmath",
      }

      links {
        "enet",
        "OpenGL.framework",
        "CoreFoundation.framework",
        "Foundation.framework",
        "Core Engine",
      }

      includedirs {
        "../../wired/3rdparty/sdl_mixer/include/",
        "../../wired/3rdparty/bullet/include/",
        "../../wired/3rdparty/enet/include",
        "/usr/local/include",
        "../../wired/code/",
        "../../wired/code/external/math",
        "../../wired/code/external/graphics_api",
        "../../wired/code/external/utilities",
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


   dofile("../../wired/projects/wired.lua")
   --dofile("../projects/test.lua")
