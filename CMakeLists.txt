cmake_minimum_required(VERSION 3.1)

project(icd)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_C_FLAGS "-std=c11")
set(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS}  -O0 -Wall -g -ggdb")

include_directories(include)
add_subdirectory(src)
add_subdirectory(examples)
aux_source_directory(src DIR_SRCS) 
aux_source_directory(examples DIR_SRCS)

add_executable(icd ${DIR_SRCS}  main.c)