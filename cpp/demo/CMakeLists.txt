cmake_minimum_required(VERSION 3.21)

project(extension_template-demos)

include(FeatureSummary)

option(ENABLE_CLANG_TIDY "Run clang-tidy while building" OFF)
add_feature_info(
    ENABLE_CLANG_TIDY
    ENABLE_CLANG_TIDY
    "Run clang-tidy while building"
)

if(ENABLE_CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY
        "clang-tidy;--config-file=${CMAKE_CURRENT_SOURCE_DIR}/../../.clang-tidy"
    )
endif()

add_subdirectory(simple)
