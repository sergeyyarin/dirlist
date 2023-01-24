cmake_minimum_required(VERSION 3.0)

project(dirlist LANGUAGES C)

add_compile_options(-std=c99 -Wall)

add_executable(dirlist
    types.h
   ./file/file.h
   ./file/file.c
   ./list/list.h
   ./list/list.c
   ./str/str.h
   ./str/str.c
    main.c
)

target_link_libraries(dirlist m)