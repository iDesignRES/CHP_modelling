#include <cassert>
#include <iostream>
#include <vector>

#include "../src/bioCHP_wrapper.h"
#include "test_utils.h"

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
  [[maybe_unused]] double expected_Mj[4];
  [[maybe_unused]] double expected_Q_prod = 70.0;
  [[maybe_unused]] double expected_W_el_prod = 100.000000004445;
  [[maybe_unused]] double expected_C_inv = 224.750123723543;
  [[maybe_unused]] double expected_C_op = 39.098300692570;
  [[maybe_unused]] double expected_C_op_var = 34.323152193531;

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
      expected_Mj[0] = 2.163095432773;
      expected_Mj[1] = 4.326190865546;
      expected_Mj[2] = 6.489286298320;
      expected_Mj[3] = 8.652381731093;
      break;
    case 2:
      fuel_def[0] = "spruce_bark";
      fuel_def[1] = "spruce_TandB";
      fuel_def[2] = "spruce_stem";
      fuel_def[3] = "birch_stem";
      fuel_count = 4;
      Yj[0] = 0.2;
      Yj[1] = 0.4;
      Yj[2] = 0.1;
      Yj[3] = 0.3;
      Yj_len = 4;
      YH2Oj[0] = 0.5;
      YH2Oj[1] = 0.45;
      YH2Oj[2] = 0.4;
      YH2Oj[3] = 0.35;
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
      expected_Mj[0] = 4.326190865546;
      expected_Mj[1] = 8.652381731093;
      expected_Mj[2] = 2.163095432773;
      expected_Mj[3] = 6.489286298320;
      break;
    default:
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
      expected_Mj[0] = 2.163095432773;
      expected_Mj[1] = 6.489286298320;
      expected_Mj[2] = 8.652381731093;
      expected_Mj[3] = 4.326190865546;
      break;
  }

  bioCHP_plant_c(fuel_def, fuel_count, Yj, Yj_len, YH2Oj, YH2Oj_len, W_el, Qk,
                 Qk_len, Tk_in, Tk_in_len, Tk_out, Tk_out_len, Mj, Mj_len,
                 &Q_prod, &W_el_prod, &C_inv, &C_op, &C_op_var);

  for (int nj = 0; nj < Mj_len; nj++)
    assert(approxEqual(Mj[nj], expected_Mj[nj]) ||
           mismatch("Mj[" + std::to_string(nj) + "]", Mj[nj]));

  assert(approxEqual(Q_prod, expected_Q_prod) || mismatch("Q_prod", Q_prod));
  assert(approxEqual(W_el_prod, expected_W_el_prod) ||
         mismatch("W_el_prod", W_el_prod));
  assert(approxEqual(C_inv, expected_C_inv) || mismatch("C_inv", C_inv));
  assert(approxEqual(C_op, expected_C_op) || mismatch("C_op", C_op));
  assert(approxEqual(C_op_var, expected_C_op_var) ||
         mismatch("C_op_var", C_op_var));

  return 0;
}
