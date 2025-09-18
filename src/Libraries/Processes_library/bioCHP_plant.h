#pragma once

#include <string>
#include <vector>

#include "../../Parameters.h"
#include "../Flows_library/flow_definitions.h"

/**
 * @brief function to get feedstock defined in plant object
 *
 * @input &plant pointer to object with feedstock specification
 *
 * @return &f = flow vector containing all feedstock
 */
void get_feedstock(std::vector<flow> &f, object &plant);

/**
 * @brief function to calculate the bioCHP plant model
 *
 * @input &bioCHP object to object with bioCHP input parameters
 *
 * @return &bioCHP object with calculated output parameters
 */
void bioCHP_plant_model(object &bioCHP);
