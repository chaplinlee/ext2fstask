# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "/Users/ryan/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/183.4886.39/CLion.app/Contents/bin/cmake/mac/bin/cmake"

# The command to remove a file.
RM = "/Users/ryan/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/183.4886.39/CLion.app/Contents/bin/cmake/mac/bin/cmake" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ryan/Desktop/ext2fstask/fsviewer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ryan/Desktop/ext2fstask/fsviewer/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/del_undo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/del_undo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/del_undo.dir/flags.make

CMakeFiles/del_undo.dir/del_undo.c.o: CMakeFiles/del_undo.dir/flags.make
CMakeFiles/del_undo.dir/del_undo.c.o: ../del_undo.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ryan/Desktop/ext2fstask/fsviewer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/del_undo.dir/del_undo.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/del_undo.dir/del_undo.c.o   -c /Users/ryan/Desktop/ext2fstask/fsviewer/del_undo.c

CMakeFiles/del_undo.dir/del_undo.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/del_undo.dir/del_undo.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/ryan/Desktop/ext2fstask/fsviewer/del_undo.c > CMakeFiles/del_undo.dir/del_undo.c.i

CMakeFiles/del_undo.dir/del_undo.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/del_undo.dir/del_undo.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/ryan/Desktop/ext2fstask/fsviewer/del_undo.c -o CMakeFiles/del_undo.dir/del_undo.c.s

# Object files for target del_undo
del_undo_OBJECTS = \
"CMakeFiles/del_undo.dir/del_undo.c.o"

# External object files for target del_undo
del_undo_EXTERNAL_OBJECTS =

del_undo: CMakeFiles/del_undo.dir/del_undo.c.o
del_undo: CMakeFiles/del_undo.dir/build.make
del_undo: CMakeFiles/del_undo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/ryan/Desktop/ext2fstask/fsviewer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable del_undo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/del_undo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/del_undo.dir/build: del_undo

.PHONY : CMakeFiles/del_undo.dir/build

CMakeFiles/del_undo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/del_undo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/del_undo.dir/clean

CMakeFiles/del_undo.dir/depend:
	cd /Users/ryan/Desktop/ext2fstask/fsviewer/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ryan/Desktop/ext2fstask/fsviewer /Users/ryan/Desktop/ext2fstask/fsviewer /Users/ryan/Desktop/ext2fstask/fsviewer/cmake-build-debug /Users/ryan/Desktop/ext2fstask/fsviewer/cmake-build-debug /Users/ryan/Desktop/ext2fstask/fsviewer/cmake-build-debug/CMakeFiles/del_undo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/del_undo.dir/depend

