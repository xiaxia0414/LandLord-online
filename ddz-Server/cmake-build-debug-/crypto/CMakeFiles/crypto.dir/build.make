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
CMAKE_BINARY_DIR = /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-

# Include any dependencies generated for this target.
include crypto/CMakeFiles/crypto.dir/depend.make

# Include the progress variables for this target.
include crypto/CMakeFiles/crypto.dir/progress.make

# Include the compile flags for this target's objects.
include crypto/CMakeFiles/crypto.dir/flags.make

crypto/CMakeFiles/crypto.dir/AesCrypto.cpp.o: crypto/CMakeFiles/crypto.dir/flags.make
crypto/CMakeFiles/crypto.dir/AesCrypto.cpp.o: ../crypto/AesCrypto.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object crypto/CMakeFiles/crypto.dir/AesCrypto.cpp.o"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/crypto && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/crypto.dir/AesCrypto.cpp.o -c /home/xiaxia0414/myProject/ddz-Server/crypto/AesCrypto.cpp

crypto/CMakeFiles/crypto.dir/AesCrypto.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/crypto.dir/AesCrypto.cpp.i"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/crypto && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xiaxia0414/myProject/ddz-Server/crypto/AesCrypto.cpp > CMakeFiles/crypto.dir/AesCrypto.cpp.i

crypto/CMakeFiles/crypto.dir/AesCrypto.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/crypto.dir/AesCrypto.cpp.s"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/crypto && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xiaxia0414/myProject/ddz-Server/crypto/AesCrypto.cpp -o CMakeFiles/crypto.dir/AesCrypto.cpp.s

crypto/CMakeFiles/crypto.dir/Base64.cpp.o: crypto/CMakeFiles/crypto.dir/flags.make
crypto/CMakeFiles/crypto.dir/Base64.cpp.o: ../crypto/Base64.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object crypto/CMakeFiles/crypto.dir/Base64.cpp.o"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/crypto && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/crypto.dir/Base64.cpp.o -c /home/xiaxia0414/myProject/ddz-Server/crypto/Base64.cpp

crypto/CMakeFiles/crypto.dir/Base64.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/crypto.dir/Base64.cpp.i"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/crypto && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xiaxia0414/myProject/ddz-Server/crypto/Base64.cpp > CMakeFiles/crypto.dir/Base64.cpp.i

crypto/CMakeFiles/crypto.dir/Base64.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/crypto.dir/Base64.cpp.s"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/crypto && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xiaxia0414/myProject/ddz-Server/crypto/Base64.cpp -o CMakeFiles/crypto.dir/Base64.cpp.s

crypto/CMakeFiles/crypto.dir/Hash.cpp.o: crypto/CMakeFiles/crypto.dir/flags.make
crypto/CMakeFiles/crypto.dir/Hash.cpp.o: ../crypto/Hash.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object crypto/CMakeFiles/crypto.dir/Hash.cpp.o"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/crypto && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/crypto.dir/Hash.cpp.o -c /home/xiaxia0414/myProject/ddz-Server/crypto/Hash.cpp

crypto/CMakeFiles/crypto.dir/Hash.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/crypto.dir/Hash.cpp.i"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/crypto && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xiaxia0414/myProject/ddz-Server/crypto/Hash.cpp > CMakeFiles/crypto.dir/Hash.cpp.i

crypto/CMakeFiles/crypto.dir/Hash.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/crypto.dir/Hash.cpp.s"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/crypto && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xiaxia0414/myProject/ddz-Server/crypto/Hash.cpp -o CMakeFiles/crypto.dir/Hash.cpp.s

crypto/CMakeFiles/crypto.dir/RsaCrypto.cpp.o: crypto/CMakeFiles/crypto.dir/flags.make
crypto/CMakeFiles/crypto.dir/RsaCrypto.cpp.o: ../crypto/RsaCrypto.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object crypto/CMakeFiles/crypto.dir/RsaCrypto.cpp.o"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/crypto && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/crypto.dir/RsaCrypto.cpp.o -c /home/xiaxia0414/myProject/ddz-Server/crypto/RsaCrypto.cpp

crypto/CMakeFiles/crypto.dir/RsaCrypto.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/crypto.dir/RsaCrypto.cpp.i"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/crypto && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xiaxia0414/myProject/ddz-Server/crypto/RsaCrypto.cpp > CMakeFiles/crypto.dir/RsaCrypto.cpp.i

crypto/CMakeFiles/crypto.dir/RsaCrypto.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/crypto.dir/RsaCrypto.cpp.s"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/crypto && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xiaxia0414/myProject/ddz-Server/crypto/RsaCrypto.cpp -o CMakeFiles/crypto.dir/RsaCrypto.cpp.s

# Object files for target crypto
crypto_OBJECTS = \
"CMakeFiles/crypto.dir/AesCrypto.cpp.o" \
"CMakeFiles/crypto.dir/Base64.cpp.o" \
"CMakeFiles/crypto.dir/Hash.cpp.o" \
"CMakeFiles/crypto.dir/RsaCrypto.cpp.o"

# External object files for target crypto
crypto_EXTERNAL_OBJECTS =

crypto/libcrypto.a: crypto/CMakeFiles/crypto.dir/AesCrypto.cpp.o
crypto/libcrypto.a: crypto/CMakeFiles/crypto.dir/Base64.cpp.o
crypto/libcrypto.a: crypto/CMakeFiles/crypto.dir/Hash.cpp.o
crypto/libcrypto.a: crypto/CMakeFiles/crypto.dir/RsaCrypto.cpp.o
crypto/libcrypto.a: crypto/CMakeFiles/crypto.dir/build.make
crypto/libcrypto.a: crypto/CMakeFiles/crypto.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX static library libcrypto.a"
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/crypto && $(CMAKE_COMMAND) -P CMakeFiles/crypto.dir/cmake_clean_target.cmake
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/crypto && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/crypto.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
crypto/CMakeFiles/crypto.dir/build: crypto/libcrypto.a

.PHONY : crypto/CMakeFiles/crypto.dir/build

crypto/CMakeFiles/crypto.dir/clean:
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/crypto && $(CMAKE_COMMAND) -P CMakeFiles/crypto.dir/cmake_clean.cmake
.PHONY : crypto/CMakeFiles/crypto.dir/clean

crypto/CMakeFiles/crypto.dir/depend:
	cd /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug- && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xiaxia0414/myProject/ddz-Server /home/xiaxia0414/myProject/ddz-Server/crypto /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug- /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/crypto /home/xiaxia0414/myProject/ddz-Server/cmake-build-debug-/crypto/CMakeFiles/crypto.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : crypto/CMakeFiles/crypto.dir/depend
