# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 4.0

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /sbin/cmake

# The command to remove a file.
RM = /sbin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/luis/github/Bulletin/Server/C-remake

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/luis/github/Bulletin/Server/C-remake/build

# Include any dependencies generated for this target.
include CMakeFiles/my_program.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/my_program.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/my_program.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/my_program.dir/flags.make

CMakeFiles/my_program.dir/codegen:
.PHONY : CMakeFiles/my_program.dir/codegen

CMakeFiles/my_program.dir/src/api/director.c.o: CMakeFiles/my_program.dir/flags.make
CMakeFiles/my_program.dir/src/api/director.c.o: /home/luis/github/Bulletin/Server/C-remake/src/api/director.c
CMakeFiles/my_program.dir/src/api/director.c.o: CMakeFiles/my_program.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/luis/github/Bulletin/Server/C-remake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/my_program.dir/src/api/director.c.o"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/my_program.dir/src/api/director.c.o -MF CMakeFiles/my_program.dir/src/api/director.c.o.d -o CMakeFiles/my_program.dir/src/api/director.c.o -c /home/luis/github/Bulletin/Server/C-remake/src/api/director.c

CMakeFiles/my_program.dir/src/api/director.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/my_program.dir/src/api/director.c.i"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/luis/github/Bulletin/Server/C-remake/src/api/director.c > CMakeFiles/my_program.dir/src/api/director.c.i

CMakeFiles/my_program.dir/src/api/director.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/my_program.dir/src/api/director.c.s"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/luis/github/Bulletin/Server/C-remake/src/api/director.c -o CMakeFiles/my_program.dir/src/api/director.c.s

CMakeFiles/my_program.dir/src/api/event_management.c.o: CMakeFiles/my_program.dir/flags.make
CMakeFiles/my_program.dir/src/api/event_management.c.o: /home/luis/github/Bulletin/Server/C-remake/src/api/event_management.c
CMakeFiles/my_program.dir/src/api/event_management.c.o: CMakeFiles/my_program.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/luis/github/Bulletin/Server/C-remake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/my_program.dir/src/api/event_management.c.o"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/my_program.dir/src/api/event_management.c.o -MF CMakeFiles/my_program.dir/src/api/event_management.c.o.d -o CMakeFiles/my_program.dir/src/api/event_management.c.o -c /home/luis/github/Bulletin/Server/C-remake/src/api/event_management.c

CMakeFiles/my_program.dir/src/api/event_management.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/my_program.dir/src/api/event_management.c.i"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/luis/github/Bulletin/Server/C-remake/src/api/event_management.c > CMakeFiles/my_program.dir/src/api/event_management.c.i

CMakeFiles/my_program.dir/src/api/event_management.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/my_program.dir/src/api/event_management.c.s"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/luis/github/Bulletin/Server/C-remake/src/api/event_management.c -o CMakeFiles/my_program.dir/src/api/event_management.c.s

CMakeFiles/my_program.dir/src/api/user_management.c.o: CMakeFiles/my_program.dir/flags.make
CMakeFiles/my_program.dir/src/api/user_management.c.o: /home/luis/github/Bulletin/Server/C-remake/src/api/user_management.c
CMakeFiles/my_program.dir/src/api/user_management.c.o: CMakeFiles/my_program.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/luis/github/Bulletin/Server/C-remake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/my_program.dir/src/api/user_management.c.o"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/my_program.dir/src/api/user_management.c.o -MF CMakeFiles/my_program.dir/src/api/user_management.c.o.d -o CMakeFiles/my_program.dir/src/api/user_management.c.o -c /home/luis/github/Bulletin/Server/C-remake/src/api/user_management.c

CMakeFiles/my_program.dir/src/api/user_management.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/my_program.dir/src/api/user_management.c.i"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/luis/github/Bulletin/Server/C-remake/src/api/user_management.c > CMakeFiles/my_program.dir/src/api/user_management.c.i

CMakeFiles/my_program.dir/src/api/user_management.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/my_program.dir/src/api/user_management.c.s"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/luis/github/Bulletin/Server/C-remake/src/api/user_management.c -o CMakeFiles/my_program.dir/src/api/user_management.c.s

