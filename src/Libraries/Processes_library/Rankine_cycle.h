#pragma once

#include "../../Parameters.h"
#include "../Flows_library/flow_definitions.h"

/**
 * @struct steam_turbine_parameters
 * @brief Structure with steam turbine parameters
 * @var steam_turbine_parameters::id
 * String, identifying name
 * @var steam_turbine_parameters::Mi
 * input steam mass flow rate (kg/s)
 * @var steam_turbine_parameters::Pi
 * input steam pressure (bar)
 * @var steam_turbine_parameters::Ti
 * input steam temperature (deg.C)
 * @var steam_turbine_parameters::qi
 * input steam quality (-)
 * @var steam_turbine_parameters::Po
 * output steam pressure (bar)
 * @var steam_turbine_parameters::To
 * Outlet steam temperature (deg.C)
 * @var steam_turbine_parameters::qo
 * Outlet steam quality (-)
 * @var steam_turbine_parameters::mu_isent
 * isentropic efficiency (-)
 * @var steam_turbine_parameters::W
 * output mechanical power (MW)
 * @var steam_turbine_parameters::w
 * output specific power per unit mass inlet steam (MJ/kg)
 * @var steam_turbine_parameters::Pext
 * vector with extractions pressure (bar)
 * @var steam_turbine_parameters::Mext
 * vector with extractions flows (kg/s)
 */
struct steam_turbine_parameters {
 public:
  std::string id = "";
  double Mi = 0.0, Pi = 0.0, Ti = 0.0, qi = 1.0;
  double Po = 0.0, To = 0.0, qo = 1.0;
  double mu_isent = 0.0, W = 0.0, w = 0.0;
  std::vector<double> Pext = {}, Mext = {};
  void assign_parameter_values(std::string, std::string, object);
};

/**
 * @brief function to calculate one steam turbine stage
 *
 * @input &in = flow vector representing input steam
 * @output &out = flow with calculated output steam
 * @input &ST = input steam turbine parameters
 * @output &ST = calculated output steam turbine parameters
 */
void steam_turbine(flow &in, flow &out, steam_turbine_parameters &ST);

/**
 * @brief function to calculate a steam turbine with bleeds
 *
 * @input &in = flow vector representing input steam
 * @output &out = flow with calculated output steam
 * @input &par = input steam turbine parameters
 * @output &par = calculated output steam turbine parameters
 */
void steam_turbine_model(flow &in, flow &out, object &par);

/**
 * @brief function to calculate a steam condenser
 *
 * @input &in = flow vector representing input steam
 * @output &out = flow with calculated output condensate
 * @input &par = input condenser parameters
 * @output &par = calculated output condenser parameters
 */
void steam_condenser(flow &steam, flow &cond, object &par);

/**
 * @brief function to calculate heat demands by district heating
 *
 * @input &par = input district heating parameters
 * @output &par = calculated output district heating parameters
 */
void district_heating(object &par);

/**
 * @brief function to calculate a rankine cycle model
 *
 * @input &par = input rankine cycle parameters
 * @output &par = calculated output rankine cycle parameters
 */
void rankine_cycle(object &par);
