cmake_minimum_required(VERSION 3.14)
project("chip8emu")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

add_executable(chip8emu src/main.cpp)

FetchContent_Declare(
    sdl2
    URL https://www.libsdl.org/release/SDL2-2.0.12.tar.gz
)
FetchContent_GetProperties(sdl2)

if(NOT sdl2_POPULATED)
    FetchContent_Populate(sdl2)
    add_subdirectory(${sdl2_SOURCE_DIR} ${sdl2_BINARY_DIR})
endif()

if (MSVC)
    set_target_properties(chip8emu PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    target_compile_options(chip8emu PUBLIC "$<$<BOOL:${MSVC}>:/permissive->")
endif(MSVC)

target_link_libraries(chip8emu SDL2-static SDL2main)