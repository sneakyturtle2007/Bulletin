cmake_minimum_required(VERSION 3.18)
project(MyCProject C)

# Set the C standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

#add_subdirectory(external/json-c)
add_subdirectory(external/json-c)
# Include header directory
include_directories(include include/api include/database include/lib)
include_directories(external external/json-c/include external/sqlite3)
# Recursively collect all .c files in src/
file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "src/*.c")
file(GLOB_RECURSE EXTERNAL CONFIGURE_DEPENDS "external/*.c")
# Define the executable
add_executable(bulletin ${SRC} ${EXTERNAL})
target_link_libraries(bulletin PRIVATE json-c)
target_link_directories(bulletin PRIVATE external/json-c)
# Define compile modes
  # Release build
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -DNDEBUG)
  endif()
  # Debug build
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0 -Wall -Wextra)
  endif()


