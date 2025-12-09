/**
 * @file bioCHP_wrapper.h
 * @brief C wrapper API for the bioCHP model.
 *
 * Provides the `bioCHP_plant_c` function that wraps the C++ bioCHP model
 * for use from C and other languages.
 */
#pragma once

extern "C" {
/**
 * @brief wrapper `C` function for the bioCHP model
 * @param[in] fuel_def as biomass composition in the input solid fuel to the
 * CHP plant
 * @param[in] fuel_count denoting the number of biomass feedstock in
 * `fuel_def`
 * @param[in] Yj as the mass fraction of biomass resource \f$ j \f$ in the
 * feed.
 * @param[in] Yj_len denoting the size of `Yj`
 * @param[in] YH2Oj as the moisture content of biomass resource \f$ j \f$ as
 * a mass fraction.
 * @param[in] YH2Oj_len denoting the size of `YH2Oj`
 * @param[in] Tk_in as the return temperature level of heat demand \f$ k \f$
 * in °C.
 * @param[in] Tk_in_len denoting the size of `Tk_in`
 * @param[in] Tk_out as the supply temperature level of heat demand \f$ k
 * \f$ in °C.
 * @param[in] Tk_out_len denoting the size of `Tk_out`
 * @param[in] Qk as the heat demand at the individual temperature levels \f$
 * k \f$ in MW.
 * @param[in] Qk_len denoting the size of `Qk`
 * @param[in] W_el as the electric power output of the CHP plant in MW.
 * @param[out] Mj as the input mass flow rate of each biomass feedstock to
 * the bioCHP plant in kg/s.
 * @param[in] Mj_len denoting the size of `Mj`
 * @param[out] Q_prod as the total heat production from the bioCHP plant in
 * MW.
 * @param[out] W_el_prod as the total electric power production from the
 * bioCHP plant in MW.
 * @param[out] C_inv as represents the capital expenditures in M\$.
 * @param[out] C_op as the total operating cost in M\$.
 * @param[out] C_op_var as the annual variable operating expenses in M\$.
 * @param[out] err_out pointer to error message string. If an error occurs,
 * this will be set to a dynamically allocated string containing the error
 * message. The caller is responsible for freeing this memory using `free()`.
 * If no error occurs, this will be set to `nullptr`.
 *
 * @retval 0 Success.
 * @retval 1 std::exception was thrown.
 * @retval 2 Any other exception.
 *
 * @note The caller must free the memory allocated for `err_out` using `free()`
 * when an error occurs (non-zero return value) to prevent memory leaks.
 */
int bioCHP_plant_c(const char** fuel_def, int fuel_count, const double* Yj,
                   int Yj_len, const double* YH2Oj, int YH2Oj_len, double W_el,
                   const double* Qk, int Qk_len, const double* Tk_in,
                   int Tk_in_len, const double* Tk_out, int Tk_out_len,
                   double* Mj, int Mj_len, double* Q_prod, double* W_el_prod,
                   double* C_inv, double* C_op, double* C_op_var,
                   char** err_out);
}
