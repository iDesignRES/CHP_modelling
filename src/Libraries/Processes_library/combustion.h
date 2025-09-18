#pragma once

#include <string>
#include <vector>

#include "../../Parameters.h"
#include "../Flows_library/flow_definitions.h"

// Declaration of calculate_fuel_combustion_properties
void calculate_fuel_combustion_properties(flow fuel, object &prop);

// Declaration of solid_fuel_boiler
void solid_fuel_boiler(std::vector<flow> &fuel, std::vector<flow> &comb_air,
             flow &flue_gas, flow &bottom_ash, flow &fly_ash,
             object &comb);
