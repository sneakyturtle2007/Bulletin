# CMAKE generated file: DO NOT EDIT!
# Generated by CMake Version 3.25
cmake_policy(SET CMP0009 NEW)

# SOURCES at CMakeLists.txt:12 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/luis/github/Bulletin/Server/C-remake/src/*.c")
set(OLD_GLOB
  "/home/luis/github/Bulletin/Server/C-remake/src/database/database.c"
  "/home/luis/github/Bulletin/Server/C-remake/src/director.c"
  "/home/luis/github/Bulletin/Server/C-remake/src/main.c"
  "/home/luis/github/Bulletin/Server/C-remake/src/sqlite3.c"
  "/home/luis/github/Bulletin/Server/C-remake/src/user_management.c"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/luis/github/Bulletin/Server/C-remake/build/CMakeFiles/cmake.verify_globs")
endif()
