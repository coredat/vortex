#!lua

package.path = '../../core/projects/?.lua;' .. package.path
package.path = '../../premake_data/?.lua;' .. package.path

require "premake_data"

dofile("../../core/projects/core.lua")
dofile("vortex-game.lua")


-- Solution data? Should this contain the the project names?
solution_data = {

  name = "VortexApplication"

}

-- Defaults will be based on platform in the future.
-- Currently only supports buildoptions.
project_defaults = {

  buildoptions_macosx = {
    "-std=c++14",
    "-stdlib=libc++",
  },
}

-- Generates the premake code calls.
make.create_solution(solution_data,
                     project_defaults,
                     vortex_project_data,
                     core_project_data);
