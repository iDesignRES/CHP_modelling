#pragma once

#include "../../Parameters.h"
#include "../Flows_library/flow_definitions.h"

struct steam_turbine_parameters {
 public:
  std::string id;
  double Mi, Pi, Ti, Po, mu_isent, W;
  std::vector<double> Pext, Mext;
  void assign_parameter_values(std::string sys_type, std::string sys_def, std::vector<parameter> &par);
};

void steam_turbine(flow &in, flow &out, steam_turbine_parameters &ST);

void steam_turbine_model(flow &in, flow &out, object &par);

void steam_condenser(flow &steam, flow &cond, object &par);

void district_heating(flow &dh_in, flow &dh_out, object &par);

void rankine_cycle(object &par);
