#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include "../src/Libraries/Processes_library/bioCHP_plant.h"
#include "../src/utils.h"
#include "test_utils.h"

int main() {
  std::cout << "Mannual test of bioCHP model with input biomass energy "
            << std::endl;

  object par("plant", "bioCHP_PLANT", get_database_path("bioCHP_inputs.toml"));
  std::vector<std::string> fuel_def = {"spruce_stem", "birch_stem",
                                       "spruce_TandB", "spruce_bark"};
  std::vector<double> Yj = {0.1, 0.3, 0.4, 0.2};
  std::vector<double> YH2Oj = {0.4, 0.35, 0.45, 0.5};
  std::vector<double> Qk = {30.0, 40.0};
  std::vector<double> Tk_in = {70.0, 50.0};
  std::vector<double> Tk_out = {90.0, 80.0};
  double Hf = 409.308;

  par.vct_sp("fuel_def", fuel_def);
  par.vct_fp("Yj", Yj);
  par.vct_fp("YH2Oj", YH2Oj);
  par.vct_fp("Qk", Qk);
  par.vct_fp("Tk_in", Tk_in);
  par.vct_fp("Tk_out", Tk_out);
  par.fval_p("Hf", Hf);

  bioCHP_plant_model(par);

  double Q_prod = par.fp("Heat_production_(MW)");
  double W_el_prod = par.fp("Electricity_production_(MW)");
  double C_inv = par.fp("C_inv") * 1e-6;
  double C_op = par.fp("C_op") * 1e-6;
  double C_op_var = par.fp("C_op_var") * 1e-6;

  [[maybe_unused]] std::vector<double> expected_Mj = {
      4.395472220721, 13.186416662165, 17.581888882886, 8.790944441442};
  [[maybe_unused]] double expected_Q_prod = 70.0;
  [[maybe_unused]] double expected_W_el_prod = 100.000000004445;
  [[maybe_unused]] double expected_C_inv = 317.941917708361;
  [[maybe_unused]] double expected_C_op = 76.500779705097;
  [[maybe_unused]] double expected_C_op_var = 69.745633830321;

  for (std::size_t nj = 0; nj < par.vctp("Mj").size(); nj++)
    assert(approxEqual(par.vctp("Mj")[nj], expected_Mj[nj]) ||
           mismatch("Mj[" + std::to_string(nj) + "]", par.vctp("Mj")[nj]));

  assert(approxEqual(Q_prod, expected_Q_prod) || mismatch("Q_prod", Q_prod));
  assert(approxEqual(W_el_prod, expected_W_el_prod) ||
         mismatch("W_el_prod", W_el_prod));
  assert(approxEqual(C_inv, expected_C_inv) || mismatch("C_inv", C_inv));
  assert(approxEqual(C_op, expected_C_op) || mismatch("C_op", C_op));
  assert(approxEqual(C_op_var, expected_C_op_var) ||
         mismatch("C_op_var", C_op_var));
  return 0;
}
