#include "bioCHP.h"
#include <cstddef>
#include <iostream>

#include "Libraries/Processes_library/bioCHP_plant.h"
#include "Parameters.h"
#include "utils.h"

bool bioCHP_plant(std::vector<std::string> fuel_def, std::vector<double> Yj,
                  std::vector<double> YH2Oj, double W_el,
                  std::vector<double> Qk, std::vector<double> Tk_in,
                  std::vector<double> Tk_out, std::vector<double> &Mj,
                  double &Q_prod, double &W_el_prod, double &C_inv,
                  double &C_op, double &C_op_var) {
  // Check specificatins of feedstock
  if (fuel_def.size() != Yj.size()) {
    std::cout << "number of specifications for Yj and fuel_def re different"
              << std::endl;
    return false;
  }

  // Check that all feedstock exist in the database
  for (std::size_t nf = 0; nf < fuel_def.size(); nf++) {
    if (!find_flow(fuel_def[nf])) {
      std::cout << "feedstock " + fuel_def[nf] + " not found in the database "
                << std::endl;
      for (std::size_t nff = 0; nff < fuel_def.size(); nff++) {
        Mj.push_back(0.0);
      }
      Q_prod = 0.0;
      W_el_prod = 0.0;
      C_inv = 0.0;
      C_op = 0.0;
      C_op_var = 0.0;
      return false;
    }
  }

  // Check specificatins of heat demands
  if (Qk.size() != Tk_in.size()) {
    std::cout << "number of specifications for Tk_in and Qk are different"
              << std::endl;
    return false;
  }
  if (Qk.size() != Tk_out.size()) {
    std::cout << "number of specifications for Tk_out and Qk are different"
              << std::endl;
    return false;
  }
  if (Tk_in.size() != Tk_out.size()) {
    std::cout << "number of specifications for Tk_in and Tk_out are different"
              << std::endl;
    return false;
  }
  for (std::size_t nk = 0; nk < Tk_in.size(); nk++) {
    if (Tk_in[nk] > Tk_out[nk]) {
      std::cout << "return temperature of heat demand no. " << nk
                << " is higher than supply temperature" << std::endl;
      return false;
    }
  }

  // Check that there is sufficient heat available from Rankine cycle
  double sum_Qk = 0.0;
  for (std::size_t nk = 0; nk < Qk.size(); nk++) {
    sum_Qk = sum_Qk + Qk[nk];
  }
  if (sum_Qk > 0.5 * (W_el / 0.2)) {
    std::cout << "there is not sufficient heat available from Rankine cycle to "
                 "supply the "
                 "specifiy heat demand"
              << std::endl;
    std::cout << "Reducing proportionally the heat demands" << std::endl;
    for (std::size_t nk = 0; nk < Qk.size(); nk++) {
      Qk[nk] = Qk[nk] * (0.5 * (W_el / 0.2)) / sum_Qk;
    }
  }

  object bioCHP("plant", "bioCHP_PLANT",
                get_database_path("bioCHP_inputs.toml"));
  bioCHP.vct_sp("fuel_def", fuel_def);
  bioCHP.vct_fp("Yj", Yj);
  bioCHP.vct_fp("YH2Oj", YH2Oj);
  bioCHP.fval_p("W_el", W_el);
  bioCHP.vct_fp("Qk", Qk);
  bioCHP.vct_fp("Tk_in", Tk_in);
  bioCHP.vct_fp("Tk_out", Tk_out);

  bioCHP_plant_model(bioCHP);

  Mj = bioCHP.vctp("Mj");
  Q_prod = bioCHP.fp("Heat_production_(MW)");
  W_el_prod = bioCHP.fp("Electricity_production_(MW)");
  C_inv = bioCHP.fp("C_inv") * 1e-6;
  C_inv = bioCHP.fp("C_inv") * 1e-6;
  C_op = bioCHP.fp("C_op") * 1e-6;
  C_op_var = bioCHP.fp("C_op_var") * 1e-6;

  export_output_parameters(bioCHP, "Output-bioCHP_project");

  return true;
}
