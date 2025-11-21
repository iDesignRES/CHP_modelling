#pragma once

#include <string>
#include <vector>

#include "../../Parameters.h"
#include "../Flows_library/flow_definitions.h"

/**
 * @brief function to calculate combustion properties
 *
 * @input fuel = flow with fuel composition
 *
 * @output &prop = object containing calculated combustion properties as
 * parameters
 */
void calculate_fuel_combustion_properties(flow fuel, object &prop);

/**
 * @brief function to calculate a boiler using a solid fuel mixture
 *
 * @input &fuel = flow vector representing all solid fuels
 * @input &comb = boiler object with input boiler parameters
 * @output &comb_air = flow vector with calculated primary and secondary
 * combustion air flows
 * @output &flue_gas = flow with calculated flue gas
 * @output &bottom_ash = flow with calculated bottom ash
 * @output &fly_ash = flow with calculated fly ash
 * @output &comb = boiler object with calculated output parameters
 */
void solid_fuel_boiler(std::vector<flow> &fuel, std::vector<flow> &comb_air,
                       flow &flue_gas, flow &bottom_ash, flow &fly_ash,
                       object &comb);
