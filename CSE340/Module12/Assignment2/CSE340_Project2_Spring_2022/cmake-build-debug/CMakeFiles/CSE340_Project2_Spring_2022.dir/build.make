# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/CSE340_Project2_Spring_2022.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/CSE340_Project2_Spring_2022.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/CSE340_Project2_Spring_2022.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CSE340_Project2_Spring_2022.dir/flags.make

CMakeFiles/CSE340_Project2_Spring_2022.dir/parser.cc.o: CMakeFiles/CSE340_Project2_Spring_2022.dir/flags.make
CMakeFiles/CSE340_Project2_Spring_2022.dir/parser.cc.o: ../parser.cc
CMakeFiles/CSE340_Project2_Spring_2022.dir/parser.cc.o: CMakeFiles/CSE340_Project2_Spring_2022.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CSE340_Project2_Spring_2022.dir/parser.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CSE340_Project2_Spring_2022.dir/parser.cc.o -MF CMakeFiles/CSE340_Project2_Spring_2022.dir/parser.cc.o.d -o CMakeFiles/CSE340_Project2_Spring_2022.dir/parser.cc.o -c /home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022/parser.cc

CMakeFiles/CSE340_Project2_Spring_2022.dir/parser.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CSE340_Project2_Spring_2022.dir/parser.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022/parser.cc > CMakeFiles/CSE340_Project2_Spring_2022.dir/parser.cc.i

CMakeFiles/CSE340_Project2_Spring_2022.dir/parser.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CSE340_Project2_Spring_2022.dir/parser.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022/parser.cc -o CMakeFiles/CSE340_Project2_Spring_2022.dir/parser.cc.s

CMakeFiles/CSE340_Project2_Spring_2022.dir/lexer.cc.o: CMakeFiles/CSE340_Project2_Spring_2022.dir/flags.make
CMakeFiles/CSE340_Project2_Spring_2022.dir/lexer.cc.o: ../lexer.cc
CMakeFiles/CSE340_Project2_Spring_2022.dir/lexer.cc.o: CMakeFiles/CSE340_Project2_Spring_2022.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/CSE340_Project2_Spring_2022.dir/lexer.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CSE340_Project2_Spring_2022.dir/lexer.cc.o -MF CMakeFiles/CSE340_Project2_Spring_2022.dir/lexer.cc.o.d -o CMakeFiles/CSE340_Project2_Spring_2022.dir/lexer.cc.o -c /home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022/lexer.cc

CMakeFiles/CSE340_Project2_Spring_2022.dir/lexer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CSE340_Project2_Spring_2022.dir/lexer.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022/lexer.cc > CMakeFiles/CSE340_Project2_Spring_2022.dir/lexer.cc.i

CMakeFiles/CSE340_Project2_Spring_2022.dir/lexer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CSE340_Project2_Spring_2022.dir/lexer.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022/lexer.cc -o CMakeFiles/CSE340_Project2_Spring_2022.dir/lexer.cc.s

CMakeFiles/CSE340_Project2_Spring_2022.dir/inputbuf.cc.o: CMakeFiles/CSE340_Project2_Spring_2022.dir/flags.make
CMakeFiles/CSE340_Project2_Spring_2022.dir/inputbuf.cc.o: ../inputbuf.cc
CMakeFiles/CSE340_Project2_Spring_2022.dir/inputbuf.cc.o: CMakeFiles/CSE340_Project2_Spring_2022.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/CSE340_Project2_Spring_2022.dir/inputbuf.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CSE340_Project2_Spring_2022.dir/inputbuf.cc.o -MF CMakeFiles/CSE340_Project2_Spring_2022.dir/inputbuf.cc.o.d -o CMakeFiles/CSE340_Project2_Spring_2022.dir/inputbuf.cc.o -c /home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022/inputbuf.cc

CMakeFiles/CSE340_Project2_Spring_2022.dir/inputbuf.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CSE340_Project2_Spring_2022.dir/inputbuf.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022/inputbuf.cc > CMakeFiles/CSE340_Project2_Spring_2022.dir/inputbuf.cc.i

CMakeFiles/CSE340_Project2_Spring_2022.dir/inputbuf.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CSE340_Project2_Spring_2022.dir/inputbuf.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022/inputbuf.cc -o CMakeFiles/CSE340_Project2_Spring_2022.dir/inputbuf.cc.s

# Object files for target CSE340_Project2_Spring_2022
CSE340_Project2_Spring_2022_OBJECTS = \
"CMakeFiles/CSE340_Project2_Spring_2022.dir/parser.cc.o" \
"CMakeFiles/CSE340_Project2_Spring_2022.dir/lexer.cc.o" \
"CMakeFiles/CSE340_Project2_Spring_2022.dir/inputbuf.cc.o"

# External object files for target CSE340_Project2_Spring_2022
CSE340_Project2_Spring_2022_EXTERNAL_OBJECTS =

CSE340_Project2_Spring_2022: CMakeFiles/CSE340_Project2_Spring_2022.dir/parser.cc.o
CSE340_Project2_Spring_2022: CMakeFiles/CSE340_Project2_Spring_2022.dir/lexer.cc.o
CSE340_Project2_Spring_2022: CMakeFiles/CSE340_Project2_Spring_2022.dir/inputbuf.cc.o
CSE340_Project2_Spring_2022: CMakeFiles/CSE340_Project2_Spring_2022.dir/build.make
CSE340_Project2_Spring_2022: CMakeFiles/CSE340_Project2_Spring_2022.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable CSE340_Project2_Spring_2022"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CSE340_Project2_Spring_2022.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CSE340_Project2_Spring_2022.dir/build: CSE340_Project2_Spring_2022
.PHONY : CMakeFiles/CSE340_Project2_Spring_2022.dir/build

CMakeFiles/CSE340_Project2_Spring_2022.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CSE340_Project2_Spring_2022.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CSE340_Project2_Spring_2022.dir/clean

CMakeFiles/CSE340_Project2_Spring_2022.dir/depend:
	cd /home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022 /home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022 /home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022/cmake-build-debug /home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022/cmake-build-debug /home/claudiordgz/asu-mcs/CSE340/Module12/Assignment2/CSE340_Project2_Spring_2022/cmake-build-debug/CMakeFiles/CSE340_Project2_Spring_2022.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CSE340_Project2_Spring_2022.dir/depend

