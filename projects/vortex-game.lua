#!lua

vortex_project_data = {

  name     = "VortexGameCode",
  kind     = "WindowedApp",
  language = "C++",
  location = make.get_proj_root(),

  src_files = {
    make.get_proj_root() .. "../code/**.hpp",
    make.get_proj_root() .. "../code/**.cpp",
    make.get_proj_root() .. "../code/**.mm",
    make.get_proj_root() .. "../code/**.h",
    make.get_proj_root() .. "../code/**.c",
    make.get_proj_root() .. "../code/**.m",
    make.get_proj_root() .. "../code/**.md",
  },

  lib_dirs_macosx = {
    "/usr/local/lib/",
  },

  project_dependencies = { -- This should pick up the links
    "CoreEngine",
  },

  inc_dirs_macosx = {
    "/usr/local/include",
  },

  inc_dirs = {
    make.get_proj_root() .. "../code/",
    -- Core Includes.
    make.get_proj_root() .. "../../core/code/",
    make.get_proj_root() .. "../../core/code/external/math",
    make.get_proj_root() .. "../../core/code/external/graphics_api", -- Do i need this?
    make.get_proj_root() .. "../../core/code/external/utilities",
  },
}
