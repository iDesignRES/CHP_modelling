cmake_minimum_required(VERSION 3.10)

# Set the project name
project(CHP_model)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Collect all source files needed for the shared library
file(GLOB SRC_FILES src/*.cpp src/Libraries/*/*.cpp)

# Add shared library
add_library(bioCHP_library SHARED ${SRC_FILES})
target_compile_options(bioCHP_library PRIVATE)
set_target_properties(bioCHP_library PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
enable_testing()

# --- Helper: add a test executable + one or more ctest entries
function(add_chp_test target src)
  add_executable(${target} ${src})
  target_link_libraries(${target} PRIVATE bioCHP_library)

  if(ARGN) # optional arguments -> multiple test cases for same binary
    foreach(arg IN LISTS ARGN)
      add_test(NAME ${target}-${arg}
               COMMAND $<TARGET_FILE:${target}> ${arg})
    endforeach()
  else()
    add_test(NAME ${target}
             COMMAND $<TARGET_FILE:${target}>)
  endif()
endfunction()

# --- List your tests succinctly
add_chp_test(test_parameters        Tests/Parameters_test.cpp)
add_chp_test(test_thermodynamics    Tests/Thermodynamics_test.cpp)
add_chp_test(test_flows             Tests/Flows_test.cpp)
add_chp_test(test_bioCHP_0-1        Tests/bioCHP_test_0-1.cpp)
add_chp_test(test_bioCHP_0-2        Tests/bioCHP_test_0-2.cpp)
add_chp_test(test_bioCHP_0-3        Tests/bioCHP_test_0-3.cpp)
add_chp_test(test_bioCHP_0-4        Tests/bioCHP_test_0-4.cpp)
add_chp_test(test_bioCHP_0-5        Tests/bioCHP_test_0-5.cpp)
add_chp_test(test_bioCHP_1          Tests/bioCHP_test_1.cpp)
add_chp_test(test_bioCHP_2          Tests/bioCHP_test_2.cpp)
add_chp_test(test_bioCHP_3          Tests/bioCHP_test_3.cpp)
add_chp_test(test_bioCHP_4_1        Tests/bioCHP_test_4-1.cpp)
add_chp_test(test_bioCHP_4_2        Tests/bioCHP_test_4-2.cpp)
add_chp_test(test_bioCHP_4          Tests/bioCHP_wrapper_test.cpp 1 2)