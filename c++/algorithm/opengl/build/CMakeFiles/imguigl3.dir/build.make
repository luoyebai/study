# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/luoyebai/code/study/c++/algorithm/opengl

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/luoyebai/code/study/c++/algorithm/opengl/build

# Include any dependencies generated for this target.
include CMakeFiles/imguigl3.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/imguigl3.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/imguigl3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/imguigl3.dir/flags.make

CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_glfw.cpp.o: CMakeFiles/imguigl3.dir/flags.make
CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_glfw.cpp.o: /home/luoyebai/code/study/c++/algorithm/opengl/imgui/backends/imgui_impl_glfw.cpp
CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_glfw.cpp.o: CMakeFiles/imguigl3.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luoyebai/code/study/c++/algorithm/opengl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_glfw.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_glfw.cpp.o -MF CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_glfw.cpp.o.d -o CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_glfw.cpp.o -c /home/luoyebai/code/study/c++/algorithm/opengl/imgui/backends/imgui_impl_glfw.cpp

CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_glfw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_glfw.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luoyebai/code/study/c++/algorithm/opengl/imgui/backends/imgui_impl_glfw.cpp > CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_glfw.cpp.i

CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_glfw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_glfw.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luoyebai/code/study/c++/algorithm/opengl/imgui/backends/imgui_impl_glfw.cpp -o CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_glfw.cpp.s

CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_opengl3.cpp.o: CMakeFiles/imguigl3.dir/flags.make
CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_opengl3.cpp.o: /home/luoyebai/code/study/c++/algorithm/opengl/imgui/backends/imgui_impl_opengl3.cpp
CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_opengl3.cpp.o: CMakeFiles/imguigl3.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luoyebai/code/study/c++/algorithm/opengl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_opengl3.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_opengl3.cpp.o -MF CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_opengl3.cpp.o.d -o CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_opengl3.cpp.o -c /home/luoyebai/code/study/c++/algorithm/opengl/imgui/backends/imgui_impl_opengl3.cpp

CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_opengl3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_opengl3.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luoyebai/code/study/c++/algorithm/opengl/imgui/backends/imgui_impl_opengl3.cpp > CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_opengl3.cpp.i

CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_opengl3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_opengl3.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luoyebai/code/study/c++/algorithm/opengl/imgui/backends/imgui_impl_opengl3.cpp -o CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_opengl3.cpp.s

# Object files for target imguigl3
imguigl3_OBJECTS = \
"CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_glfw.cpp.o" \
"CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_opengl3.cpp.o"

# External object files for target imguigl3
imguigl3_EXTERNAL_OBJECTS =

libimguigl3.a: CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_glfw.cpp.o
libimguigl3.a: CMakeFiles/imguigl3.dir/imgui/backends/imgui_impl_opengl3.cpp.o
libimguigl3.a: CMakeFiles/imguigl3.dir/build.make
libimguigl3.a: CMakeFiles/imguigl3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/luoyebai/code/study/c++/algorithm/opengl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libimguigl3.a"
	$(CMAKE_COMMAND) -P CMakeFiles/imguigl3.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/imguigl3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/imguigl3.dir/build: libimguigl3.a
.PHONY : CMakeFiles/imguigl3.dir/build

CMakeFiles/imguigl3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/imguigl3.dir/cmake_clean.cmake
.PHONY : CMakeFiles/imguigl3.dir/clean

CMakeFiles/imguigl3.dir/depend:
	cd /home/luoyebai/code/study/c++/algorithm/opengl/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/luoyebai/code/study/c++/algorithm/opengl /home/luoyebai/code/study/c++/algorithm/opengl /home/luoyebai/code/study/c++/algorithm/opengl/build /home/luoyebai/code/study/c++/algorithm/opengl/build /home/luoyebai/code/study/c++/algorithm/opengl/build/CMakeFiles/imguigl3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/imguigl3.dir/depend
