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
CMAKE_SOURCE_DIR = /home/kin/workspace/sak

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kin/workspace/sak/mybuild

# Include any dependencies generated for this target.
include tests/CMakeFiles/test.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/test.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/test.dir/flags.make

tests/CMakeFiles/test.dir/ThreadPoolTest.cpp.o: tests/CMakeFiles/test.dir/flags.make
tests/CMakeFiles/test.dir/ThreadPoolTest.cpp.o: ../tests/ThreadPoolTest.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/kin/workspace/sak/mybuild/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tests/CMakeFiles/test.dir/ThreadPoolTest.cpp.o"
	cd /home/kin/workspace/sak/mybuild/tests && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/test.dir/ThreadPoolTest.cpp.o -c /home/kin/workspace/sak/tests/ThreadPoolTest.cpp

tests/CMakeFiles/test.dir/ThreadPoolTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/ThreadPoolTest.cpp.i"
	cd /home/kin/workspace/sak/mybuild/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/kin/workspace/sak/tests/ThreadPoolTest.cpp > CMakeFiles/test.dir/ThreadPoolTest.cpp.i

tests/CMakeFiles/test.dir/ThreadPoolTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/ThreadPoolTest.cpp.s"
	cd /home/kin/workspace/sak/mybuild/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/kin/workspace/sak/tests/ThreadPoolTest.cpp -o CMakeFiles/test.dir/ThreadPoolTest.cpp.s

tests/CMakeFiles/test.dir/ThreadPoolTest.cpp.o.requires:
.PHONY : tests/CMakeFiles/test.dir/ThreadPoolTest.cpp.o.requires

tests/CMakeFiles/test.dir/ThreadPoolTest.cpp.o.provides: tests/CMakeFiles/test.dir/ThreadPoolTest.cpp.o.requires
	$(MAKE) -f tests/CMakeFiles/test.dir/build.make tests/CMakeFiles/test.dir/ThreadPoolTest.cpp.o.provides.build
.PHONY : tests/CMakeFiles/test.dir/ThreadPoolTest.cpp.o.provides

tests/CMakeFiles/test.dir/ThreadPoolTest.cpp.o.provides.build: tests/CMakeFiles/test.dir/ThreadPoolTest.cpp.o

tests/CMakeFiles/test.dir/IdleEventTest.cpp.o: tests/CMakeFiles/test.dir/flags.make
tests/CMakeFiles/test.dir/IdleEventTest.cpp.o: ../tests/IdleEventTest.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/kin/workspace/sak/mybuild/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tests/CMakeFiles/test.dir/IdleEventTest.cpp.o"
	cd /home/kin/workspace/sak/mybuild/tests && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/test.dir/IdleEventTest.cpp.o -c /home/kin/workspace/sak/tests/IdleEventTest.cpp

tests/CMakeFiles/test.dir/IdleEventTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/IdleEventTest.cpp.i"
	cd /home/kin/workspace/sak/mybuild/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/kin/workspace/sak/tests/IdleEventTest.cpp > CMakeFiles/test.dir/IdleEventTest.cpp.i

tests/CMakeFiles/test.dir/IdleEventTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/IdleEventTest.cpp.s"
	cd /home/kin/workspace/sak/mybuild/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/kin/workspace/sak/tests/IdleEventTest.cpp -o CMakeFiles/test.dir/IdleEventTest.cpp.s

tests/CMakeFiles/test.dir/IdleEventTest.cpp.o.requires:
.PHONY : tests/CMakeFiles/test.dir/IdleEventTest.cpp.o.requires

tests/CMakeFiles/test.dir/IdleEventTest.cpp.o.provides: tests/CMakeFiles/test.dir/IdleEventTest.cpp.o.requires
	$(MAKE) -f tests/CMakeFiles/test.dir/build.make tests/CMakeFiles/test.dir/IdleEventTest.cpp.o.provides.build
.PHONY : tests/CMakeFiles/test.dir/IdleEventTest.cpp.o.provides

tests/CMakeFiles/test.dir/IdleEventTest.cpp.o.provides.build: tests/CMakeFiles/test.dir/IdleEventTest.cpp.o

tests/CMakeFiles/test.dir/testsak.cpp.o: tests/CMakeFiles/test.dir/flags.make
tests/CMakeFiles/test.dir/testsak.cpp.o: ../tests/testsak.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/kin/workspace/sak/mybuild/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tests/CMakeFiles/test.dir/testsak.cpp.o"
	cd /home/kin/workspace/sak/mybuild/tests && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/test.dir/testsak.cpp.o -c /home/kin/workspace/sak/tests/testsak.cpp

tests/CMakeFiles/test.dir/testsak.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/testsak.cpp.i"
	cd /home/kin/workspace/sak/mybuild/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/kin/workspace/sak/tests/testsak.cpp > CMakeFiles/test.dir/testsak.cpp.i

