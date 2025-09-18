#pragma once

#include <string>
#include <vector>

#include "flow_definitions.h"
/**
 * @brief function to mix two flow of the same composition
 *
 */
void mix_same_type_flows(flow &f1, flow &f2, flow &f);

/**
 * @brief General function to calculate a flow
 *
 */
void calculate_flow(std::string state_def);

/**
 * @brief Function to calculate solid fuel
 *
 */
void calculate_solid_fuel();

/**
 * @brief Function to calculate flow composition
 *
 */
void calculate_flow_composition();

/**
 * @brief Function to calculate flow properties
 *
 * @param state_def definition of thermodynamic state variables
 */
void calculate_flow_properties(std::string state_def);

/**
 * @brief Function to calculate species properties
 *
 * @param state_def definition of thermodynamic state variables
 */
void calculate_species_properties(std::string state_def);

/**
 * @brief Function to calculate flow parameters
 */
void calculate_flow_parameters();
