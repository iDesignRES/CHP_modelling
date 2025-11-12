## bioCHP plant model

This README provides an overview of the biomass-based Combined Heat and Power (bioCHP) Model.

[![Build Status](https://github.com/iDesignRES/CHP_modelling/workflows/CI/badge.svg)](https://github.com/iDesignRES/CHP_modelling/actions?query=workflow%3ACI)
[![Stable](https://img.shields.io/badge/docs-stable-blue.svg)](https://idesignres.github.io/CHP_modelling/stable/)
[![In Development](https://img.shields.io/badge/docs-dev-blue.svg)](https://idesignres.github.io/CHP_modelling/dev/)
![clang-format](https://img.shields.io/badge/clang--format-Google%20style-blue?logo=clang&logoColor=white)

## Purpose of the model

The main purpose of this model is to evaluate numerically the overall process and economics of converting solid biomass to electricity and heat. Important features of the model are: 1) supply of different types of biomass feedstock from multiple sources, 2) specification of multiple heat outputs (as steam or district heating) defined in terms of thermal power, temperature and pressure. The overall process is calculated in terms of the mass and energy flows. The economics is calculated in terms of capital investment (CAPEX), variable and fixed operating cost (OPEX).  

## Model design philosophy

The model is programmed in C++ and includes the following elements:
- Flows as objects (classes) defined by mass flow rates, chemical composition and thermodynamics.
- Processes: as functions transforming the flows according to physical and thermochemical processes
- Equipment as objects (classes) within processes, defined by their type (ex. pump, boiler, heat exchanger, etc) and main design specifications.
- Cost module, evaluates CAPEX and OPEX using flows and equipment as inputs. CAPEX is calculated in terms of the total permanent investment, which includes the installed cost of all equipment and additional capital cost factors associated with piping, electric system and instrumentation and control, land, civil work for site preparation and construction of buildings, engineering and contingencies for civil work and process equipment, and project development and licenses. OPEX is the annual operating costs, calculated as the sum of variable direct operational cost (proportional the annual operating time) and fixed indirect operational costs (required for having the plant in activity).

Detailed description of the model can be found at \ref detailed_model_documentation.

## Core assumptions

- All material and energy flows are calculated assuming state-state behavior of all processes.   
- The cost of supply of each biomass feedstock is proportional to the total mass flow rate assuming constant unit transportation cost per unit mass and distance and constant unit harvesting.     
- The combustion of biomass assumes a constant excess-air ratio of 1.2.  
- The steam turbine power outputs are calculated assuming constant isentropic efficiencies for each stage. 

<a name="quickstart"></a>
## Quick start - Linux and Windows Setup

### Install dependencies

Make sure you have `g++`, `CMake`, and `Conan` installed. 

1. **Linux setup**

    You can install dependencies with:

    ```bash
    sudo apt update
    sudo apt install -y g++ cmake python3-pip
    pip install conan
    conan profile detect
    ```
    
2. **Windows setup (PowerShell)**

    Open PowerShell as Administrator and run:

    ```powershell
    winget install -e --id Kitware.CMake
    winget install -e --id Git.Git
    winget install --id Python.Python.3.11 --source winget --scope user --override "PrependPath=1"
    ```

    Restart the PowerShell and run

    ```powershell
    pip install conan
    conan profile detect
    ```

### Build and run minimal viable case

Create a build directory and compile:

```bash
git clone https://github.com/iDesignRES/CHP_modelling.git
cd CHP_modelling
mkdir build && cd build
```

Install external dependencies (via Conan):

```bash
conan install .. --output-folder=. --build=missing -s compiler.cppstd=17 -s arch=x86_64
```

Configure and build the project:

```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE="${PWD}/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

You can now run the created `bioCHP` program:

1. On Linux

   ```bash
   ./bioCHP ../docs/example/bioCHP_input.toml output.txt
   ```

2. On Windows

   ```powershell
   ./Release/bioCHP ../docs/example/bioCHP_input.toml output.txt
   ```

This example takes the input parameters in `../docs/example/bioCHP_input.toml` and prints outputs to `output.txt`.

### Run tests

Following the above comands, the tests can be run with

```bash
ctest -C Release
```

## Project Funding

The development of `bioCHP_plant` was funded by the European Union’s Horizon Europe research and innovation programme in the project [iDesignRES](https://idesignres.eu/) under grant agreement [101095849](https://doi.org/10.3030/101095849).
