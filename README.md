## bioCHP plant model

`bioCHP_plant` is a c++ module with functionality to model mass and energy flows, investment cost and operational cost of a combined heat and power production plant using biomass as feedstock. The documentation can be found at \ref detailed_model_documentation.

## Model test

The bioCHP plant model can be tested separately using the c++ files bioCHP_function_test_[number].cpp in the main directory:
 - bioCHP_function_test_1.cpp: single biomass feedstock with specified electric power production and no heat demand
 - bioCHP_function_test_2.cpp: multiple biomass feedstock with specified electric power production and no heat demands
 - bioCHP_function_test_3.cpp: multiple biomass feedstock with specified electric power production and several heat demand

## Project Funding

The development of `bioCHP_plant` was funded by the European Union’s Horizon Europe research and innovation programme in the project [iDesignRES](https://idesignres.eu/) under grant agreement [101095849](https://doi.org/10.3030/101095849).
