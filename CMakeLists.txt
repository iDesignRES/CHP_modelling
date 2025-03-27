cmake_minimum_required(VERSION 3.10)

# Set the project name
project(CHP_model)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(test1 Tests/bioCHP_test_1.cpp)
