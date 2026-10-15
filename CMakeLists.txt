cmake_minimum_required(VERSION 3.16)
project(heartReal C)
set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

add_executable(heartReal main.c)
target_include_directories(heartReal PRIVATE ${GTK4_INCLUDE_DIRS})
target_link_directories(heartReal PRIVATE ${GTK4_LIBRARY_DIRS})
target_compile_options(heartReal PRIVATE ${GTK4_CFLAGS_OTHER})
target_link_libraries(heartReal PRIVATE ${GTK4_LIBRARIES} m)