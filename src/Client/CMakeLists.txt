cmake_minimum_required(VERSION 3.22.1)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE CLIENT_SOURCE_FILES *.cpp)
set(CMAKE_CXX_STANDARD 20)

add_executable(Client ${CLIENT_SOURCE_FILES})

target_include_directories(Client AFTER PUBLIC ../../lib/)
target_include_directories(Client AFTER PUBLIC .)

target_link_directories(Client BEFORE PUBLIC ../)
target_link_libraries(Client glad glfw3 OpenGL::GL STBImage IMGui ASIO wsock32 ws2_32 bcrypt Common)