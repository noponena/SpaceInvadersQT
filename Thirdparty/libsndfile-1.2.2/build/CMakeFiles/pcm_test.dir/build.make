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
include CMakeFiles/pcm_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/pcm_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/pcm_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pcm_test.dir/flags.make

tests/pcm_test.c: C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/tests/pcm_test.def
tests/pcm_test.c: C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/cmake/CMakeAutoGenScript.cmake
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "CMakeAutoGen: generating pcm_test.c"
	C:\Qt\Tools\CMake_64\bin\cmake.exe -DDEFINITION=C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/tests/pcm_test.def -DOUTPUTDIR=C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/build/tests -P C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/cmake/CMakeAutoGenScript.cmake

CMakeFiles/pcm_test.dir/tests/pcm_test.c.obj: CMakeFiles/pcm_test.dir/flags.make
CMakeFiles/pcm_test.dir/tests/pcm_test.c.obj: CMakeFiles/pcm_test.dir/includes_C.rsp
CMakeFiles/pcm_test.dir/tests/pcm_test.c.obj: tests/pcm_test.c
CMakeFiles/pcm_test.dir/tests/pcm_test.c.obj: CMakeFiles/pcm_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/pcm_test.dir/tests/pcm_test.c.obj"
	C:\Qt\Tools\mingw1310_64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/pcm_test.dir/tests/pcm_test.c.obj -MF CMakeFiles\pcm_test.dir\tests\pcm_test.c.obj.d -o CMakeFiles\pcm_test.dir\tests\pcm_test.c.obj -c C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2\build\tests\pcm_test.c

CMakeFiles/pcm_test.dir/tests/pcm_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/pcm_test.dir/tests/pcm_test.c.i"
	C:\Qt\Tools\mingw1310_64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2\build\tests\pcm_test.c > CMakeFiles\pcm_test.dir\tests\pcm_test.c.i

CMakeFiles/pcm_test.dir/tests/pcm_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/pcm_test.dir/tests/pcm_test.c.s"
	C:\Qt\Tools\mingw1310_64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2\build\tests\pcm_test.c -o CMakeFiles\pcm_test.dir\tests\pcm_test.c.s

# Object files for target pcm_test
pcm_test_OBJECTS = \
"CMakeFiles/pcm_test.dir/tests/pcm_test.c.obj"

# External object files for target pcm_test
pcm_test_EXTERNAL_OBJECTS =

pcm_test.exe: CMakeFiles/pcm_test.dir/tests/pcm_test.c.obj
pcm_test.exe: CMakeFiles/pcm_test.dir/build.make
pcm_test.exe: libsndfile.a
pcm_test.exe: libtest_utils.a
pcm_test.exe: libsndfile.a
pcm_test.exe: CMakeFiles/pcm_test.dir/linkLibs.rsp
pcm_test.exe: CMakeFiles/pcm_test.dir/objects1.rsp
pcm_test.exe: CMakeFiles/pcm_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable pcm_test.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\pcm_test.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pcm_test.dir/build: pcm_test.exe
.PHONY : CMakeFiles/pcm_test.dir/build

CMakeFiles/pcm_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\pcm_test.dir\cmake_clean.cmake
.PHONY : CMakeFiles/pcm_test.dir/clean

CMakeFiles/pcm_test.dir/depend: tests/pcm_test.c
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2 C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2 C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2\build C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2\build C:\Users\aaron\Downloads\libsndfile-1.2.2\libsndfile-1.2.2\build\CMakeFiles\pcm_test.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/pcm_test.dir/depend

