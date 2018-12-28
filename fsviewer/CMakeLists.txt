cmake_minimum_required(VERSION 3.5.1)
project(fsviewer)

set(CMAKE_C_STANDARD 11)

add_executable(dump_layout dump_layout.c)
add_executable(dump_del dump_del.c)
add_executable(del_undo del_undo.c)
