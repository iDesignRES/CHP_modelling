#pragma once

extern "C" {
bool bioCHP_plant_c(const char** fuel_def, int fuel_count, const double* Yj,
          int Yj_len, const double* YH2Oj, int YH2Oj_len, double W_el,
          const double* Qk, int Qk_len, const double* Tk_in,
          int Tk_in_len, const double* Tk_out, int Tk_out_len,
          double* Mj, int Mj_len, double* Q_prod, double* W_el_prod,
          double* C_inv, double* C_op, double* C_op_var);
}
