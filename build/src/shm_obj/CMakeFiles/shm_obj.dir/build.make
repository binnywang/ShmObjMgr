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
include src/shm_obj/CMakeFiles/shm_obj.dir/depend.make

# Include the progress variables for this target.
include src/shm_obj/CMakeFiles/shm_obj.dir/progress.make

# Include the compile flags for this target's objects.
include src/shm_obj/CMakeFiles/shm_obj.dir/flags.make

src/shm_obj/CMakeFiles/shm_obj.dir/shm_pool.cpp.o: src/shm_obj/CMakeFiles/shm_obj.dir/flags.make
src/shm_obj/CMakeFiles/shm_obj.dir/shm_pool.cpp.o: ../src/shm_obj/shm_pool.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /media/sf_E_DRIVE/workshop/ShmObjMgr/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/shm_obj/CMakeFiles/shm_obj.dir/shm_pool.cpp.o"
	cd /media/sf_E_DRIVE/workshop/ShmObjMgr/build/src/shm_obj && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/shm_obj.dir/shm_pool.cpp.o -c /media/sf_E_DRIVE/workshop/ShmObjMgr/src/shm_obj/shm_pool.cpp

src/shm_obj/CMakeFiles/shm_obj.dir/shm_pool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/shm_obj.dir/shm_pool.cpp.i"
	cd /media/sf_E_DRIVE/workshop/ShmObjMgr/build/src/shm_obj && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /media/sf_E_DRIVE/workshop/ShmObjMgr/src/shm_obj/shm_pool.cpp > CMakeFiles/shm_obj.dir/shm_pool.cpp.i

src/shm_obj/CMakeFiles/shm_obj.dir/shm_pool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/shm_obj.dir/shm_pool.cpp.s"
	cd /media/sf_E_DRIVE/workshop/ShmObjMgr/build/src/shm_obj && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /media/sf_E_DRIVE/workshop/ShmObjMgr/src/shm_obj/shm_pool.cpp -o CMakeFiles/shm_obj.dir/shm_pool.cpp.s

src/shm_obj/CMakeFiles/shm_obj.dir/shm_pool.cpp.o.requires:
.PHONY : src/shm_obj/CMakeFiles/shm_obj.dir/shm_pool.cpp.o.requires

src/shm_obj/CMakeFiles/shm_obj.dir/shm_pool.cpp.o.provides: src/shm_obj/CMakeFiles/shm_obj.dir/shm_pool.cpp.o.requires
	$(MAKE) -f src/shm_obj/CMakeFiles/shm_obj.dir/build.make src/shm_obj/CMakeFiles/shm_obj.dir/shm_pool.cpp.o.provides.build
.PHONY : src/shm_obj/CMakeFiles/shm_obj.dir/shm_pool.cpp.o.provides

src/shm_obj/CMakeFiles/shm_obj.dir/shm_pool.cpp.o.provides.build: src/shm_obj/CMakeFiles/shm_obj.dir/shm_pool.cpp.o

src/shm_obj/CMakeFiles/shm_obj.dir/system.cpp.o: src/shm_obj/CMakeFiles/shm_obj.dir/flags.make
src/shm_obj/CMakeFiles/shm_obj.dir/system.cpp.o: ../src/shm_obj/system.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /media/sf_E_DRIVE/workshop/ShmObjMgr/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/shm_obj/CMakeFiles/shm_obj.dir/system.cpp.o"
	cd /media/sf_E_DRIVE/workshop/ShmObjMgr/build/src/shm_obj && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/shm_obj.dir/system.cpp.o -c /media/sf_E_DRIVE/workshop/ShmObjMgr/src/shm_obj/system.cpp

src/shm_obj/CMakeFiles/shm_obj.dir/system.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/shm_obj.dir/system.cpp.i"
	cd /media/sf_E_DRIVE/workshop/ShmObjMgr/build/src/shm_obj && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /media/sf_E_DRIVE/workshop/ShmObjMgr/src/shm_obj/system.cpp > CMakeFiles/shm_obj.dir/system.cpp.i

