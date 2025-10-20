#include "Rankine_cycle.h"

#include <cstddef>
#include <iostream>

#include "../../Cost.h"
#include "../Thermodynamic_library/Library_Water_Correlations.h"

/**
 * @brief function to create steam turbine parameters
 *
 * @param sys_type = type of system
 * @param sys_def = Name of system
 * @param &par = vector of parameters
 */
void steam_turbine_parameters::assign_parameter_values(std::string sys_type,
                                                       std::string sys_def,
                                                       object par) {
  Po = get_num_parameter(par.p, sys_type, sys_def, "Po");
  mu_isent = get_num_parameter(par.p, sys_type, sys_def, "mu_isent");
}

void steam_turbine(flow &in, flow &out, steam_turbine_parameters &ST) {
  /* Calculation of single-stage steam turbine model, based on:
    - Input steam flow, in
    - Isentropic efficiency, ST.mu_isent
    - Outlet pressure, ST.Po

    Assumptions:
    - Mechanical to electric conversion effieiency, eff_el = 0.9
  */

  ST.Mi = in.F.M;
  ST.Ti = in.F.T;
  ST.Pi = in.F.P;
  in.calculate_flow_properties();

  const double eff_el = 0.9;

  /* Calculation of inlet thermodynamic propoperties */
  double s_out, s_in = sPTSupSteam(ST.Pi, ST.Ti);
  double hs_out, h_out, h_calc, h_in = hPTSupSteam(ST.Pi, ST.Ti);

  /* Initial guess for specific entropy and steam quality at outlet */
  double T_out, Ts_out = TSatWater(ST.Po);
  double ys_moisture = 0.0, y_moisture;
  double s_out_initial = sPTSupSteam(ST.Po, Ts_out);

  /* Iterate with isentropic outlet temperature and steam quality
  until reaching isentropic outlet conditions  */
  if (s_out_initial < s_in) {
    s_out = s_out_initial;
    while (s_out < s_in) {
      Ts_out += 0.01;
      s_out = sPTSupSteam(ST.Po, Ts_out);
    }
    hs_out = hPTSupSteam(ST.Po, Ts_out);
    h_out = h_in + ST.mu_isent * (hPTSupSteam(ST.Po, Ts_out) - h_in);
  } else {
    s_out = s_out_initial;
    while (s_out > s_in) {
      ys_moisture += 0.001;
      s_out = ys_moisture * sPWater(ST.Po) +
              (1.0 - ys_moisture) * sPSatSteam(ST.Po);
    }
    hs_out =
        ys_moisture * hPWater(ST.Po) + (1.0 - ys_moisture) * hPSatSteam(ST.Po);
    h_out = h_in + ST.mu_isent * (hs_out - h_in);
  }

  if (h_out > hPSatSteam(ST.Po)) {
    T_out = Ts_out;
    h_calc = hPSatSteam(ST.Po);
    while (h_calc < h_out) {
      T_out += 0.001;
      h_calc = hPTSupSteam(ST.Po, T_out);
    }
  } else {
    y_moisture = ys_moisture;
    T_out = Ts_out;
    h_calc = hs_out;
    while (h_calc < h_out) {
      y_moisture = y_moisture - 0.001;
      h_calc =
          y_moisture * hPWater(ST.Po) + (1.0 - y_moisture) * hPSatSteam(ST.Po);
    }
  }

  /* Updating inlet and outlet flow parameters */
  in.F.Ht = in.F.M * 1.0e3 * h_in;

  out.F.M = in.F.M;
  out.F.T = T_out;
  out.F.P = ST.Po;
  out.P.h = h_calc;
  out.P.ht = out.P.h;
  out.F.Ht = out.F.M * 1.0e3 * h_calc;

  /* Calculation of power output in Watts */
  ST.W = eff_el * in.F.M * 1.0e3 * (h_in - h_calc);
}

