cmake_minimum_required(VERSION 3.18)
project(MyCProject C)

# Set the C standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include header directory
include_directories(include)

# Recursively collect all .c files in src/
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.c")

# Define the executable
add_executable(my_program ${SOURCES})

