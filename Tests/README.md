
This README provides instructions on how to set up the environment and run the test examples on both Linux and Windows for using the bioCHP model

## Model tests description

This repository contains separately c++ files bioCHP_function_test_[number].cpp:
 - Parameters_test.cpp: Parameters functionalities
 - Thermodynamics_test.cpp: Thermodynamics using NIST database and water-thermodynamics correlations
 - Flows_test.cpp: Flows functionalities
 - bioCHP_test_0-[1-5].cpp: errors in specifying the input parameters of the bioCHP plant
 - bioCHP_function_test_1.cpp: bioCHP plant using a single biomass feedstock with specified electric power production and no heat demands
 - bioCHP_function_test_2.cpp: bioCHP plant using two biomass feedstock with specified electric power production and no heat demands
 - bioCHP_function_test_3.cpp: bioCHP plant using two biomass feedstock with specified electric power production and several heat demands
 - bioCHP_function_test_4-[1,2].cpp: bioCHP plant using four biomass feedstock with specified electric power production and several heat demands
 - bioCHP_wrapper_test.cpp: bioCHP plant using four biomass feedstock with specified electric power production and several heat 

## Prerequisites
- **Git**: Ensure Git is installed on your system.
- **CMake**: Install CMake from CMake website.
- **C++ Compiler**: Install a C++ compiler (e.g., `g++` for Linux, `MinGW` or Visual Studio for Windows).

## Getting Started

### Cloning the Repository
First, clone the repository to your local computer:
```bash
mkdir [your_project_name]
cd [your_project_name]
git clone https://github.com/yourusername/your-repo.git
```

### Linux Setup

1. **Install Dependencies**:
   Ensure you have `g++` and CMake installed. You can install them using:
   ```bash
   sudo apt update
   sudo apt install g++ cmake
   ```

2. **Build and run the tests**:
   Create a build directory and compile:
   ```bash
   cd Test
   mkdir build
   cd build
   cmake ..
   make
   ctest --verbose
   ```

### Windows Setup
