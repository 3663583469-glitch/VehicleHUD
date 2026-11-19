cmake_minimum_required(VERSION 3.20)
project(VehicleHUD VERSION 0.2.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SDL_INSTALL OFF CACHE BOOL "" FORCE)

# SDL2 has CMake support and is fetched from its official repository.
FetchContent_Declare(SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.11
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(SDL2)

# Dear ImGui is a source-only repository (no top-level CMakeLists.txt),
# so populate it and compile the required core + SDL2/OpenGL3 backends here.
FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.8
    GIT_SHALLOW TRUE)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()

add_library(imgui_lib STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
target_include_directories(imgui_lib PUBLIC ${imgui_SOURCE_DIR})
target_link_libraries(imgui_lib PUBLIC SDL2::SDL2)

find_package(OpenGL REQUIRED)
add_executable(VehicleHUD
    src/main.cpp
    src/Simulator.cpp)
target_include_directories(VehicleHUD PRIVATE src)
target_compile_definitions(VehicleHUD PRIVATE SDL_MAIN_HANDLED)
target_link_libraries(VehicleHUD PRIVATE SDL2::SDL2 imgui_lib OpenGL::GL)

if(MSVC)
    target_compile_options(VehicleHUD PRIVATE /W4 /permissive- /utf-8)
else()
    target_compile_options(VehicleHUD PRIVATE -Wall -Wextra -Wpedantic)
endif()
