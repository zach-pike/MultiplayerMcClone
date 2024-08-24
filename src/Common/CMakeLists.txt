cmake_minimum_required(VERSION 3.30.0)

file(GLOB_RECURSE COMMON_SOURCE *.cpp)
add_library(Common STATIC ${COMMON_SOURCE})
target_include_directories(Common AFTER PUBLIC .)