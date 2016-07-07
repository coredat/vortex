#!lua

package.path = '../../core/projects/?.lua;' .. package.path
package.path = '../../premake_data/?.lua;' .. package.path

require "premake_data"

dofile("../../core/projects/core.lua")
dofile("vortex-game.lua")


-- Solution data? Should this contain the the project names?
solution_data = {

  name = "VortexApplication",

}

-- Defaults will be based on platform in the future.
-- Currently only supports buildoptions.
project_defaults = {

  buildoptions_macosx = {
    "-std=c++14",
    "-stdlib=libc++",
  },

  buildoptions_windows = {
    "/IGNORE:C4577",
  },

  flags = {
    "EnableSSE2",
    "ExtraWarnings",
    "FloatFast",
    "NoExceptions", -- deprecated
    "NoRTTI", -- deprecated
  },

  defines = {
    "CORE_USE_SOIL",
    "CORE_DEBUG_MENU",
  },

  defines_macosx = {
    "MATH_USE_SIMD",
  },

  defines_windows = {
    "_HAS_EXCEPTIONS=0",
    "_CRT_NO_WARNINGS",
  },

  exceptions = false,
  rtti = false,
}

-- Generates the premake code calls.
make.create_solution(solution_data,
                     project_defaults,
                     vortex_project_data,
                     core_project_data);
