# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/wang/Desktop/FLTK_HW2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wang/Desktop/FLTK_HW2/build

# Include any dependencies generated for this target.
include src/CMakeFiles/FLTK_HW2.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/FLTK_HW2.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/FLTK_HW2.dir/flags.make

src/CMakeFiles/FLTK_HW2.dir/main.cpp.o: src/CMakeFiles/FLTK_HW2.dir/flags.make
src/CMakeFiles/FLTK_HW2.dir/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wang/Desktop/FLTK_HW2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/FLTK_HW2.dir/main.cpp.o"
	cd /home/wang/Desktop/FLTK_HW2/build/src && g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FLTK_HW2.dir/main.cpp.o -c /home/wang/Desktop/FLTK_HW2/src/main.cpp

src/CMakeFiles/FLTK_HW2.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FLTK_HW2.dir/main.cpp.i"
	cd /home/wang/Desktop/FLTK_HW2/build/src && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wang/Desktop/FLTK_HW2/src/main.cpp > CMakeFiles/FLTK_HW2.dir/main.cpp.i

src/CMakeFiles/FLTK_HW2.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FLTK_HW2.dir/main.cpp.s"
	cd /home/wang/Desktop/FLTK_HW2/build/src && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wang/Desktop/FLTK_HW2/src/main.cpp -o CMakeFiles/FLTK_HW2.dir/main.cpp.s

src/CMakeFiles/FLTK_HW2.dir/main.cpp.o.requires:

.PHONY : src/CMakeFiles/FLTK_HW2.dir/main.cpp.o.requires

src/CMakeFiles/FLTK_HW2.dir/main.cpp.o.provides: src/CMakeFiles/FLTK_HW2.dir/main.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/FLTK_HW2.dir/build.make src/CMakeFiles/FLTK_HW2.dir/main.cpp.o.provides.build
.PHONY : src/CMakeFiles/FLTK_HW2.dir/main.cpp.o.provides

src/CMakeFiles/FLTK_HW2.dir/main.cpp.o.provides.build: src/CMakeFiles/FLTK_HW2.dir/main.cpp.o


# Object files for target FLTK_HW2
FLTK_HW2_OBJECTS = \
"CMakeFiles/FLTK_HW2.dir/main.cpp.o"

# External object files for target FLTK_HW2
FLTK_HW2_EXTERNAL_OBJECTS =

../bin/FLTK_HW2: src/CMakeFiles/FLTK_HW2.dir/main.cpp.o
../bin/FLTK_HW2: src/CMakeFiles/FLTK_HW2.dir/build.make
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libGLU.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libGL.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libGLU.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libGL.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libglut.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libXmu.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libXi.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libfltk_images.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libfltk_forms.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libfltk_gl.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libGL.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libfltk.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libSM.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libICE.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libX11.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libXext.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libm.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libGLU.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libglut.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libXmu.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libXi.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libSM.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libICE.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libX11.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libXext.so
../bin/FLTK_HW2: /usr/lib/x86_64-linux-gnu/libm.so
../bin/FLTK_HW2: src/CMakeFiles/FLTK_HW2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wang/Desktop/FLTK_HW2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/FLTK_HW2"
	cd /home/wang/Desktop/FLTK_HW2/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FLTK_HW2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/FLTK_HW2.dir/build: ../bin/FLTK_HW2

.PHONY : src/CMakeFiles/FLTK_HW2.dir/build

src/CMakeFiles/FLTK_HW2.dir/requires: src/CMakeFiles/FLTK_HW2.dir/main.cpp.o.requires

.PHONY : src/CMakeFiles/FLTK_HW2.dir/requires

src/CMakeFiles/FLTK_HW2.dir/clean:
	cd /home/wang/Desktop/FLTK_HW2/build/src && $(CMAKE_COMMAND) -P CMakeFiles/FLTK_HW2.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/FLTK_HW2.dir/clean

src/CMakeFiles/FLTK_HW2.dir/depend:
	cd /home/wang/Desktop/FLTK_HW2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wang/Desktop/FLTK_HW2 /home/wang/Desktop/FLTK_HW2/src /home/wang/Desktop/FLTK_HW2/build /home/wang/Desktop/FLTK_HW2/build/src /home/wang/Desktop/FLTK_HW2/build/src/CMakeFiles/FLTK_HW2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/FLTK_HW2.dir/depend

