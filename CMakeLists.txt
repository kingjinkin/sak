cmake_minimum_required(VERSION 2.6)
project(SAK)

#find_package(OpenGL)

include_directories(
	${CMAKE_SOURCE_DIR}/src/base 
	${CMAKE_SOURCE_DIR}/lib/gtest
	${CMAKE_SOURCE_DIR}/lib/gtest/gtest 
	${CMAKE_SOURCE_DIR}/lib/gtest/gtest/internal
	)
link_directories(
	${CMAKE_SOURCE_DIR}/src/base  
	${CMAKE_SOURCE_DIR}/lib/gtest
	)

set(CMAKE_CXX_FLAGS "-g -Wall")

FOREACH( mylibfolder src tests )

    add_subdirectory(${mylibfolder})

ENDFOREACH()