CMakeFiles/my_program.dir/src/backend/main.c.o: CMakeFiles/my_program.dir/flags.make
CMakeFiles/my_program.dir/src/backend/main.c.o: /home/luis/github/Bulletin/Server/C-remake/src/backend/main.c
CMakeFiles/my_program.dir/src/backend/main.c.o: CMakeFiles/my_program.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/luis/github/Bulletin/Server/C-remake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/my_program.dir/src/backend/main.c.o"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/my_program.dir/src/backend/main.c.o -MF CMakeFiles/my_program.dir/src/backend/main.c.o.d -o CMakeFiles/my_program.dir/src/backend/main.c.o -c /home/luis/github/Bulletin/Server/C-remake/src/backend/main.c

CMakeFiles/my_program.dir/src/backend/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/my_program.dir/src/backend/main.c.i"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/luis/github/Bulletin/Server/C-remake/src/backend/main.c > CMakeFiles/my_program.dir/src/backend/main.c.i

CMakeFiles/my_program.dir/src/backend/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/my_program.dir/src/backend/main.c.s"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/luis/github/Bulletin/Server/C-remake/src/backend/main.c -o CMakeFiles/my_program.dir/src/backend/main.c.s

CMakeFiles/my_program.dir/src/database/database.c.o: CMakeFiles/my_program.dir/flags.make
CMakeFiles/my_program.dir/src/database/database.c.o: /home/luis/github/Bulletin/Server/C-remake/src/database/database.c
CMakeFiles/my_program.dir/src/database/database.c.o: CMakeFiles/my_program.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/luis/github/Bulletin/Server/C-remake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/my_program.dir/src/database/database.c.o"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/my_program.dir/src/database/database.c.o -MF CMakeFiles/my_program.dir/src/database/database.c.o.d -o CMakeFiles/my_program.dir/src/database/database.c.o -c /home/luis/github/Bulletin/Server/C-remake/src/database/database.c

CMakeFiles/my_program.dir/src/database/database.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/my_program.dir/src/database/database.c.i"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/luis/github/Bulletin/Server/C-remake/src/database/database.c > CMakeFiles/my_program.dir/src/database/database.c.i

CMakeFiles/my_program.dir/src/database/database.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/my_program.dir/src/database/database.c.s"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/luis/github/Bulletin/Server/C-remake/src/database/database.c -o CMakeFiles/my_program.dir/src/database/database.c.s

CMakeFiles/my_program.dir/src/database/sqlite3.c.o: CMakeFiles/my_program.dir/flags.make
CMakeFiles/my_program.dir/src/database/sqlite3.c.o: /home/luis/github/Bulletin/Server/C-remake/src/database/sqlite3.c
CMakeFiles/my_program.dir/src/database/sqlite3.c.o: CMakeFiles/my_program.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/luis/github/Bulletin/Server/C-remake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/my_program.dir/src/database/sqlite3.c.o"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/my_program.dir/src/database/sqlite3.c.o -MF CMakeFiles/my_program.dir/src/database/sqlite3.c.o.d -o CMakeFiles/my_program.dir/src/database/sqlite3.c.o -c /home/luis/github/Bulletin/Server/C-remake/src/database/sqlite3.c

CMakeFiles/my_program.dir/src/database/sqlite3.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/my_program.dir/src/database/sqlite3.c.i"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/luis/github/Bulletin/Server/C-remake/src/database/sqlite3.c > CMakeFiles/my_program.dir/src/database/sqlite3.c.i

CMakeFiles/my_program.dir/src/database/sqlite3.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/my_program.dir/src/database/sqlite3.c.s"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/luis/github/Bulletin/Server/C-remake/src/database/sqlite3.c -o CMakeFiles/my_program.dir/src/database/sqlite3.c.s

CMakeFiles/my_program.dir/src/lib/string_utils.c.o: CMakeFiles/my_program.dir/flags.make
CMakeFiles/my_program.dir/src/lib/string_utils.c.o: /home/luis/github/Bulletin/Server/C-remake/src/lib/string_utils.c
CMakeFiles/my_program.dir/src/lib/string_utils.c.o: CMakeFiles/my_program.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/luis/github/Bulletin/Server/C-remake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/my_program.dir/src/lib/string_utils.c.o"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/my_program.dir/src/lib/string_utils.c.o -MF CMakeFiles/my_program.dir/src/lib/string_utils.c.o.d -o CMakeFiles/my_program.dir/src/lib/string_utils.c.o -c /home/luis/github/Bulletin/Server/C-remake/src/lib/string_utils.c

