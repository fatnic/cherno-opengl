cmake_minimum_required(VERSION 2.8)
project(project)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lglfw -lGL -lGLEW -lX11 -lpthread -lXrandr -lXi -ldl")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#include_directories(include)
#include_directories(include/vendor)

include_directories(src)
include_directories(src/vendor)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