tests/CMakeFiles/test.dir/testsak.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/testsak.cpp.s"
	cd /home/kin/workspace/sak/mybuild/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/kin/workspace/sak/tests/testsak.cpp -o CMakeFiles/test.dir/testsak.cpp.s

tests/CMakeFiles/test.dir/testsak.cpp.o.requires:
.PHONY : tests/CMakeFiles/test.dir/testsak.cpp.o.requires

tests/CMakeFiles/test.dir/testsak.cpp.o.provides: tests/CMakeFiles/test.dir/testsak.cpp.o.requires
	$(MAKE) -f tests/CMakeFiles/test.dir/build.make tests/CMakeFiles/test.dir/testsak.cpp.o.provides.build
.PHONY : tests/CMakeFiles/test.dir/testsak.cpp.o.provides

tests/CMakeFiles/test.dir/testsak.cpp.o.provides.build: tests/CMakeFiles/test.dir/testsak.cpp.o

tests/CMakeFiles/test.dir/BitTest.cpp.o: tests/CMakeFiles/test.dir/flags.make
tests/CMakeFiles/test.dir/BitTest.cpp.o: ../tests/BitTest.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/kin/workspace/sak/mybuild/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tests/CMakeFiles/test.dir/BitTest.cpp.o"
	cd /home/kin/workspace/sak/mybuild/tests && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/test.dir/BitTest.cpp.o -c /home/kin/workspace/sak/tests/BitTest.cpp

tests/CMakeFiles/test.dir/BitTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/BitTest.cpp.i"
	cd /home/kin/workspace/sak/mybuild/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/kin/workspace/sak/tests/BitTest.cpp > CMakeFiles/test.dir/BitTest.cpp.i

tests/CMakeFiles/test.dir/BitTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/BitTest.cpp.s"
	cd /home/kin/workspace/sak/mybuild/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/kin/workspace/sak/tests/BitTest.cpp -o CMakeFiles/test.dir/BitTest.cpp.s

tests/CMakeFiles/test.dir/BitTest.cpp.o.requires:
.PHONY : tests/CMakeFiles/test.dir/BitTest.cpp.o.requires

tests/CMakeFiles/test.dir/BitTest.cpp.o.provides: tests/CMakeFiles/test.dir/BitTest.cpp.o.requires
	$(MAKE) -f tests/CMakeFiles/test.dir/build.make tests/CMakeFiles/test.dir/BitTest.cpp.o.provides.build
.PHONY : tests/CMakeFiles/test.dir/BitTest.cpp.o.provides

tests/CMakeFiles/test.dir/BitTest.cpp.o.provides.build: tests/CMakeFiles/test.dir/BitTest.cpp.o

# Object files for target test
test_OBJECTS = \
"CMakeFiles/test.dir/ThreadPoolTest.cpp.o" \
"CMakeFiles/test.dir/IdleEventTest.cpp.o" \
"CMakeFiles/test.dir/testsak.cpp.o" \
"CMakeFiles/test.dir/BitTest.cpp.o"

# External object files for target test
test_EXTERNAL_OBJECTS =

tests/test: tests/CMakeFiles/test.dir/ThreadPoolTest.cpp.o
tests/test: tests/CMakeFiles/test.dir/IdleEventTest.cpp.o
tests/test: tests/CMakeFiles/test.dir/testsak.cpp.o
tests/test: tests/CMakeFiles/test.dir/BitTest.cpp.o
tests/test: tests/CMakeFiles/test.dir/build.make
tests/test: src/base/libbase.a
tests/test: tests/CMakeFiles/test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable test"
	cd /home/kin/workspace/sak/mybuild/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/test.dir/build: tests/test
.PHONY : tests/CMakeFiles/test.dir/build

tests/CMakeFiles/test.dir/requires: tests/CMakeFiles/test.dir/ThreadPoolTest.cpp.o.requires
tests/CMakeFiles/test.dir/requires: tests/CMakeFiles/test.dir/IdleEventTest.cpp.o.requires
tests/CMakeFiles/test.dir/requires: tests/CMakeFiles/test.dir/testsak.cpp.o.requires
tests/CMakeFiles/test.dir/requires: tests/CMakeFiles/test.dir/BitTest.cpp.o.requires
.PHONY : tests/CMakeFiles/test.dir/requires

tests/CMakeFiles/test.dir/clean:
	cd /home/kin/workspace/sak/mybuild/tests && $(CMAKE_COMMAND) -P CMakeFiles/test.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/test.dir/clean

tests/CMakeFiles/test.dir/depend:
	cd /home/kin/workspace/sak/mybuild && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kin/workspace/sak /home/kin/workspace/sak/tests /home/kin/workspace/sak/mybuild /home/kin/workspace/sak/mybuild/tests /home/kin/workspace/sak/mybuild/tests/CMakeFiles/test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/test.dir/depend

