cmake_minimum_required(VERSION 3.22.1)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SERVER_SOURCE_FILES *.cpp)
set(CMAKE_CXX_STANDARD 20)

add_executable(Server ${SERVER_SOURCE_FILES})
target_include_directories(Server AFTER PUBLIC .)

IF(WIN32)
    target_link_libraries(Server STBImage ASIO wsock32 ws2_32 bcrypt Common)
else()
    target_link_libraries(Server STBImage ASIO Common)
endif()