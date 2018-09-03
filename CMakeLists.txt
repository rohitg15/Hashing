cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)

project(myProject)

add_subdirectory(main)
add_subdirectory(libmurmur)
add_subdirectory(libutils)
add_subdirectory(libcollisions)