#include <cmath>    // for fabs
#include <iomanip>  // for setprecision
#include <iostream>
#include <string>
#include <vector>

#include "../src/bioCHP_wrapper.h"

bool approxEqual(double a, double b, double tol = 1e-4) {
  return std::fabs(a - b) < tol;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <test_number>" << std::endl;
    return 1;
  }

  int test = std::stoi(argv[1]);

  const char *fuel_def[4];
  int fuel_count, Yj_len, YH2Oj_len, Qk_len, Tk_in_len, Tk_out_len, Mj_len;
  double Yj[4], YH2Oj[4], Qk[2], Tk_in[2], Tk_out[2], Mj[4];
  double W_el, Q_prod, W_el_prod, C_inv, C_op, C_op_var;
  double expected_Mj[4], expected_Q_prod, expected_W_el_prod, expected_C_inv,
      expected_C_op, expected_C_op_var;

  switch (test) {
    case 1:
      fuel_def[0] = "spruce_stem";
      fuel_def[1] = "spruce_bark";
      fuel_def[2] = "birch_stem";
      fuel_def[3] = "spruce_TandB";
      fuel_count = 4;
      Yj[0] = 0.1;
      Yj[1] = 0.2;
      Yj[2] = 0.3;
      Yj[3] = 0.4;
      Yj_len = 4;
      YH2Oj[0] = 0.4;
      YH2Oj[1] = 0.5;
      YH2Oj[2] = 0.35;
      YH2Oj[3] = 0.45;
      YH2Oj_len = 4;
      W_el = 100.0;
      Qk[0] = 30.0;
      Qk[1] = 40.0;
      Qk_len = 2;
      Tk_in[0] = 70.0;
      Tk_in[1] = 50.0;
      Tk_in_len = 2;
      Tk_out[0] = 90.0;
      Tk_out[1] = 80.0;
      Tk_out_len = 2;

      Mj_len = 4;
      expected_Mj[0] = 2.55886;
      expected_Mj[1] = 5.11772;
      expected_Mj[2] = 7.67659;
      expected_Mj[3] = 10.2354;
      expected_Q_prod = 70.0;
      expected_W_el_prod = 100.0;
      expected_C_inv = 243.176757;
      expected_C_op = 45.768077;
      expected_C_op_var = 40.601427;
      break;
    case 2:
      fuel_def[0] = "spruce_stem";
      fuel_def[1] = "birch_stem";
      fuel_def[2] = "spruce_TandB";
      fuel_def[3] = "spruce_bark";
      fuel_count = 4;
      Yj[0] = 0.1;
      Yj[1] = 0.3;
      Yj[2] = 0.4;
      Yj[3] = 0.2;
      Yj_len = 4;
      YH2Oj[0] = 0.4;
      YH2Oj[1] = 0.35;
      YH2Oj[2] = 0.45;
      YH2Oj[3] = 0.5;
      YH2Oj_len = 4;
      W_el = 100.0;
      Qk[0] = 30.0;
      Qk[1] = 40.0;
      Qk_len = 2;
      Tk_in[0] = 70.0;
      Tk_in[1] = 50.0;
      Tk_in_len = 2;
      Tk_out[0] = 90.0;
      Tk_out[1] = 80.0;
      Tk_out_len = 2;

      Mj_len = 4;
      expected_Mj[0] = 2.55886;
      expected_Mj[1] = 7.67659;
      expected_Mj[2] = 10.2354;
      expected_Mj[3] = 5.11772;
      expected_Q_prod = 70.0;
      expected_W_el_prod = 100.0;
      expected_C_inv = 243.176757;
      expected_C_op = 45.768077;
      expected_C_op_var = 40.601427;
      break;
  }

  if (bioCHP_plant_c(fuel_def, fuel_count, Yj, Yj_len, YH2Oj, YH2Oj_len, W_el,
                     Qk, Qk_len, Tk_in, Tk_in_len, Tk_out, Tk_out_len, Mj,
                     Mj_len, &Q_prod, &W_el_prod, &C_inv, &C_op, &C_op_var)) {
    bool all_passed = true;

    for (int nj = 0; nj < Mj_len; nj++) {
      std::cout << "M_" << fuel_def[nj] << " = " << Mj[nj] << std::endl;
      if (!approxEqual(Mj[nj], expected_Mj[nj])) {
        all_passed = false;
        std::cout << "Mj[" << nj << "] failed: expected " << expected_Mj[nj]
                  << ", got " << Mj[nj] << std::endl;
      }
    }

    std::cout << std::fixed << std::setprecision(6);  // Set higher precision
    std::cout << "Q_prod (MW) = " << Q_prod << std::endl;
    std::cout << "W_el_prod (MW) = " << W_el_prod << std::endl;
    std::cout << "C_inv (M$) = " << C_inv << std::endl;
    std::cout << "C_op (M$) = " << C_op << std::endl;
    std::cout << "C_op_var (M$) = " << C_op_var << std::endl;

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
      std::cout << "All tests pass!" << std::endl;
    } else {
      std::cout << "Some tests failed!" << std::endl;
    }

    return all_passed ? 0 : 1;
  } else {
    std::cout << "Running bioCHP_plant_c failed on execution!" << std::endl;
    return 1;
  }

  return 0;
}
