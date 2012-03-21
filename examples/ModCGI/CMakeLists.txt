cmake_minimum_required(VERSION 2.8)
project(ModCGI)

include_directories (${CMAKE_SOURCE_DIR}/../../include)

find_package(Boost 1.49.0 COMPONENTS algorithm FATAL_ERROR)
link_directories (${Boost_LIBRARY_DIRS})
include_directories (${Boost_INCLUDE_DIRS})

#
# Shared library
#
add_library(mod_cgi SHARED
  # Sources
  ModCGI.cpp
  
  ${Boost_LIBRARIES}
  )
