#include <cstddef>
#include <iomanip>  // for setprecision
#include <iostream>
#include <string>
#include <vector>

#include "../src/bioCHP.h"

int main() {
  std::vector<std::string> fuel_def = {"spruce_chips", "spruce_T&B"};
  std::vector<double> Yj = {0.5, 0.5};
  std::vector<double> YH2Oj = {0.3, 0.6};
  double W_el = 300.0;
  std::vector<double> Qk = {};
  std::vector<double> Tk_in = {};
  std::vector<double> Tk_out = {};

  std::vector<double> Mj;
  double Q_prod;
  double W_el_prod;
  double C_inv;
  double C_op;
  double C_op_var;

  if (bioCHP_plant(fuel_def, Yj, YH2Oj, W_el, Qk, Tk_in, Tk_out, Mj, Q_prod,
                   W_el_prod, C_inv, C_op, C_op_var)) {
    for (std::size_t nj = 0; nj < Mj.size(); nj++) {
      std::cout << "M_" + fuel_def[nj] + " = " << Mj[nj] << std::endl;
    }
    std::cout << std::fixed << std::setprecision(6);  // Set higher precision
    std::cout << "Q_prod (MW) = " << Q_prod << std::endl;
    std::cout << "W_el_prod (MW) = " << W_el_prod << std::endl;
    std::cout << "C_inv (M$) = " << C_inv << std::endl;
    std::cout << "C_op (M$) = " << C_op << std::endl;
    std::cout << "C_op_var (M$) = " << C_op_var << std::endl;

  } else {
    std::cout << "error" << std::endl;
  }

  return 0;
}
