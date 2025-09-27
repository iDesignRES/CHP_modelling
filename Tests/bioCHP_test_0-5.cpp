#include <cstddef>
#include <iomanip>  // for setprecision
#include <iostream>
#include <string>
#include <vector>

#include "../src/bioCHP.h"

int main() {
  std::vector<std::string> fuel_def = {"spruce_chips", "spruce_TandB"};
  std::vector<double> Yj = {0.5, 0.5};
  std::vector<double> YH2Oj = {0.3, 0.6};
  double W_el = 300.0000;
  std::vector<double> Qk = {30.0, 40.0};
  std::vector<double> Tk_in = {70.0, 70.0};
  std::vector<double> Tk_out = {90.0, 90.0, 90.0};

  std::vector<double> Mj;
  double Q_prod;
  double W_el_prod;
  double C_inv;
  double C_op;
  double C_op_var;

  if (bioCHP_plant(fuel_def, Yj, YH2Oj, W_el, Qk, Tk_in, Tk_out, Mj, Q_prod,
                   W_el_prod, C_inv, C_op, C_op_var)) {
    std::cout << "No errors" << std::endl;

  } else {
    std::cout << "error" << std::endl;
  }

  return 0;
}
