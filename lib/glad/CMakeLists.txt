cmake_minimum_required(VERSION 3.22.1)

add_library(glad glad.c)
file(GLOB GLAD_HEADERS *.h)