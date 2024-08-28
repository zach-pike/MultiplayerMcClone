cmake_minimum_required(VERSION 3.22.1)

project(Multiplayer_Minecraft_Clone)

set(IMGUI_SOURCE 
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)
add_library(ImGui STATIC ${IMGUI_SOURCE})
target_include_directories(ImGui AFTER PUBLIC ./imgui/)
target_include_directories(ImGui AFTER PUBLIC ./)

# ASIO
set(ASIO_SOURCE asio/asio/src/asio.cpp)

add_library(ASIO STATIC ${ASIO_SOURCE})
target_compile_definitions(ASIO PUBLIC _WIN32_WINNT=0x0A00)
target_compile_definitions(ASIO PRIVATE ASIO_SEPARATE_COMPILATION)

target_include_directories(ASIO AFTER PUBLIC ./asio/asio/include/)

# Zlib
file(GLOB ZLIB_SOURCE zlib/*.c)
add_library(ZLib STATIC ${ZLIB_SOURCE})
target_include_directories(ZLib AFTER PUBLIC ./zlib/)

add_subdirectory(src/)
add_subdirectory(lib/)