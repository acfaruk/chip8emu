cmake_minimum_required(VERSION 3.14)
project("chip8emu")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

add_executable(chip8emu src/main.cpp)

# SDL2

set(SDL_STATIC_PIC ON CACHE BOOL "Position Independent Code" FORCE)
FetchContent_Declare(
    sdl2
    URL https://www.libsdl.org/release/SDL2-2.0.12.tar.gz
)
FetchContent_GetProperties(sdl2)

FetchContent_Populate(sdl2)
add_subdirectory(${sdl2_SOURCE_DIR} ${sdl2_BINARY_DIR})

target_link_libraries(chip8emu SDL2-static SDL2main)

# YAMP parser

set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "Enable yaml-cpp contrib in library" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "Enable parse tools" FORCE)
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "Tests" FORCE)
set(YAML_CPP_INSTALL OFF CACHE BOOL "Install" FORCE)

FetchContent_Declare(
    yaml_cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG yaml-cpp-0.6.3
)
FetchContent_GetProperties(yaml_cpp)

FetchContent_Populate(yaml_cpp)
add_subdirectory(${yaml_cpp_SOURCE_DIR} ${yaml_cpp_BINARY_DIR})


include_directories(${yaml_cpp_SOURCE_DIR}/include)
target_link_libraries(chip8emu yaml-cpp)


if (MSVC)
    set_target_properties(chip8emu PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    target_compile_options(chip8emu PUBLIC "$<$<BOOL:${MSVC}>:/permissive->")
endif(MSVC)