void steam_turbine_model(flow &in, flow &out, object &par) {
  /* Calculation of multi-stage steam turbine model
    with multiple steam extractions (bleeds) specified by
    double vectors P_bleed (pressure, bar-g) and
    M_bleed (mass flow rate, kg/s)
    Assumptions:
    1. Isentropic efficiency constant and equal for all stages
    */

  /* Importing steam_turbine model parameters */
  steam_turbine_parameters ST;
  ST.assign_parameter_values("process", "Rankine_cycle", par);

  /* Initialize the total power output to zero*/
  double W = 0;

  /* Creating intern flow representing
  the outlet from each steam turbine stage */
  flow out_n("water");

  /* Calculate each steam turbine stage */
  std::size_t N_bleed = par.vctp("P_bleed").size();
  if (N_bleed == 0) {
    steam_turbine(in, out, ST);
    par.fval_p("W_el", 1e-6 * ST.W);
  }

  if (N_bleed > 0) {
    steam_turbine_parameters ST_n;
    for (int n = 0; n < N_bleed + 1; n++) {
      if (n == 0) {
        ST_n.Ti = in.F.T;
        ST_n.Pi = in.F.P;
        ST_n.Po = par.vctp("P_bleed")[n];
        ST_n.mu_isent = ST.mu_isent;
        steam_turbine(in, out_n, ST_n);
        out_n.F.M = out_n.F.M - par.vctp("M_bleed")[n];
      }

      if (N_bleed > 1 && n > 0 && n < N_bleed) {
        ST_n.Ti = out_n.F.T;
        ST_n.Pi = out_n.F.P;
        ST_n.Po = par.vctp("P_bleed")[n];
        ST_n.mu_isent = ST.mu_isent;
        steam_turbine(out_n, out_n, ST_n);
        out_n.F.M = out_n.F.M - par.vctp("M_bleed")[n];
      }

      if (n == N_bleed) {
        ST_n.Ti = out_n.F.T;
        ST_n.Pi = out_n.F.P;
        ST_n.Po = ST.Po;
        ST_n.mu_isent = ST.mu_isent;
        steam_turbine(out_n, out, ST_n);
      }
      W = W + ST_n.W;  // Add power production from each stage
    }

    /* Exporting total power output in MW*/
    par.fval_p("W_el", 1e-6 * W);
  }
}

void steam_condenser(flow &steam, flow &cond, object &par) {
  /* Calculation of steam condenser model
    specified by outlet pressure P_cond (bar-g) and
    temperature T_cond (deg. C) conditions
    Assumed isobaric
  */

  cond = flow("cond", "water");
  cond.F.T = par.fp("T_cond");
  cond.F.P = par.fp("P_cond");
  cond.F.M = steam.F.M;

  steam.P.h = hPTSupSteam(steam.F.P, steam.F.T);
  cond.P.h = hTWater(cond.F.T);
  double Q_cond = steam.F.M * (steam.P.h - cond.P.h);

  /* Exporting total thermal power output in MW*/
  par.fval_p("Q_cond", Q_cond * 1e-6);
}

void district_heating(object &par) {
  /* Calculation steam extractions (bleeds)
  required from steam turbine to meet heat demands
  for distric heating
    as
    Inputs:
    - Qk = Thermal capacity of each heat demand (MW)
    - Tk_in = Return temperature (deg.C) of each heat demand (MW)
    - Tk_out = Supply temperature (deg.C) of each heat demand (MW)
    Outputs:
    - Pb vector with steam extractions pressure (, )bar-g)
    - Mb vector with steam extractions mass flow rate (kg/s)
    Assumptions:
    - All heat demands are assumed to be at atmospheric pressure
  */

  /* Calculation of bleeds for each heat demand:
     - Pressure as saturated pressure at Tk_out + 25.0 deg. C
     - Mass flow assuming complete steam condensation at saturation point
     to equal Qk  */
  std::vector<double> P_bleed, M_bleed;

  /* Create internal flows repreenting return and supply water to district
   * heating for each Qk */
  flow dh_in("dh_in", "water");
  flow dh_out("dh_out", "water");

  if (par.vctp("Qk").size() > 0) {
    for (std::size_t nk = 0; nk < par.vctp("Qk").size(); nk++) {
      /* Calculate return and supply district heating flows for each Qk */
      dh_in.F.T = par.vctp("Tk_in")[nk];
      dh_in.F.P = 1.01325;  // bar, atmospheric pressure
      dh_in.P.h = hTWater(dh_in.F.T);
      dh_out.F.T = par.vctp("Tk_out")[nk];
      dh_out.F.P = 1.01325;  // bar, atmospheric pressure
      dh_out.P.h = hTWater(dh_out.F.T);
      dh_in.F.M = par.vctp("Qk")[nk] / (dh_out.P.h - dh_in.P.h);
      dh_out.F.M = dh_in.F.M;

      /* Create flows representing inlet and outlet heating fluids (steam)
      in a heat exchanger for exporting heat Qk for district heating */
      flow hf_in, hf_out;
      hf_in = flow("hf_in", "water");
      hf_out = flow("hf_out", "water");
      hf_in.F.T = dh_out.F.T + 25.0;
      hf_in.F.P = PSatWater(hf_in.F.T);
      double Tsat_hf_in = TSatWater(hf_in.F.P);
      hf_in.P.h = hPSatSteam(hf_in.F.P);
      hf_out.F.T = Tsat_hf_in - 5.0;
      hf_out.F.P = hf_in.F.P;
      hf_out.P.h = hTWater(hf_out.F.T);
      hf_in.F.M = 1e3 * par.vctp("Qk")[nk] / (hf_in.P.h - hf_out.P.h);
      hf_out.F.M = hf_in.F.M;

      /* Export bleed parameters for each Qk */
      P_bleed.push_back(hf_in.F.P);
      M_bleed.push_back(hf_in.F.M);
    }

    /* Sort bleed parameters in pressure descendent order */
    std::vector<double> Pb_ord = P_bleed, Mb_ord = M_bleed;

    for (std::size_t i = 0; i < Pb_ord.size() - 1; ++i) {
      for (std::size_t j = 0; j < Pb_ord.size() - i - 1; ++j) {
        if (Pb_ord[j] < Pb_ord[j + 1]) {
          std::swap(Pb_ord[j], Pb_ord[j + 1]);
          std::swap(Mb_ord[j], Mb_ord[j + 1]);
        }
      }
    }

    /* Merge bleeds with pressure difference lower than 5 bar-g */
    std::vector<double> Pb = Pb_ord, Mb = Mb_ord;
    std::vector<std::size_t> merged;
    if (Pb_ord.size() > 1) {
      double Pmax = Pb_ord[0];
      for (std::size_t nb = 1; nb < Pb_ord.size(); nb++) {
        if ((Pmax - Pb_ord[nb]) < 5.0) {
          Mb[nb - 1] += Mb[nb];
          Pb.erase(Pb.begin() + nb);
          Mb.erase(Mb.begin() + nb);
        } else if ((Pmax - Pb_ord[nb]) > 5.0) {
          Pmax = Pb_ord[nb];
        }
      }
    }

    /* Expert sorted and merged bleeds as steam extractions */
    par.vct_fp("P_bleed", Pb);
    par.vct_fp("M_bleed", Mb);
  }
}