CMakeFiles/my_program.dir/src/lib/string_utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/my_program.dir/src/lib/string_utils.c.i"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/luis/github/Bulletin/Server/C-remake/src/lib/string_utils.c > CMakeFiles/my_program.dir/src/lib/string_utils.c.i

CMakeFiles/my_program.dir/src/lib/string_utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/my_program.dir/src/lib/string_utils.c.s"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/luis/github/Bulletin/Server/C-remake/src/lib/string_utils.c -o CMakeFiles/my_program.dir/src/lib/string_utils.c.s

CMakeFiles/my_program.dir/src/lib/table.c.o: CMakeFiles/my_program.dir/flags.make
CMakeFiles/my_program.dir/src/lib/table.c.o: /home/luis/github/Bulletin/Server/C-remake/src/lib/table.c
CMakeFiles/my_program.dir/src/lib/table.c.o: CMakeFiles/my_program.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/luis/github/Bulletin/Server/C-remake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/my_program.dir/src/lib/table.c.o"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/my_program.dir/src/lib/table.c.o -MF CMakeFiles/my_program.dir/src/lib/table.c.o.d -o CMakeFiles/my_program.dir/src/lib/table.c.o -c /home/luis/github/Bulletin/Server/C-remake/src/lib/table.c

CMakeFiles/my_program.dir/src/lib/table.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/my_program.dir/src/lib/table.c.i"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/luis/github/Bulletin/Server/C-remake/src/lib/table.c > CMakeFiles/my_program.dir/src/lib/table.c.i

CMakeFiles/my_program.dir/src/lib/table.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/my_program.dir/src/lib/table.c.s"
	/sbin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/luis/github/Bulletin/Server/C-remake/src/lib/table.c -o CMakeFiles/my_program.dir/src/lib/table.c.s

# Object files for target my_program
my_program_OBJECTS = \
"CMakeFiles/my_program.dir/src/api/director.c.o" \
"CMakeFiles/my_program.dir/src/api/event_management.c.o" \
"CMakeFiles/my_program.dir/src/api/user_management.c.o" \
"CMakeFiles/my_program.dir/src/backend/main.c.o" \
"CMakeFiles/my_program.dir/src/database/database.c.o" \
"CMakeFiles/my_program.dir/src/database/sqlite3.c.o" \
"CMakeFiles/my_program.dir/src/lib/string_utils.c.o" \
"CMakeFiles/my_program.dir/src/lib/table.c.o"

# External object files for target my_program
my_program_EXTERNAL_OBJECTS =

my_program: CMakeFiles/my_program.dir/src/api/director.c.o
my_program: CMakeFiles/my_program.dir/src/api/event_management.c.o
my_program: CMakeFiles/my_program.dir/src/api/user_management.c.o
my_program: CMakeFiles/my_program.dir/src/backend/main.c.o
my_program: CMakeFiles/my_program.dir/src/database/database.c.o
my_program: CMakeFiles/my_program.dir/src/database/sqlite3.c.o
my_program: CMakeFiles/my_program.dir/src/lib/string_utils.c.o
my_program: CMakeFiles/my_program.dir/src/lib/table.c.o
my_program: CMakeFiles/my_program.dir/build.make
my_program: CMakeFiles/my_program.dir/compiler_depend.ts
my_program: CMakeFiles/my_program.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/luis/github/Bulletin/Server/C-remake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking C executable my_program"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/my_program.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/my_program.dir/build: my_program
.PHONY : CMakeFiles/my_program.dir/build

CMakeFiles/my_program.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/my_program.dir/cmake_clean.cmake
.PHONY : CMakeFiles/my_program.dir/clean

CMakeFiles/my_program.dir/depend:
	cd /home/luis/github/Bulletin/Server/C-remake/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/luis/github/Bulletin/Server/C-remake /home/luis/github/Bulletin/Server/C-remake /home/luis/github/Bulletin/Server/C-remake/build /home/luis/github/Bulletin/Server/C-remake/build /home/luis/github/Bulletin/Server/C-remake/build/CMakeFiles/my_program.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/my_program.dir/depend

