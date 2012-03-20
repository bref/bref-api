cmake_minimum_required(VERSION 2.8)
project(ModRewrite)

include_directories (${CMAKE_SOURCE_DIR}/../../include)

#
# Shared library
#
add_library(mod_rewrite SHARED
  # Sources
  ModRewrite.cpp
  )
