#pragma once

#include <string>
#include <vector>

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
 * @return false if some error/inconsistency, or true otherwise
 */
bool bioCHP_plant(std::vector<std::string> fuel_def, std::vector<double> Yj,
                  std::vector<double> YH2Oj, double W_el,
                  std::vector<double> Qk, std::vector<double> Tk_in,
                  std::vector<double> Tk_out, std::vector<double> &Mj,
                  double &Q_prod, double &W_el_prod, double &C_inv,
                  double &C_op, double &C_op_var);
