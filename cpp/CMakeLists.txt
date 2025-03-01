cmake_minimum_required(VERSION 3.21)

project(extension_template VERSION "0.9.0.0")

set(CMAKE_PREFIX_PATH
    ${CMAKE_PREFIX_PATH}
    ${CMAKE_CURRENT_BINARY_DIR}/extension_template
)

if(CMAKE_VERSION VERSION_GREATER "3.24.0")
    set(CMAKE_COLOR_DIAGNOSTICS ON)
endif()

include(FeatureSummary)

option(BUILD_SHARED_LIBS "Build extension_template with shared libraries." ON)
add_feature_info(
    BUILD_SHARED_LIBS
    BUILD_SHARED_LIBS
    "Build extension_template with shared libraries."
)

option(
    CMAKE_INSTALL_RPATH_USE_LINK_PATH
    "Add paths to linker search and installed rpath."
    ON
)
add_feature_info(
    CMAKE_INSTALL_RPATH_USE_LINK_PATH
    CMAKE_INSTALL_RPATH_USE_LINK_PATH
    "Add paths to linker search and installed rpath."
)

option(BUILD_TESTING "Build tests, can be run with ctest" OFF)
add_feature_info(
    BUILD_TESTING
    BUILD_TESTING
    "Build tests, can be run with ctest"
)

find_package(DOLFINX 0.9.0.0 REQUIRED)
set_package_properties(
    DOLFINX
    PROPERTIES TYPE REQUIRED URL "https://github.com/FEniCS/dolfinx"
)
feature_summary(WHAT ALL)

add_library(extension_template)
set_property(TARGET extension_template PROPERTY CXX_STANDARD 23)
set_property(TARGET extension_template PROPERTY CXX_STANDARD_REQUIRED TRUE)

target_link_libraries(extension_template PUBLIC dolfinx)

include(GNUInstallDirs)

install(
    DIRECTORY extension_template/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/extension_template
    COMPONENT Development
    FILES_MATCHING
    PATTERN "*.hpp"
)

target_sources(
    extension_template
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/extension_template/extension_template.cpp
)

target_include_directories(
    extension_template
    PUBLIC
        $<INSTALL_INTERFACE:include>
        "$<BUILD_INTERFACE:${extension_template_SOURCE_DIR};${extension_template_SOURCE_DIR}/extension_template>"
)

set_target_properties(
    extension_template
    PROPERTIES
        VERSION ${extension_template_VERSION}
        SOVERSION
            ${extension_template_VERSION_MAJOR}.${extension_template_VERSION_MINOR}
)

target_compile_definitions(
    extension_template
    PUBLIC extension_template_VERSION="${extension_template_VERSION}"
)

install(
    TARGETS extension_template
    EXPORT extension_templateTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT RuntimeExecutables
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
)

install(
    EXPORT extension_templateTargets
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/extension_template
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${extension_template_SOURCE_DIR}/extension_templateConfig.cmake.in
    ${CMAKE_BINARY_DIR}/extension_template/extension_templateConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/extension_template
)

install(
    FILES ${CMAKE_BINARY_DIR}/extension_template/extension_templateConfig.cmake
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/extension_template
    COMPONENT Development
)