void rankine_cycle(object &par) {
  /*Rankine cycle model, described in docs/bioCHP_model_description.md,
  including:
   1. Multi-stage steam turbine
   2. Condenser
   3. Heat export to district heating
  */

  flow bfw, sat_cond, sat_stm, steam, steam_out, cond;

  /* Defining and calculating boiler feed water
  Assumed at 105 deg. C and boiler pressure P_stm */
  bfw = flow("bfw", "water");
  bfw.F.T = 105.0;
  bfw.F.P = par.fp("P_stm");
  bfw.P.h = hTWater(bfw.F.T);
  bfw.P.Tsat = TSatWater(bfw.F.P);
  double Tsat = bfw.P.Tsat;

  /* Calculating saturated condensate and steam at
 boiler pressure P_stm */
  sat_cond = flow("sat_cond", "water");
  sat_cond.F.T = Tsat - 5.0;
  sat_cond.F.P = par.fp("P_stm");
  sat_cond.calculate_flow_properties();
  sat_stm = flow("sat_stm", "water");
  sat_stm.F.P = par.fp("P_stm");
  sat_stm.F.T = Tsat + 5.0;
  sat_stm.calculate_flow_properties();

  /* Defining and calculating superheated steam
  from solid fuel boiler, based on specified
  temperature and pressure conditions, P_stm and T_stm  */
  steam = flow("steam", "water");
  steam.F.P = par.fp("P_stm");
  steam.F.T = par.fp("T_stm");
  steam.P.h = hPTSupSteam(steam.F.P, steam.F.T);
  steam.F.M = 1.0e-3 * par.fp("Q_stm") / (steam.P.h - bfw.P.h);
  steam_out = flow("cond", "water");

  /* Defining condensate flow */
  cond = flow("cond", "water");

  /* Evaluating heat demands to district heating */
  district_heating(par);

  /* Calculate steam turbine model using:
    1. steam at soild fuel boiler outlet
    2. steam extraction */
  steam_turbine_model(steam, steam_out, par);

  /* calculating steam condenser */
  steam_condenser(steam_out, cond, par);

  /* creating, specifying and calculating cost
  of rankine_cycle equipment */
  object rankine_eq("equipment", "rankine_cycle");
  rankine_eq.fval_p("S", par.fp("Q_stm") * 1e-6);
  equipment_cost(rankine_eq);
  par.c.push_back(rankine_eq);
}
