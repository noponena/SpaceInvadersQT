# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.30

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Qt\Tools\CMake_64\bin\cmake.exe

# The command to remove a file.
RM = C:\Qt\Tools\CMake_64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2\build

# Include any dependencies generated for this target.
include CMakeFiles/string_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/string_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/string_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/string_test.dir/flags.make

CMakeFiles/string_test.dir/tests/string_test.c.obj: CMakeFiles/string_test.dir/flags.make
CMakeFiles/string_test.dir/tests/string_test.c.obj: CMakeFiles/string_test.dir/includes_C.rsp
CMakeFiles/string_test.dir/tests/string_test.c.obj: C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/tests/string_test.c
CMakeFiles/string_test.dir/tests/string_test.c.obj: CMakeFiles/string_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/string_test.dir/tests/string_test.c.obj"
	C:\Qt\Tools\mingw1310_64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/string_test.dir/tests/string_test.c.obj -MF CMakeFiles\string_test.dir\tests\string_test.c.obj.d -o CMakeFiles\string_test.dir\tests\string_test.c.obj -c C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2\tests\string_test.c

CMakeFiles/string_test.dir/tests/string_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/string_test.dir/tests/string_test.c.i"
	C:\Qt\Tools\mingw1310_64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2\tests\string_test.c > CMakeFiles\string_test.dir\tests\string_test.c.i

CMakeFiles/string_test.dir/tests/string_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/string_test.dir/tests/string_test.c.s"
	C:\Qt\Tools\mingw1310_64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2\tests\string_test.c -o CMakeFiles\string_test.dir\tests\string_test.c.s

# Object files for target string_test
string_test_OBJECTS = \
"CMakeFiles/string_test.dir/tests/string_test.c.obj"

# External object files for target string_test
string_test_EXTERNAL_OBJECTS =

string_test.exe: CMakeFiles/string_test.dir/tests/string_test.c.obj
string_test.exe: CMakeFiles/string_test.dir/build.make
string_test.exe: libsndfile.a
string_test.exe: libtest_utils.a
string_test.exe: libsndfile.a
string_test.exe: CMakeFiles/string_test.dir/linkLibs.rsp
string_test.exe: CMakeFiles/string_test.dir/objects1.rsp
string_test.exe: CMakeFiles/string_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable string_test.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\string_test.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/string_test.dir/build: string_test.exe
.PHONY : CMakeFiles/string_test.dir/build

CMakeFiles/string_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\string_test.dir\cmake_clean.cmake
.PHONY : CMakeFiles/string_test.dir/clean

CMakeFiles/string_test.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2 C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2 C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2\build C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2\build C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2\build\CMakeFiles\string_test.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/string_test.dir/depend

