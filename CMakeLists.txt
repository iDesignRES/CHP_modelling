cmake_minimum_required(VERSION 3.10)

# Set the project name
project(CHP_model)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add shared library
add_library(bioCHP_wrapper SHARED src/bioCHP_wrapper.cpp)
set_target_properties(bioCHP_wrapper PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
enable_testing()

# Add the executable
add_executable(test1 Tests/bioCHP_test_1.cpp)
add_executable(test2 Tests/bioCHP_test_2.cpp)
add_executable(test3 Tests/bioCHP_test_3.cpp)

add_test(NAME MyTest1 COMMAND test1)
add_test(NAME MyTest2 COMMAND test2)
add_test(NAME MyTest3 COMMAND test3)