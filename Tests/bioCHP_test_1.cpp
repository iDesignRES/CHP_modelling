#include <cstddef>
#include <iomanip>  // for setprecision
#include <iostream>
#include <string>
#include <vector>

#include "../src/bioCHP.h"

bool approxEqual(double a, double b, double tol = 1e-4) {
  return std::abs(a - b) < tol;
}

int main() {
  std::vector<std::string> fuel_def = {"spruce_chips"};
  std::vector<double> Yj = {1.0};
  std::vector<double> YH2Oj = {0.3};
  double W_el = 300.0;
  std::vector<double> Qk = {};
  std::vector<double> Tk_in = {};
  std::vector<double> Tk_out = {};

  std::vector<double> Mj, expected_Mj;
  expected_Mj.push_back(60.3722);
  double Q_prod, expected_Q_prod = 0.000000;
  double W_el_prod, expected_W_el_prod = 300.000000;
  double C_inv, expected_C_inv = 443.211296;
  double C_op, expected_C_op = 112.166124;
  double C_op_var, expected_C_op_var = 102.749445;

  if (bioCHP_plant(fuel_def, Yj, YH2Oj, W_el, Qk, Tk_in, Tk_out, Mj, Q_prod,
                   W_el_prod, C_inv, C_op, C_op_var)) {
    bool all_passed = true;

    std::cout << std::fixed << std::setprecision(6);  // Set higher precision

    for (std::size_t nj = 0; nj < Mj.size(); nj++) {
      std::cout << "M_" + fuel_def[nj] + " = " << Mj[nj] << std::endl;
    }
    std::cout << std::fixed << std::setprecision(6);  // Set higher precision
    std::cout << "Q_prod (MW) = " << Q_prod << std::endl;
    std::cout << "W_el_prod (MW) = " << W_el_prod << std::endl;
    std::cout << "C_inv (M$) = " << C_inv << std::endl;
    std::cout << "C_op (M$) = " << C_op << std::endl;
    std::cout << "C_op_var (M$) = " << C_op_var << std::endl;

    for (std::size_t nj = 0; nj < Mj.size(); nj++) {
      std::cout << "M_" << fuel_def[nj] << " = " << Mj[nj] << std::endl;
      if (!approxEqual(Mj[nj], expected_Mj[nj])) {
        all_passed = false;
        std::cout << "Mj[" << nj << "] failed: expected " << expected_Mj[nj]
                  << ", got " << Mj[nj] << std::endl;
      }
    }

    if (!approxEqual(Q_prod, expected_Q_prod)) {
      all_passed = false;
      std::cout << "Q_prod failed: expected " << expected_Q_prod << ", got "
                << Q_prod << std::endl;
    }

    if (!approxEqual(W_el_prod, expected_W_el_prod)) {
      all_passed = false;
      std::cout << "W_el_prod failed: expected " << expected_W_el_prod
                << ", got " << W_el_prod << std::endl;
    }

    if (!approxEqual(C_inv, expected_C_inv)) {
      all_passed = false;
      std::cout << "C_inv failed: expected " << expected_C_inv << ", got "
                << C_inv << std::endl;
    }

    if (!approxEqual(C_op, expected_C_op)) {
      all_passed = false;
      std::cout << "C_op failed: expected " << expected_C_op << ", got " << C_op
                << std::endl;
    }

    if (!approxEqual(C_op_var, expected_C_op_var)) {
      all_passed = false;
      std::cout << "C_op_var failed: expected " << expected_C_op_var << ", got "
                << C_op_var << std::endl;
    }

    if (all_passed) {
      std::cout << "Test pass!" << std::endl;
    } else {
      std::cout << "Test failed!" << std::endl;
    }

    return all_passed ? 0 : 1;
  }
    else {
      std::cout << "Running bioCHP_plant_test_1 failed on execution!" << std::endl;
      return 1;
    }
  return 0;
}
