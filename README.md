## bioCHP plant model

[![Build Status](https://github.com/iDesignRES/CHP_modelling/workflows/CI/badge.svg)](https://github.com/iDesignRES/CHP_modelling/actions?query=workflow%3ACI)
[![Stable](https://img.shields.io/badge/docs-stable-blue.svg)](https://idesignres.github.io/CHP_modelling/stable/)
[![In Development](https://img.shields.io/badge/docs-dev-blue.svg)](https://idesignres.github.io/CHP_modelling/dev/)
![clang-format](https://img.shields.io/badge/clang--format-Google%20style-blue?logo=clang&logoColor=white)

This README provides an overview of the iDesignRES biomass-based Combined Heat and Power (bioCHP) Model within iDesignRES.

It is handled by SINTEF Energy Research and part of WP1, Task number Task 1.3. 

## Purpose of the model

The main purpose of this model is to evaluate numerically the overall process and economics of converting solid biomass to electricity and heat. Important features of the model are: 1) supply of different types of biomass feedstock from multiple sources, 2) specification of multiple heat outputs (as steam or district heating) defined in terms of thermal power, temperature and pressure. The overall process is calculated in terms of the mass and energy flows. The economics is calculated in terms of capital investment (CAPEX), variable and fixed operating cost (OPEX). The implementation of this model aims to extend the EnergyModelsBase (EMB) for including bioenergy technologies, which have been recognized as an important contribution in present and future renewable energy mix.  


## Model design philosophy

The model is programmed in C++ and includes the following elements:
- Flows as objects (classes) defined by mass flow rates, chemical composition and thermodynamics.
- Processes: as functions transforming the flows according to physical and thermochemical processes
- Equipment as objects (classes) within processes, defined by their type (ex. pump, boiler, heat exchanger, etc) and main design specifications.
- Cost module, evaluates CAPEX and OPEX using flows and equipment as inputs. CAPEX is calculated in terms of the total permanent investment, which includes the installed cost of all equipment and additional capital cost factors associated with piping, electric system and instrumentation and control, land, civil work for site preparation and construction of buildings, engineering and contingencies for civil work and process equipment, and project development and licenses. OPEX is the annual operating costs, calculated as the sum of variable direct operational cost (proportional the annual operating time) and fixed indirect operational costs (required for having the plant in activity).

Detailed description of the model can be found at \ref detailed_model_documentation.


## Core assumptions

- All material and energy flows are calculated assuming state-state behavior of all processes. 
- Annually, the plant has a nominal operating time is 8000 hours and one maintenance shutdown of 736 hours during the summer.   
- The cost of supply of each biomass feedstock is proportional to the total mass flow rate assuming constant unit transportation cost per unit mass and distance and constant unit harvesting.     
- The combustion of biomass assumes a constant excess-air ratio of 1.2.  
- The steam turbine power outputs are calculated assuming constant isentropic efficiencies for each stage. 


## Usage

The bioCHP plant model can be tested through separate C++ files located in the [`Tests`]tests directory.
The tests are minimum working examples highlighting the main functionalities of the model.

## Model test

The bioCHP plant model can be tested separately using the c++ files bioCHP_function_test_[number].cpp in the main directory:
 - bioCHP_function_test_1.cpp: single biomass feedstock with specified electric power production and no heat demand
 - bioCHP_function_test_2.cpp: multiple biomass feedstock with specified electric power production and no heat demands
 - bioCHP_function_test_3.cpp: multiple biomass feedstock with specified electric power production and several heat demand

## Project Funding

The development of `bioCHP_plant` was funded by the European Union’s Horizon Europe research and innovation programme in the project [iDesignRES](https://idesignres.eu/) under grant agreement [101095849](https://doi.org/10.3030/101095849).
