cmake_minimum_required(VERSION 3.8)

PROJECT(dmicmp)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(cmake/ModuleImport.cmake)
INCLUDE(cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()
ModuleSetWinCompilerFlags()

InterfaceImport("dmicmp" "include" "")

if (PROJECT_IS_TOP_LEVEL)
    ExeImport("example" "dmicmp")
endif(PROJECT_IS_TOP_LEVEL)