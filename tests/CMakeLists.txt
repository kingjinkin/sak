
link_directories(${CMAKE_SOURCE_DIR}/src/base  ${CMAKE_SOURCE_DIR}/lib/gtest)

aux_source_directory(. TARGET_SRC)
add_executable(test ${TARGET_SRC})
target_link_libraries(test base gtest pthread)


