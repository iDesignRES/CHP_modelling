#include "bioCHP_wrapper.h"
#include <cstring>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "bioCHP.h"

extern "C" {
/**
 * @brief wrapper `C` function for the bioCHP model
 * @param fuel_def (input) as biomass composition in the input soild fuel to the
 * CHP plant
 * @param fuel_count (input) denoting the number of biomass feedstock in
 * `fuel_def`
 * @param Yj(input) as the mass fraction of biomass resource \f$ j \f$ in the
 * feed.
 * @param Yj_len (input) denoting the size of `Yj`
 * @param YH2Oj(input) as the moisture content of biomass resource \f$ j \f$ as
 * a mass fraction.
 * @param YH2Oj_len (input) denoting the size of `YH2Oj`
 * @param Tk_in (input) as the return temperature level of heat demand \f$ k \f$
 * in °C.
 * @param Tk_in_len (input) denoting the size of `Tk_in`
 * @param Tk_out (input) as the supply temperature level of heat demand \f$ k
 * \f$ in °C.
 * @param Tk_out_len (input) denoting the size of `Tk_out`
 * @param Qk (input) as the heat demand at the individual temperature levels \f$
 * k \f$ in MW.
 * @param Qk_len (input) denoting the size of `Qk`
 * @param W_el (input) as the electric power output of the CHP plant in MW.
 * @param Mj (output) as the input mass flow rate of each biomass feedstock to
 * the bioCHP plant in kg/s.
 * @param Mj_len (output) denoting the size of `Mj`
 * @param Q_prod (output) as the total heat production from the bioCHP plant in
 * MW.
 * @param W_el_prod (output) as the total electric power production from the
 * bioCHP plant in MW.
 * @param C_inv (output) as represents the capital expenditures in M\$.
 * @param C_op_d (output) as the annual direct variable operating expenses in
 * M\$.
 * @param C_op_f (output) as the annual fixed operating expenses in M\$.
 */
int bioCHP_plant_c(const char** fuel_def, int fuel_count, const double* Yj,
                   int Yj_len, const double* YH2Oj, int YH2Oj_len, double W_el,
                   const double* Qk, int Qk_len, const double* Tk_in,
                   int Tk_in_len, const double* Tk_out, int Tk_out_len,
                   double* Mj, int Mj_len, double* Q_prod, double* W_el_prod,
                   double* C_inv, double* C_op, double* C_op_var,
                   char** err_out) {
  if (err_out) *err_out = nullptr;
  try {
    std::vector<std::string> fuel_vec;
    for (int i = 0; i < fuel_count; ++i) fuel_vec.emplace_back(fuel_def[i]);

    std::vector<double> Yj_vec(Yj, Yj + Yj_len);
    std::vector<double> YH2Oj_vec(YH2Oj, YH2Oj + YH2Oj_len);
    std::vector<double> Qk_vec(Qk, Qk + Qk_len);
    std::vector<double> Tk_in_vec(Tk_in, Tk_in + Tk_in_len);
    std::vector<double> Tk_out_vec(Tk_out, Tk_out + Tk_out_len);

    object bioCHP = bioCHP_plant(fuel_vec, Yj_vec, YH2Oj_vec, W_el, Qk_vec,
                                 Tk_in_vec, Tk_out_vec);

    std::vector<double> Mj_vec(Mj_len);
    extract_outputs(bioCHP, Q_prod, W_el_prod, C_inv, C_op, C_op_var, Mj_vec);
    for (int i = 0; i < Mj_len; ++i) Mj[i] = Mj_vec[i];
    return 0;
  } catch (const std::exception& e) {
    if (err_out) {
      const char* msg = e.what();
      if (!msg) msg = "unknown std::exception";
      *err_out = strdup(msg);
    }
    return 1;  // failure
  } catch (...) {
    if (err_out) *err_out = strdup("non-std exception");
    return 2;  // other failure
  }
}
}
