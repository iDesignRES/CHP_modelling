#include "combustion.h"
#include <cstddef>
#include <iostream>

#include "../../Cost.h"
#include "../Flows_library/flow_calculations.h"

void calculate_fuel_combustion_properties(flow fuel, object &prop) {
  std::size_t C = index_species(fuel.i, "C");
  std::size_t H = index_species(fuel.i, "H");
  std::size_t O = index_species(fuel.i, "O");

  std::size_t H2O = index_species(fuel.k, "H2O");
  std::size_t ash = index_species(fuel.k, "ash");

  if (fuel.prop_data == "solid_fuel") {
    std::string missing;
    if (C == static_cast<std::size_t>(-1)) missing += "C ";
    if (H == static_cast<std::size_t>(-1)) missing += "H ";
    if (O == static_cast<std::size_t>(-1)) missing += "O ";
    if (H2O == static_cast<std::size_t>(-1)) missing += "H2O ";
    if (ash == static_cast<std::size_t>(-1)) missing += "ash ";
    if (!missing.empty()) {
      throw std::runtime_error(
          "Error in fuel definition as following species are not present: " +
          missing);
    }
    /* Solid fuel composition per unit ol of carbon as CH_xO_y; x = n_H, y = n_O
     */
    /* n_H and n_O are the molar ratio of hydrogen and oxygen relative to carbon
     */
    prop.fval_p("n_H", (fuel.i[H].Y / 1) / (fuel.i[C].Y / 12));
    prop.fval_p("n_O", (fuel.i[O].Y / 16) / (fuel.i[C].Y / 12));
    /* W_CHxOy molecular weight of CHxOy */
    prop.fval_p("W_CHxOy",
                0.012 + prop.fp("n_H") * 0.001 + prop.fp("n_O") * 0.016);
    /* Stoichiometric coefficients, considering a global combustion reaction
       CHxOy + nu_O2 * (O2 + (79/21) N2) -> nu_CO2 * CO2 + nu_H2O * H2O  */
    prop.fval_p("nu_O2", 1 + prop.fp("n_H") / 4 - prop.fp("n_O") / 2);
    prop.fval_p("nu_CO2", 1.0);
    prop.fval_p("nu_H2O", prop.fp("n_H") / 2);

    /* V_stoich is the stoichiometric combustion air flow rate (Nm3/s) */
    prop.fval_p("V_stoich",
                fuel.F.M * (1 - fuel.k[H2O].Y) * (1 - fuel.k[ash].Y) *
                    (prop.fp("nu_O2") / 0.21) * (0.02241 / prop.fp("W_CHxOy")));
    /* vn_CO2_m: stoichiometric normal flow rate of CO2 (Nm3) per unit mass (kg)
     * of solid fuel*/
    /* vn_H2O_m: stoichiometric normal flow rate of H2O (Nm3) per unit mass (kg)
     * of solid fuel*/
    prop.fval_p("vn_CO2_m", (1 - fuel.k[H2O].Y) * prop.fp("nu_CO2") *
                                (0.02241 / prop.fp("W_CHxOy")));
    prop.fval_p("vn_H2O_m", (1 - fuel.k[H2O].Y) * prop.fp("nu_H2O") *
                                    (0.02241 / prop.fp("W_CHxOy")) +
                                fuel.k[H2O].Y * (0.02241 / 0.018));
  }
}

