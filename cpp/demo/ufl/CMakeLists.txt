cmake_minimum_required(VERSION 3.21)

project(demo_ufl LANGUAGES C CXX)

find_package(extension_template)
find_package(DOLFINX)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/form.c
    COMMAND
        ffcx ${CMAKE_CURRENT_SOURCE_DIR}/form.py -o ${CMAKE_CURRENT_SOURCE_DIR}
    VERBATIM
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/form.py
    COMMENT "Compile form.py using FFCx"
)

add_executable(demo_ufl main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/form.c)

set_property(TARGET demo_ufl PROPERTY CXX_STANDARD 20) # TODO: 23

target_link_libraries(demo_ufl PUBLIC extension_template dolfinx)
