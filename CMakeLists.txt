cmake_minimum_required(VERSION 3.10)
project(SDL2CMake C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)

if(WIN32)
    set(SDL2_PLATFORM  "x64")
    set(SDL2_VERSION   "2.0.14")
    set(SDL2_PATH      ${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2-${SDL2_VERSION})
    set(SDL2_DEVEL_PKG SDL2-devel-${SDL2_VERSION}-VC.zip)

    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(SDL2_PLATFORM "x86")
    endif()

    include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

    ExternalProject_Add(SDL2_devel
        URL https://www.libsdl.org/release/${SDL2_DEVEL_PKG}
        URL_HASH SHA1=48d5dcd4a445410301f5575219cffb6de654edb8
        DOWNLOAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external
        DOWNLOAD_NO_PROGRESS true
        TLS_VERIFY true
        SOURCE_DIR ${SDL2_PATH}/
        BUILD_BYPRODUCTS ${SDL2_PATH}/lib/${SDL2_PLATFORM}/SDL2.lib

        BUILD_COMMAND cmake -E echo "Skipping build step."

        INSTALL_COMMAND cmake -E copy
        ${SDL2_PATH}/lib/${SDL2_PLATFORM}/SDL2.dll ${CMAKE_CURRENT_SOURCE_DIR}/out

        PATCH_COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CMakeLists_SDL2_devel.txt" ${SDL2_PATH}/CMakeLists.txt)

    set(SDL2_INCLUDE_DIR ${SDL2_PATH}/include)
    set(SDL2_LIBRARY     ${SDL2_PATH}/lib/${SDL2_PLATFORM}/SDL2.lib)

endif(WIN32)

find_package(SDL2 REQUIRED)

include_directories(
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
    SYSTEM ${SDL2_INCLUDE_DIRS})

set(Project_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_CURRENT_SOURCE_DIR}/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/out)

add_executable(
    ${PROJECT_NAME}
    ${Project_sources})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/out)

if(WIN32)
    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        ADDITIONAL_CLEAN_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/out/SDL2.dll)
endif(WIN32)

option(ENABLE_DIAGNOSTICS "Enable all diagnostics"           OFF)

target_link_libraries(
    ${PROJECT_NAME}
    ${SDL2_LIBRARIES})

if(UNIX)
    target_link_libraries(${PROJECT_NAME} m)
endif(UNIX)

if (CMAKE_C_COMPILER_ID     STREQUAL "Clang")
    set(COMPILE_OPTIONS
        -Wall
        -Wextra
        -Wpedantic)

elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(COMPILE_OPTIONS
        -Wall
        -Wextra
        -Wpedantic)

elseif (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(COMPILE_OPTIONS
        /W4)
endif()

if (CMAKE_C_COMPILER_ID STREQUAL "Clang" AND ENABLE_DIAGNOSTICS)
    message("Enabling all diagnostics")
    set(COMPILE_OPTIONS
        -Weverything)
    add_compile_options(-Weverything)
endif()
