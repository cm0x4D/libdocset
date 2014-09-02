cmake_minimum_required(VERSION 2.8)
project(libDocset-tests)
include_directories(../src)

add_executable(find EXCLUDE_FROM_ALL find.cpp)
target_link_libraries(find Docset)
target_link_libraries(find sqlite3)
