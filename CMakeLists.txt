cmake_minimum_required(VERSION 3.17)
project(mini_laska C)

set(CMAKE_C_STANDARD 90)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -finline-functions")

add_executable(mini_laska main.c mini_laska.h mini_laska.c librerie/dynamic_vet.h librerie/dynamic_vet.c librerie/generic_dynamic_vet/dynarr.h librerie/generic_dynamic_vet/dynarr.c)

target_link_libraries(mini_laska m)