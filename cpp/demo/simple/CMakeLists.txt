cmake_minimum_required(VERSION 3.21)

project(demo_simple LANGUAGES CXX)

find_package(extension_template)
find_package(DOLFINX)

add_executable(demo_simple main.cpp)

set_property(TARGET demo_simple PROPERTY CXX_STANDARD 23)

target_link_libraries(demo_simple PRIVATE extension_template dolfinx)