void solid_fuel_boiler(std::vector<flow> &fuel, std::vector<flow> &comb_air,
                       flow &flue_gas, flow &bottom_ash, flow &fly_ash,
                       object &comb) {
  /*Calculation of mass and energy balances for a solid fuel boiler
    Solid fuel defined as a vector of biomass feedstock
    Assumed linear contribution of each biomass feedstock
    based on one global combustion reaction
    CHxOy + nu_O2 * (O2 + (79/21) N2) -> nu_CO2 * CO2 + nu_H2O * H2O
    with linear contribution of each biomass feedstock */

  std::vector<flow> air;
  std::vector<flow> fg;
  std::vector<flow> ba;
  std::vector<flow> fa;

  if (air.size() == 0) {
    for (std::size_t n = 0; n < fuel.size(); n++) {
      air.push_back(flow("dry_air"));
      fg.push_back(flow("flue_gas", "flue_gas"));
      ba.push_back(flow("bottom_ash", "ash"));
      fa.push_back(flow("fly_ash", "ash"));
    }
  }

  /* Calculation of mass and energy balances
  as a linear contribution from combustion of each
  biomass feedstock in the feed */

  for (std::size_t n = 0; n < fuel.size(); n++) {
    std::size_t H2O = index_species(fuel[n].k, "H2O");
    std::size_t ash = index_species(fuel[n].k, "ash");

    std::size_t O2g = index_species(fg[n].j, "O2");
    std::size_t N2g = index_species(fg[n].j, "N2");
    std::size_t CO2g = index_species(fg[n].j, "CO2");
    std::size_t H2Og = index_species(fg[n].j, "H2O");

    object comb_f("fuel", fuel[n].def);
    calculate_fuel_combustion_properties(fuel[n], comb_f);

    /* Calculation of combustion air
    based on stoichiometric air flow rate, using specified input
    excess ratio (lambda)), temperature (T_ox) and pressure (P_bar) */
    air[n].F.VN = comb.fp("lambda") * comb_f.fp("V_stoich");
    air[n].F.T = comb.fp("T_ox");
    air[n].F.P = comb.fp("P_bar");
    air[n].molec_def = "X";
    air[n].calculate_flow();

    /* Calculation of flue gas flow parameters and
    O2, N2, CO2, H2O composition based on
    mass, molecules and energy balances
    using calculated stoiciometry and specified input
    excess ratio (lambda)), temperature (T_g) and pressure (P_bar) */

    fg[n].F.T = comb.fp("T_g");
    fg[n].F.P = comb.fp("P_bar");
    fg[n].j[O2g].F.VN = (comb.fp("lambda") - 1) * comb_f.fp("V_stoich") * 0.21;
    fg[n].j[N2g].F.VN = comb.fp("lambda") * comb_f.fp("V_stoich") * 0.79;
    fg[n].j[CO2g].F.VN = fuel[n].F.M * comb_f.fp("vn_CO2_m");
    fg[n].j[H2Og].F.VN = fuel[n].F.M * comb_f.fp("vn_H2O_m");
    fg[n].F.VN = 0.0;
    for (std::size_t nj = 0; nj < fg[n].j.size(); nj++) {
      fg[n].F.VN = fg[n].F.VN + fg[n].j[nj].F.VN;
    }
    for (std::size_t nj = 0; nj < fg[n].j.size(); nj++) {
      fg[n].j[nj].X = fg[n].j[nj].F.VN / fg[n].F.VN;
    }
    fg[n].calculate_flow();

    /* Calculation of bottom and fly ash flows using:
     f_ba = fraction of total ash as bottom ash
     yC_ba = Carbon content in bottom ash (as mass fraction on dry basis)
     T_ba = temperature bottom ash at boiler outlet
     T_fa = temperature fly ash at boiler outlet*/

    double f_ba = comb.fp("f_ba");
    double T_ba = comb.fp("T_ba");
    double T_fa = comb.fp("T_fa");
    double yC_ba = comb.fp("yC_ba");

    ba[n].F.M =
        fuel[n].F.M * (1.0 - fuel[n].k[H2O].Y) * (fuel[n].k[ash].Y * f_ba);
    ba[n].P.cp = 1.25;  // kJ/kg*k, assumed value
    ba[n].F.Ht =
        ba[n].F.M * (ba[n].P.cp * (T_ba - 25.0) * 1.0e3 + yC_ba * 34.1 * 1.0e6);

    fa[n].F.M = fuel[n].F.M * (1.0 - fuel[n].k[H2O].Y) * fuel[n].k[ash].Y *
                (1.0 - f_ba);
    fa[n].P.cp = 1.25;  // kJ/kg*k, assumed value
    fa[n].F.Ht = fa[n].F.M * fa[n].P.cp * (T_fa - 25.0);
  }

  /* Calculation of total input fuel energy,
   combustion air, flue gas, bottom and fly ash flows as a
   sum of contributions from all biomass feedstock combustion,
   using:
   f_ba = fraction of total ash as bottom ash
   yC_ba = Carbon content in bottom ash (as mass fraction on dry basis)
   T_ba = temperature bottom ash at boiler outlet
   T_fa = temperature fly ash at boiler outlet */

  double comb_Hf = 0.0;
  for (std::size_t n = 0; n < fuel.size(); n++) {
    if (n == 0) {
      if (comb_air.size() == 0) {
        comb_air.push_back(air[n]);
      } else if (comb_air.size() > 0) {
        comb_air[0] = air[n];
      }
      flue_gas = fg[n];
      bottom_ash = ba[n];
      fly_ash = fa[n];
    }
    if (n > 0) {
      mix_same_type_flows(comb_air[0], air[n], comb_air[0]);
      mix_same_type_flows(bottom_ash, ba[n], bottom_ash);
      mix_same_type_flows(flue_gas, fg[n], flue_gas);
      mix_same_type_flows(fly_ash, fa[n], fly_ash);
    }

    comb_Hf = comb_Hf + fuel[n].F.Hf;
  }

  comb.fval_p("Hf", comb_Hf);

  /* Calculation of heat losses, as proportional to
  flue gas enthalpy flow with the input parameter q_loss
  as propertionality constant
  */

  comb.fval_p("Q_loss", flue_gas.F.Ht * comb.fp("q_loss"));

  /* Calculation of boiler heat output from energy balance*/
  comb.fval_p("Q_out", comb.fp("Hf") * 1.0e6 + comb_air[0].F.Ht -
                           flue_gas.F.Ht - bottom_ash.F.Ht - comb.fp("Q_loss"));

  /* defining and specifying the equipment associated to the solid_fuel_boiler:
      1. biomass_stoker_boiler_power
      2. biomass_storage_and_feeding
  */

  object boiler("equipment", "biomass_stoker_boiler_power");
  boiler.fval_p("S", comb.fp("M_fuel") * 3.6);
  equipment_cost(boiler);
  boiler.fval_p("W_el", comb.fp("M_fuel") * 3.6 * boiler.fp("w_el"));
  comb.c.push_back(boiler);

  object prep("equipment", "biomass_storage_and_feeding");
  prep.fval_p("S", comb.fp("M_fuel") * 3.6);
  equipment_cost(prep);
  comb.c.push_back(prep);

  air.clear();
  fg.clear();
  ba.clear();
  fa.clear();
}
