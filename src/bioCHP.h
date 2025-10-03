#pragma once

#include <string>
#include <toml++/toml.hpp>
#include <vector>
#include "Parameters.h"

/**
 * @brief Main bioCHP model function
 *
 * INPUTS
 * @param feed_def: name of each biomass feedstock
 * @param Yj: mass fraction of each biomass feedstock
 * @param YH2Oj: moisture of each biomass feedstock
 * @param W_el: electric power output (MW_el)
 * @param Qk: heat demand (MW)
 * @param Tk_in: Return temperature for each heat demand (district heating)
 * @param Tk_out: Supply temperature for each heat demand (district heating)
 *
 * OUTPUTS
 * @param Mj: Required mass flow of each biomass feedstock
 * @param Q_prod: calculated heat production (MW)
 * @param W_el_prod: calculated electric power production (MW)
 * @param C_inv: Investment cost
 * @param C_op: Total operating cost
 * @param C_op_var: Variable operating cost
 *
 * @return the bioCHP object containing all input and output parameters
 */
object bioCHP_plant(std::vector<std::string> fuel_def, std::vector<double> Yj,
                    std::vector<double> YH2Oj, double W_el,
                    std::vector<double> Qk, std::vector<double> Tk_in,
                    std::vector<double> Tk_out);

/**
 * @brief Creates and returns a bioCHP_plant object based on the provided TOML
 * configuration.
 *
 * This function parses the given TOML table and initializes a bioCHP_plant
 * object with the corresponding parameters specified in the configuration. It
 * is used to instantiate a bioCHP (biomass Combined Heat and Power) plant model
 * for further simulation or analysis.
 *
 * @param tbl A constant reference to a toml::table containing the configuration
 * parameters for the bioCHP plant.
 * @return An object representing the configured bioCHP plant.
 */
object bioCHP_plant(const toml::table& tbl);

/**
 * @brief Creates a bioCHP_plant object by reading configuration from a
 * specified input file.
 *
 * This function parses the provided input file and initializes a bioCHP_plant
 * object with the parameters and settings defined within the file.
 *
 * @param input_file The path to the input file containing the bioCHP plant
 * configuration.
 * @return An object representing the initialized bioCHP plant.
 *
 * @throws std::runtime_error If the file cannot be opened or parsed correctly.
 */
object bioCHP_plant(const std::string& input_file);

/**
 * @brief Extracts output values from a bioCHP object.
 *
 * This function retrieves output parameters from the given bioCHP object,
 * including produced heat (Q_prod), produced electrical work (W_el_prod),
 * investment cost (C_inv), operational cost (C_op), variable operational cost
 * (C_op_var), and a vector of additional output metrics (Mj).
 *
 * @param bioCHP         The bioCHP object from which to extract outputs.
 * @param Q_prod         Output: produced heat.
 * @param W_el_prod      Output: produced electrical work.
 * @param C_inv          Output: investment cost.
 * @param C_op           Output: operational cost.
 * @param C_op_var       Output: variable operational cost.
 * @param Mj             Output: vector of additional output metrics.
 */
void extract_outputs(object bioCHP, double* Q_prod, double* W_el_prod,
                     double* C_inv, double* C_op, double* C_op_var,
                     std::vector<double>& Mj);
