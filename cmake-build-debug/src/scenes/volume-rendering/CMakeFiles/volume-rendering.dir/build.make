# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /home/kruseborn/clion/clion-2019.2.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/kruseborn/clion/clion-2019.2.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kruseborn/git/mongoose

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kruseborn/git/mongoose/cmake-build-debug

# Include any dependencies generated for this target.
include src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/depend.make

# Include the progress variables for this target.
include src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/progress.make

# Include the compile flags for this target's objects.
include src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/flags.make

src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_scene.cpp.o: src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/flags.make
src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_scene.cpp.o: ../src/scenes/volume-rendering/volume_scene.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kruseborn/git/mongoose/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_scene.cpp.o"
	cd /home/kruseborn/git/mongoose/cmake-build-debug/src/scenes/volume-rendering && /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/volume-rendering.dir/volume_scene.cpp.o -c /home/kruseborn/git/mongoose/src/scenes/volume-rendering/volume_scene.cpp

src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_scene.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/volume-rendering.dir/volume_scene.cpp.i"
	cd /home/kruseborn/git/mongoose/cmake-build-debug/src/scenes/volume-rendering && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kruseborn/git/mongoose/src/scenes/volume-rendering/volume_scene.cpp > CMakeFiles/volume-rendering.dir/volume_scene.cpp.i

src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_scene.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/volume-rendering.dir/volume_scene.cpp.s"
	cd /home/kruseborn/git/mongoose/cmake-build-debug/src/scenes/volume-rendering && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kruseborn/git/mongoose/src/scenes/volume-rendering/volume_scene.cpp -o CMakeFiles/volume-rendering.dir/volume_scene.cpp.s

src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_rendering.cpp.o: src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/flags.make
src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_rendering.cpp.o: ../src/scenes/volume-rendering/volume_rendering.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kruseborn/git/mongoose/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_rendering.cpp.o"
	cd /home/kruseborn/git/mongoose/cmake-build-debug/src/scenes/volume-rendering && /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/volume-rendering.dir/volume_rendering.cpp.o -c /home/kruseborn/git/mongoose/src/scenes/volume-rendering/volume_rendering.cpp

src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_rendering.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/volume-rendering.dir/volume_rendering.cpp.i"
	cd /home/kruseborn/git/mongoose/cmake-build-debug/src/scenes/volume-rendering && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kruseborn/git/mongoose/src/scenes/volume-rendering/volume_rendering.cpp > CMakeFiles/volume-rendering.dir/volume_rendering.cpp.i

src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_rendering.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/volume-rendering.dir/volume_rendering.cpp.s"
	cd /home/kruseborn/git/mongoose/cmake-build-debug/src/scenes/volume-rendering && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kruseborn/git/mongoose/src/scenes/volume-rendering/volume_rendering.cpp -o CMakeFiles/volume-rendering.dir/volume_rendering.cpp.s

src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_renderpass.cpp.o: src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/flags.make
src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_renderpass.cpp.o: ../src/scenes/volume-rendering/volume_renderpass.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kruseborn/git/mongoose/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_renderpass.cpp.o"
	cd /home/kruseborn/git/mongoose/cmake-build-debug/src/scenes/volume-rendering && /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/volume-rendering.dir/volume_renderpass.cpp.o -c /home/kruseborn/git/mongoose/src/scenes/volume-rendering/volume_renderpass.cpp

src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_renderpass.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/volume-rendering.dir/volume_renderpass.cpp.i"
	cd /home/kruseborn/git/mongoose/cmake-build-debug/src/scenes/volume-rendering && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kruseborn/git/mongoose/src/scenes/volume-rendering/volume_renderpass.cpp > CMakeFiles/volume-rendering.dir/volume_renderpass.cpp.i

src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_renderpass.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/volume-rendering.dir/volume_renderpass.cpp.s"
	cd /home/kruseborn/git/mongoose/cmake-build-debug/src/scenes/volume-rendering && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kruseborn/git/mongoose/src/scenes/volume-rendering/volume_renderpass.cpp -o CMakeFiles/volume-rendering.dir/volume_renderpass.cpp.s

src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_main.cpp.o: src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/flags.make
src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_main.cpp.o: ../src/scenes/volume-rendering/volume_main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kruseborn/git/mongoose/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_main.cpp.o"
	cd /home/kruseborn/git/mongoose/cmake-build-debug/src/scenes/volume-rendering && /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/volume-rendering.dir/volume_main.cpp.o -c /home/kruseborn/git/mongoose/src/scenes/volume-rendering/volume_main.cpp

src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/volume-rendering.dir/volume_main.cpp.i"
	cd /home/kruseborn/git/mongoose/cmake-build-debug/src/scenes/volume-rendering && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kruseborn/git/mongoose/src/scenes/volume-rendering/volume_main.cpp > CMakeFiles/volume-rendering.dir/volume_main.cpp.i

