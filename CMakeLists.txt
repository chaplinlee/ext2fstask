cmake_minimum_required(VERSION 3.5)
project(ext2fstask)

set(CMAKE_C_STANDARD 11)

add_executable(dump_layout src/dump_layout.c)
add_executable(dump_del src/dump_del.c)
add_executable(del_undo src/del_undo.c)
