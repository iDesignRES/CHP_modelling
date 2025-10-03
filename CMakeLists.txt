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
add_library(bioCHP_wrapper SHARED ${SRC_FILES})
target_include_directories(bioCHP_wrapper PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tomlplusplus/include)
target_compile_options(bioCHP_wrapper PRIVATE)

if(MSVC)
    add_compile_options(/W4) # Enable all warnings for MSVC, do not treat as errors
elseif(WIN32)
    # MinGW on Windows – disable sanitizers
    add_compile_options(-Wall -Wextra -Wpedantic)
else()
    set(SANITIZER_FLAGS -fsanitize=address -fsanitize=undefined -g)
    add_compile_options(-Wall -Wextra -Wpedantic ${SANITIZER_FLAGS})
    add_link_options(${SANITIZER_FLAGS})
endif()

set_target_properties(bioCHP_wrapper PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# Add executable for main program
add_executable(bioCHP src/bioCHP.cpp)
target_link_libraries(bioCHP PRIVATE bioCHP_wrapper)
target_include_directories(bioCHP PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tomlplusplus/include)

enable_testing()

# Add helper function
function(add_chp_test target src)
  add_executable(${target} ${src})
  target_link_libraries(${target} PRIVATE bioCHP_wrapper)

  if(ARGN) # optional arguments -> multiple test cases for same binary
    foreach(arg IN LISTS ARGN)
      add_test(NAME ${target}-${arg}
               COMMAND $<TARGET_FILE:${target}> ${arg})
      target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tomlplusplus/include)
    endforeach()
  else()
    add_test(NAME ${target}
             COMMAND $<TARGET_FILE:${target}>)
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tomlplusplus/include)
  endif()
endfunction()

# Add tests
add_chp_test(test_parameters     Tests/Parameters_test.cpp)
add_chp_test(test_thermodynamics Tests/Thermodynamics_test.cpp)
add_chp_test(test_flows          Tests/Flows_test.cpp)
add_chp_test(test_bioCHP_4       Tests/bioCHP_wrapper_test.cpp 1 2)
add_chp_test(test_bioCHP         Tests/test_bioCHP.cpp)