# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/xiaxia0414/myProject/ddz-Server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted

# Include any dependencies generated for this target.
include reactor/CMakeFiles/reactor.dir/depend.make

# Include the progress variables for this target.
include reactor/CMakeFiles/reactor.dir/progress.make

# Include the compile flags for this target's objects.
include reactor/CMakeFiles/reactor.dir/flags.make

reactor/CMakeFiles/reactor.dir/Dispatcher.cpp.o: reactor/CMakeFiles/reactor.dir/flags.make
reactor/CMakeFiles/reactor.dir/Dispatcher.cpp.o: ../reactor/Dispatcher.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object reactor/CMakeFiles/reactor.dir/Dispatcher.cpp.o"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted/reactor && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/reactor.dir/Dispatcher.cpp.o -c /home/xiaxia0414/myProject/ddz-Server/reactor/Dispatcher.cpp

reactor/CMakeFiles/reactor.dir/Dispatcher.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/reactor.dir/Dispatcher.cpp.i"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted/reactor && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xiaxia0414/myProject/ddz-Server/reactor/Dispatcher.cpp > CMakeFiles/reactor.dir/Dispatcher.cpp.i

reactor/CMakeFiles/reactor.dir/Dispatcher.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/reactor.dir/Dispatcher.cpp.s"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted/reactor && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xiaxia0414/myProject/ddz-Server/reactor/Dispatcher.cpp -o CMakeFiles/reactor.dir/Dispatcher.cpp.s

reactor/CMakeFiles/reactor.dir/EpollDispatcher.cpp.o: reactor/CMakeFiles/reactor.dir/flags.make
reactor/CMakeFiles/reactor.dir/EpollDispatcher.cpp.o: ../reactor/EpollDispatcher.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object reactor/CMakeFiles/reactor.dir/EpollDispatcher.cpp.o"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted/reactor && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/reactor.dir/EpollDispatcher.cpp.o -c /home/xiaxia0414/myProject/ddz-Server/reactor/EpollDispatcher.cpp

reactor/CMakeFiles/reactor.dir/EpollDispatcher.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/reactor.dir/EpollDispatcher.cpp.i"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted/reactor && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xiaxia0414/myProject/ddz-Server/reactor/EpollDispatcher.cpp > CMakeFiles/reactor.dir/EpollDispatcher.cpp.i

reactor/CMakeFiles/reactor.dir/EpollDispatcher.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/reactor.dir/EpollDispatcher.cpp.s"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted/reactor && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xiaxia0414/myProject/ddz-Server/reactor/EpollDispatcher.cpp -o CMakeFiles/reactor.dir/EpollDispatcher.cpp.s

reactor/CMakeFiles/reactor.dir/EventLoop.cpp.o: reactor/CMakeFiles/reactor.dir/flags.make
reactor/CMakeFiles/reactor.dir/EventLoop.cpp.o: ../reactor/EventLoop.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object reactor/CMakeFiles/reactor.dir/EventLoop.cpp.o"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted/reactor && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/reactor.dir/EventLoop.cpp.o -c /home/xiaxia0414/myProject/ddz-Server/reactor/EventLoop.cpp

reactor/CMakeFiles/reactor.dir/EventLoop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/reactor.dir/EventLoop.cpp.i"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted/reactor && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xiaxia0414/myProject/ddz-Server/reactor/EventLoop.cpp > CMakeFiles/reactor.dir/EventLoop.cpp.i

reactor/CMakeFiles/reactor.dir/EventLoop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/reactor.dir/EventLoop.cpp.s"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted/reactor && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xiaxia0414/myProject/ddz-Server/reactor/EventLoop.cpp -o CMakeFiles/reactor.dir/EventLoop.cpp.s

# Object files for target reactor
reactor_OBJECTS = \
"CMakeFiles/reactor.dir/Dispatcher.cpp.o" \
"CMakeFiles/reactor.dir/EpollDispatcher.cpp.o" \
"CMakeFiles/reactor.dir/EventLoop.cpp.o"

# External object files for target reactor
reactor_EXTERNAL_OBJECTS =

reactor/libreactor.a: reactor/CMakeFiles/reactor.dir/Dispatcher.cpp.o
reactor/libreactor.a: reactor/CMakeFiles/reactor.dir/EpollDispatcher.cpp.o
reactor/libreactor.a: reactor/CMakeFiles/reactor.dir/EventLoop.cpp.o
reactor/libreactor.a: reactor/CMakeFiles/reactor.dir/build.make
reactor/libreactor.a: reactor/CMakeFiles/reactor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libreactor.a"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted/reactor && $(CMAKE_COMMAND) -P CMakeFiles/reactor.dir/cmake_clean_target.cmake
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted/reactor && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/reactor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
reactor/CMakeFiles/reactor.dir/build: reactor/libreactor.a

.PHONY : reactor/CMakeFiles/reactor.dir/build

reactor/CMakeFiles/reactor.dir/clean:
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted/reactor && $(CMAKE_COMMAND) -P CMakeFiles/reactor.dir/cmake_clean.cmake
.PHONY : reactor/CMakeFiles/reactor.dir/clean

reactor/CMakeFiles/reactor.dir/depend:
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xiaxia0414/myProject/ddz-Server /home/xiaxia0414/myProject/ddz-Server/reactor /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted/reactor /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-remoted/reactor/CMakeFiles/reactor.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : reactor/CMakeFiles/reactor.dir/depend

