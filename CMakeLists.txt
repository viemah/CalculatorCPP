cmake_minimum_required(VERSION 3.5)
project(calc)

set(CMAKE_CXX_STANDARD 11)

# Core executable
add_executable(calc src/main.cpp)
target_link_libraries(calc dl)

# Plugins
add_library(add SHARED plugins/add.cpp)
set_target_properties(add PROPERTIES PREFIX "" OUTPUT_NAME "add")
add_library(mul SHARED plugins/mul.cpp)
set_target_properties(mul PROPERTIES PREFIX "" OUTPUT_NAME "mul")
