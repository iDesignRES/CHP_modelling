#include <cstring>
#include <string>
#include <vector>

#include "bioCHP.cpp"

extern "C" {
bool bioCHP_plant_c(const char** fuel_def, int fuel_count, const double* Yj, int Yj_len,
                    const double* YH2Oj, int YH2Oj_len, double W_el, const double* Qk,
                    int Qk_len, const double* Tk_in, int Tk_in_len, const double* Tk_out,
                    int Tk_out_len, double* Mj, int Mj_len, double* Q_prod,
                    double* W_el_prod, double* C_inv, double* C_op, double* C_op_var) {
  std::vector<std::string> fuel_vec;
  for (int i = 0; i < fuel_count; ++i) {
    fuel_vec.emplace_back(fuel_def[i]);
  }

  std::vector<double> Yj_vec(Yj, Yj + Yj_len);
  std::vector<double> YH2Oj_vec(YH2Oj, YH2Oj + YH2Oj_len);
  std::vector<double> Qk_vec(Qk, Qk + Qk_len);
  std::vector<double> Tk_in_vec(Tk_in, Tk_in + Tk_in_len);
  std::vector<double> Tk_out_vec(Tk_out, Tk_out + Tk_out_len);
  std::vector<double> Mj_vec(Mj_len);

  bool result =
      bioCHP_plant(fuel_vec, Yj_vec, YH2Oj_vec, W_el, Qk_vec, Tk_in_vec, Tk_out_vec, Mj_vec,
                   *Q_prod, *W_el_prod, *C_inv, *C_op, *C_op_var);

  for (int i = 0; i < Mj_len; ++i) {
    Mj[i] = Mj_vec[i];
  }

  return result;
}
}
