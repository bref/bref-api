cmake_minimum_required(VERSION 2.8)
project(ModHello)

include_directories (${CMAKE_SOURCE_DIR}/../../include)

#
# Shared library
#
add_library(mod_hello SHARED
  # Sources
  ModHello.h
  ModHello.cpp
  )
