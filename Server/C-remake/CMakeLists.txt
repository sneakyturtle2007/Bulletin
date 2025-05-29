cmake_minimum_required(VERSION 3.31)
project(C_remake)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(external)
include_directories(included)
include_directories(src)
include_directories(src/HeaderFiles)
include_directories(src/ProgramFiles)

add_executable(C_remake
        src/HeaderFiles/sqlite3.h
        src/ProgramFiles/sqlite3.c
        src/ProgramFiles/main.c)
