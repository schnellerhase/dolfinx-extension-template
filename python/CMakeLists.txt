cmake_minimum_required(VERSION 3.15)

project(abcFEM LANGUAGES C CXX)

if(NOT SKBUILD)
    message(
        WARNING
        "\
  This CMake file is meant to be executed using 'scikit-build'. Running
  it directly will almost certainly not produce the desired result. If
  you are a user trying to install this package, please use the command
  below, which will install all necessary build dependencies, compile
  the package in an isolated environment, and then install it.
  =====================================================================
   $ pip install .
  =====================================================================
  If you are a software developer, and this is your own package, then
  it is usually much more efficient to install the build dependencies
  in your environment once and use the following command that avoids
  a costly creation of a new virtual environment at every compilation:
  =====================================================================
   $ pip install nanobind scikit-build-core[pyproject]
   $ pip install --no-build-isolation -ve .
  =====================================================================
  You may optionally add -Ceditable.rebuild=true to auto-rebuild when
  the package is imported. Otherwise, you need to re-run the above
  after editing C++ files."
    )
endif()

option(ENABLE_CLANG_TIDY "Run clang-tidy while building" OFF)

# Try to import all Python components potentially needed by nanobind
find_package(
    Python
    3.8
    REQUIRED
    COMPONENTS Interpreter Development.Module
    OPTIONAL_COMPONENTS Development.SABIModule
)

find_package(nanobind CONFIG REQUIRED)
find_package(DOLFINX 0.9.0.0 REQUIRED)
find_package(extension_template REQUIRED)

# Check for DOLFINx python wrappers
execute_process(
    COMMAND
        ${Python_EXECUTABLE} -c "import dolfinx; print(dolfinx.get_include())"
    OUTPUT_VARIABLE DOLFINX_PY_WRAPPERS_DIR
    RESULT_VARIABLE DOLFINX_PY_WRAPPERS_COMMAND_RESULT
    ERROR_VARIABLE DOLFINX_PY_WRAPPERS_COMMAND_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT DOLFINX_PY_WRAPPERS_COMMAND_RESULT)
    message(
        STATUS
        "Found DOLFINx python wrappers at ${DOLFINX_PY_WRAPPERS_DIR}"
    )
else()
    message(
        FATAL_ERROR
        "DOLFINx python wrappers could not be found. Possibly caused by running without '--no-build-isolation'."
    )
endif()

set(FILES ${FILES} extension_template.cpp)

# Given the way nanobind builds its module, the clang-tidy checks are also run on these files if specified project wide.
if(ENABLE_CLANG_TIDY)
    foreach(FILE ${FILES})
        set_source_files_properties(
            ${FILE}
            PROPERTIES
                CMAKE_CXX_CLANG_TIDY
                    "clang-tidy;--config-file=${CMAKE_CURRENT_SOURCE_DIR}/../.clang-tidy"
        )
    endforeach()
endif()

nanobind_add_module(
  extension_template_mod
  NB_STATIC
  ${FILES}
)

target_link_libraries(extension_template_mod PRIVATE extension_template)

target_include_directories(
    extension_template_mod
    PRIVATE ${DOLFINX_PY_WRAPPERS_DIR}
)

set_target_properties(
    extension_template_mod
    PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE
)

nanobind_add_stub(
  extenstion_template_mod_stub
  MODULE extension_template_mod
  OUTPUT extension_template_mod.pyi
  PYTHON_PATH $<TARGET_FILE_DIR:extension_template_mod>
  DEPENDS extension_template_mod
)

install(TARGETS extension_template_mod LIBRARY DESTINATION extension_template)
install(TARGETS extension_template_mod DESTINATION extension_template)
install(
    FILES ${CMAKE_BINARY_DIR}/extension_template_mod.pyi
    DESTINATION extension_template/extension_template_mod
    RENAME __init__.pyi
)