src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/volume-rendering.dir/volume_main.cpp.s"
	cd /home/kruseborn/git/mongoose/cmake-build-debug/src/scenes/volume-rendering && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kruseborn/git/mongoose/src/scenes/volume-rendering/volume_main.cpp -o CMakeFiles/volume-rendering.dir/volume_main.cpp.s

src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_utils.cpp.o: src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/flags.make
src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_utils.cpp.o: ../src/scenes/volume-rendering/volume_utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kruseborn/git/mongoose/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_utils.cpp.o"
	cd /home/kruseborn/git/mongoose/cmake-build-debug/src/scenes/volume-rendering && /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/volume-rendering.dir/volume_utils.cpp.o -c /home/kruseborn/git/mongoose/src/scenes/volume-rendering/volume_utils.cpp

src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/volume-rendering.dir/volume_utils.cpp.i"
	cd /home/kruseborn/git/mongoose/cmake-build-debug/src/scenes/volume-rendering && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kruseborn/git/mongoose/src/scenes/volume-rendering/volume_utils.cpp > CMakeFiles/volume-rendering.dir/volume_utils.cpp.i

src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/volume-rendering.dir/volume_utils.cpp.s"
	cd /home/kruseborn/git/mongoose/cmake-build-debug/src/scenes/volume-rendering && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kruseborn/git/mongoose/src/scenes/volume-rendering/volume_utils.cpp -o CMakeFiles/volume-rendering.dir/volume_utils.cpp.s

# Object files for target volume-rendering
volume__rendering_OBJECTS = \
"CMakeFiles/volume-rendering.dir/volume_scene.cpp.o" \
"CMakeFiles/volume-rendering.dir/volume_rendering.cpp.o" \
"CMakeFiles/volume-rendering.dir/volume_renderpass.cpp.o" \
"CMakeFiles/volume-rendering.dir/volume_main.cpp.o" \
"CMakeFiles/volume-rendering.dir/volume_utils.cpp.o"

# External object files for target volume-rendering
volume__rendering_EXTERNAL_OBJECTS =

../build/Debug/bin/volume-rendering: src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_scene.cpp.o
../build/Debug/bin/volume-rendering: src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_rendering.cpp.o
../build/Debug/bin/volume-rendering: src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_renderpass.cpp.o
../build/Debug/bin/volume-rendering: src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_main.cpp.o
../build/Debug/bin/volume-rendering: src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/volume_utils.cpp.o
../build/Debug/bin/volume-rendering: src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/build.make
../build/Debug/bin/volume-rendering: ../build/Debug/lib/libmg-engine.a
../build/Debug/bin/volume-rendering: /home/kruseborn/vulkan/1.1.108.0/x86_64/lib/libvulkan.so
../build/Debug/bin/volume-rendering: ../build/Debug/lib/libglfw3.a
../build/Debug/bin/volume-rendering: /usr/lib/librt.so
../build/Debug/bin/volume-rendering: /usr/lib/libm.so
../build/Debug/bin/volume-rendering: /usr/lib/libX11.so
../build/Debug/bin/volume-rendering: /usr/lib/libXrandr.so
../build/Debug/bin/volume-rendering: /usr/lib/libXinerama.so
../build/Debug/bin/volume-rendering: /usr/lib/libXxf86vm.so
../build/Debug/bin/volume-rendering: /usr/lib/libXcursor.so
../build/Debug/bin/volume-rendering: ../build/Debug/lib/liblodepng.a
../build/Debug/bin/volume-rendering: ../build/Debug/lib/libimgui.a
../build/Debug/bin/volume-rendering: ../build/Debug/lib/libfreetyped.a
../build/Debug/bin/volume-rendering: /usr/lib/libz.so
../build/Debug/bin/volume-rendering: /usr/lib/libbz2.so
../build/Debug/bin/volume-rendering: /usr/lib/libpng.so
../build/Debug/bin/volume-rendering: /usr/lib/libz.so
../build/Debug/bin/volume-rendering: /usr/lib/libbz2.so
../build/Debug/bin/volume-rendering: /usr/lib/libpng.so
../build/Debug/bin/volume-rendering: src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kruseborn/git/mongoose/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable ../../../../build/Debug/bin/volume-rendering"
	cd /home/kruseborn/git/mongoose/cmake-build-debug/src/scenes/volume-rendering && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/volume-rendering.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/build: ../build/Debug/bin/volume-rendering

.PHONY : src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/build

src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/clean:
	cd /home/kruseborn/git/mongoose/cmake-build-debug/src/scenes/volume-rendering && $(CMAKE_COMMAND) -P CMakeFiles/volume-rendering.dir/cmake_clean.cmake
.PHONY : src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/clean

src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/depend:
	cd /home/kruseborn/git/mongoose/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kruseborn/git/mongoose /home/kruseborn/git/mongoose/src/scenes/volume-rendering /home/kruseborn/git/mongoose/cmake-build-debug /home/kruseborn/git/mongoose/cmake-build-debug/src/scenes/volume-rendering /home/kruseborn/git/mongoose/cmake-build-debug/src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/scenes/volume-rendering/CMakeFiles/volume-rendering.dir/depend

