# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /media/sf_E_DRIVE/workshop/ShmObjMgr

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /media/sf_E_DRIVE/workshop/ShmObjMgr/build

# Include any dependencies generated for this target.
include src/shm_pointer/CMakeFiles/shm_pointer.dir/depend.make

# Include the progress variables for this target.
include src/shm_pointer/CMakeFiles/shm_pointer.dir/progress.make

# Include the compile flags for this target's objects.
include src/shm_pointer/CMakeFiles/shm_pointer.dir/flags.make

src/shm_pointer/CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.o: src/shm_pointer/CMakeFiles/shm_pointer.dir/flags.make
src/shm_pointer/CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.o: ../src/shm_pointer/shm_pointer_count.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /media/sf_E_DRIVE/workshop/ShmObjMgr/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/shm_pointer/CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.o"
	cd /media/sf_E_DRIVE/workshop/ShmObjMgr/build/src/shm_pointer && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.o -c /media/sf_E_DRIVE/workshop/ShmObjMgr/src/shm_pointer/shm_pointer_count.cpp

src/shm_pointer/CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.i"
	cd /media/sf_E_DRIVE/workshop/ShmObjMgr/build/src/shm_pointer && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /media/sf_E_DRIVE/workshop/ShmObjMgr/src/shm_pointer/shm_pointer_count.cpp > CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.i

src/shm_pointer/CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.s"
	cd /media/sf_E_DRIVE/workshop/ShmObjMgr/build/src/shm_pointer && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /media/sf_E_DRIVE/workshop/ShmObjMgr/src/shm_pointer/shm_pointer_count.cpp -o CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.s

src/shm_pointer/CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.o.requires:
.PHONY : src/shm_pointer/CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.o.requires

src/shm_pointer/CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.o.provides: src/shm_pointer/CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.o.requires
	$(MAKE) -f src/shm_pointer/CMakeFiles/shm_pointer.dir/build.make src/shm_pointer/CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.o.provides.build
.PHONY : src/shm_pointer/CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.o.provides

src/shm_pointer/CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.o.provides.build: src/shm_pointer/CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.o

# Object files for target shm_pointer
shm_pointer_OBJECTS = \
"CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.o"

# External object files for target shm_pointer
shm_pointer_EXTERNAL_OBJECTS =

src/shm_pointer/lib/libshm_pointer.a: src/shm_pointer/CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.o
src/shm_pointer/lib/libshm_pointer.a: src/shm_pointer/CMakeFiles/shm_pointer.dir/build.make
src/shm_pointer/lib/libshm_pointer.a: src/shm_pointer/CMakeFiles/shm_pointer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library lib/libshm_pointer.a"
	cd /media/sf_E_DRIVE/workshop/ShmObjMgr/build/src/shm_pointer && $(CMAKE_COMMAND) -P CMakeFiles/shm_pointer.dir/cmake_clean_target.cmake
	cd /media/sf_E_DRIVE/workshop/ShmObjMgr/build/src/shm_pointer && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/shm_pointer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/shm_pointer/CMakeFiles/shm_pointer.dir/build: src/shm_pointer/lib/libshm_pointer.a
.PHONY : src/shm_pointer/CMakeFiles/shm_pointer.dir/build

src/shm_pointer/CMakeFiles/shm_pointer.dir/requires: src/shm_pointer/CMakeFiles/shm_pointer.dir/shm_pointer_count.cpp.o.requires
.PHONY : src/shm_pointer/CMakeFiles/shm_pointer.dir/requires

src/shm_pointer/CMakeFiles/shm_pointer.dir/clean:
	cd /media/sf_E_DRIVE/workshop/ShmObjMgr/build/src/shm_pointer && $(CMAKE_COMMAND) -P CMakeFiles/shm_pointer.dir/cmake_clean.cmake
.PHONY : src/shm_pointer/CMakeFiles/shm_pointer.dir/clean

src/shm_pointer/CMakeFiles/shm_pointer.dir/depend:
	cd /media/sf_E_DRIVE/workshop/ShmObjMgr/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /media/sf_E_DRIVE/workshop/ShmObjMgr /media/sf_E_DRIVE/workshop/ShmObjMgr/src/shm_pointer /media/sf_E_DRIVE/workshop/ShmObjMgr/build /media/sf_E_DRIVE/workshop/ShmObjMgr/build/src/shm_pointer /media/sf_E_DRIVE/workshop/ShmObjMgr/build/src/shm_pointer/CMakeFiles/shm_pointer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/shm_pointer/CMakeFiles/shm_pointer.dir/depend