src/shm_obj/CMakeFiles/shm_obj.dir/system.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/shm_obj.dir/system.cpp.s"
	cd /media/sf_E_DRIVE/workshop/ShmObjMgr/build/src/shm_obj && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /media/sf_E_DRIVE/workshop/ShmObjMgr/src/shm_obj/system.cpp -o CMakeFiles/shm_obj.dir/system.cpp.s

src/shm_obj/CMakeFiles/shm_obj.dir/system.cpp.o.requires:
.PHONY : src/shm_obj/CMakeFiles/shm_obj.dir/system.cpp.o.requires

src/shm_obj/CMakeFiles/shm_obj.dir/system.cpp.o.provides: src/shm_obj/CMakeFiles/shm_obj.dir/system.cpp.o.requires
	$(MAKE) -f src/shm_obj/CMakeFiles/shm_obj.dir/build.make src/shm_obj/CMakeFiles/shm_obj.dir/system.cpp.o.provides.build
.PHONY : src/shm_obj/CMakeFiles/shm_obj.dir/system.cpp.o.provides

src/shm_obj/CMakeFiles/shm_obj.dir/system.cpp.o.provides.build: src/shm_obj/CMakeFiles/shm_obj.dir/system.cpp.o

# Object files for target shm_obj
shm_obj_OBJECTS = \
"CMakeFiles/shm_obj.dir/shm_pool.cpp.o" \
"CMakeFiles/shm_obj.dir/system.cpp.o"

# External object files for target shm_obj
shm_obj_EXTERNAL_OBJECTS =

src/shm_obj/lib/libshm_obj.a: src/shm_obj/CMakeFiles/shm_obj.dir/shm_pool.cpp.o
src/shm_obj/lib/libshm_obj.a: src/shm_obj/CMakeFiles/shm_obj.dir/system.cpp.o
src/shm_obj/lib/libshm_obj.a: src/shm_obj/CMakeFiles/shm_obj.dir/build.make
src/shm_obj/lib/libshm_obj.a: src/shm_obj/CMakeFiles/shm_obj.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library lib/libshm_obj.a"
	cd /media/sf_E_DRIVE/workshop/ShmObjMgr/build/src/shm_obj && $(CMAKE_COMMAND) -P CMakeFiles/shm_obj.dir/cmake_clean_target.cmake
	cd /media/sf_E_DRIVE/workshop/ShmObjMgr/build/src/shm_obj && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/shm_obj.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/shm_obj/CMakeFiles/shm_obj.dir/build: src/shm_obj/lib/libshm_obj.a
.PHONY : src/shm_obj/CMakeFiles/shm_obj.dir/build

src/shm_obj/CMakeFiles/shm_obj.dir/requires: src/shm_obj/CMakeFiles/shm_obj.dir/shm_pool.cpp.o.requires
src/shm_obj/CMakeFiles/shm_obj.dir/requires: src/shm_obj/CMakeFiles/shm_obj.dir/system.cpp.o.requires
.PHONY : src/shm_obj/CMakeFiles/shm_obj.dir/requires

src/shm_obj/CMakeFiles/shm_obj.dir/clean:
	cd /media/sf_E_DRIVE/workshop/ShmObjMgr/build/src/shm_obj && $(CMAKE_COMMAND) -P CMakeFiles/shm_obj.dir/cmake_clean.cmake
.PHONY : src/shm_obj/CMakeFiles/shm_obj.dir/clean

src/shm_obj/CMakeFiles/shm_obj.dir/depend:
	cd /media/sf_E_DRIVE/workshop/ShmObjMgr/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /media/sf_E_DRIVE/workshop/ShmObjMgr /media/sf_E_DRIVE/workshop/ShmObjMgr/src/shm_obj /media/sf_E_DRIVE/workshop/ShmObjMgr/build /media/sf_E_DRIVE/workshop/ShmObjMgr/build/src/shm_obj /media/sf_E_DRIVE/workshop/ShmObjMgr/build/src/shm_obj/CMakeFiles/shm_obj.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/shm_obj/CMakeFiles/shm_obj.dir/depend

