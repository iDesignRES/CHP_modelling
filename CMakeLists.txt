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
add_executable(test_parameters Tests/Parameters_test.cpp)
add_executable(test_thermodynamics Tests/Thermodynamics_test.cpp)
add_executable(test_flows Tests/Flows_test.cpp)
add_executable(test_bioCHP_0-1 Tests/bioCHP_test_0-1.cpp)
add_executable(test_bioCHP_0-2 Tests/bioCHP_test_0-2.cpp)
add_executable(test_bioCHP_0-3 Tests/bioCHP_test_0-3.cpp)
add_executable(test_bioCHP_0-4 Tests/bioCHP_test_0-4.cpp)
add_executable(test_bioCHP_0-5 Tests/bioCHP_test_0-5.cpp)
add_executable(test_bioCHP_1 Tests/bioCHP_test_1.cpp)
add_executable(test_bioCHP_2 Tests/bioCHP_test_2.cpp)
add_executable(test_bioCHP_3 Tests/bioCHP_test_3.cpp)
add_executable(test_bioCHP_4-1 Tests/bioCHP_test_4-1.cpp)
add_executable(test_bioCHP_4-2 Tests/bioCHP_test_4-2.cpp)
add_executable(test_bioCHP_4 Tests/bioCHP_wrapper_test.cpp)

add_test(NAME MyTest_thermodynamics COMMAND test_thermodynamics)
add_test(NAME MyTest_flows COMMAND test_flows)
add_test(NAME MyTest_parameters COMMAND test_parameters)
add_test(NAME MyTest_bioCHP_0-1 COMMAND test_bioCHP_0-1)
add_test(NAME MyTest_bioCHP_0-2 COMMAND test_bioCHP_0-2)
add_test(NAME MyTest_bioCHP_0-3 COMMAND test_bioCHP_0-3)
add_test(NAME MyTest_bioCHP_0-4 COMMAND test_bioCHP_0-4)
add_test(NAME MyTest_bioCHP_0-5 COMMAND test_bioCHP_0-5)
add_test(NAME MyTest_bioCHP_1 COMMAND test_bioCHP_1)
add_test(NAME MyTest_bioCHP_2 COMMAND test_bioCHP_2)
add_test(NAME MyTest_bioCHP_3 COMMAND test_bioCHP_3)
add_test(NAME MyTest_bioCHP_4-1-1 COMMAND test_bioCHP_4-1)
add_test(NAME MyTest_bioCHP_4-1-2 COMMAND test_bioCHP_4 1)
add_test(NAME MyTest_bioCHP_4-2-1 COMMAND test_bioCHP_4-2)
add_test(NAME MyTest_bioCHP_4-2-2 COMMAND test_bioCHP_4 2)