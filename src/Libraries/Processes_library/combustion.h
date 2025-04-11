

void calculate_fuel_combustion_properties(flow fuel, object &prop) {

  size_t C = index_species(fuel.i, "C");
  size_t H = index_species(fuel.i, "H");
  size_t O = index_species(fuel.i, "O");

  size_t H2O = index_species(fuel.k, "H2O");
  size_t ash = index_species(fuel.k, "ash");

  if (fuel.prop_data == "solid_fuel") {
    prop.fval_p("n_H", (fuel.i[H].Y / 1) / (fuel.i[C].Y / 12));
    prop.fval_p("n_O", (fuel.i[O].Y / 16) / (fuel.i[C].Y / 12));
    prop.fval_p("W_CHxOy",
                0.012 + prop.fp("n_H") * 0.001 + prop.fp("n_O") * 0.016);
    prop.fval_p("nu_O2", 1 + prop.fp("n_H") / 4 - prop.fp("n_O") / 2);
    prop.fval_p("nu_CO2", 1.0);
    prop.fval_p("nu_H2O", prop.fp("n_H") / 2);
    prop.fval_p("V_stoich",
                fuel.F.M * (1 - fuel.k[H2O].Y) * (1 - fuel.k[ash].Y) *
                    (prop.fp("nu_O2") / 0.21) * (0.02241 / prop.fp("W_CHxOy")));
    prop.fval_p("vn_CO2_m", (1 - fuel.k[H2O].Y) * prop.fp("nu_CO2") *
                                (0.02241 / prop.fp("W_CHxOy")));
    prop.fval_p("vn_H2O_m", (1 - fuel.k[H2O].Y) * prop.fp("nu_H2O") *
                                    (0.02241 / prop.fp("W_CHxOy")) +
                                fuel.k[H2O].Y * (0.02241 / 0.018));
  }

  if (fuel.prop_data == "gas_fuel") {
    size_t CO2 = index_species(fuel.j, "CO2");

    if (CO2 > 0 && fuel.j[CO2].Y > 0.0) {
      // gas fuel contains CO2
      double Y_CO2 = fuel.j[CO2].Y;
      for (size_t nj = 0; nj < fuel.j.size(); nj++) {
        if (fuel.j[nj].id != "CO2") {
          fuel.j[nj].Y = fuel.j[nj].Y / (1.0 - fuel.j[CO2].Y);
        }
      }
      fuel.j[CO2].Y = 0.0;
      fuel.molec_def = "Y";
      fuel.calculate_flow_composition();
      fuel.interpret_molecules();

      prop.fval_p("n_H", (fuel.i[H].Y / 1.0) /
                             ((fuel.i[C].Y - (Y_CO2 * 12.0 / 44.0)) / 12.0));
      prop.fval_p("n_O", ((fuel.i[O].Y - (2.0 * Y_CO2 * 16.0 / 44.0)) / 16.0) /
                             ((fuel.i[C].Y - (Y_CO2 * 12.0 / 44.0)) / 12.0));
      prop.fval_p("W_CHxOy",
                  0.012 + prop.fp("n_H") * 0.001 + prop.fp("n_O") * 0.016);
      prop.fval_p("nu_O2", 1.0 + prop.fp("n_H") / 4.0 - prop.fp("n_O") / 2.0);
      prop.fval_p("nu_CO2", 1.0);
      prop.fval_p("nu_H2O", prop.fp("n_H") / 2.0);
      prop.fval_p("V_stoich", fuel.F.M * (1.0 - Y_CO2) * (1 - fuel.k[H2O].Y) *
                                  (1 - fuel.k[ash].Y) *
                                  (prop.fp("nu_O2") / 0.21) *
                                  (0.02241 / prop.fp("W_CHxOy")));
      prop.fval_p("vn_CO2_m",
                  (1 - fuel.k[H2O].Y) * ((1.0 - Y_CO2) * prop.fp("nu_CO2") *
                                             (0.02241 / prop.fp("W_CHxOy")) +
                                         Y_CO2 * (0.02241 / 0.044)));
      prop.fval_p("vn_H2O_m", (1 - fuel.k[H2O].Y) * (1.0 - Y_CO2) *
                                      prop.fp("nu_H2O") *
                                      (0.02241 / prop.fp("W_CHxOy")) +
                                  Y_CO2 * (0.02241 / 0.018));

    }

    else {

      prop.fval_p("n_H", (fuel.i[H].Y / 1.0) / (fuel.i[C].Y / 12.0));
      prop.fval_p("n_O", (fuel.i[O].Y / 16.0) / (fuel.i[C].Y / 12.0));
      prop.fval_p("W_CHxOy",
                  0.012 + prop.fp("n_H") * 0.001 + prop.fp("n_O") * 0.016);
      prop.fval_p("nu_O2", 1.0 + prop.fp("n_H") / 4.0 - prop.fp("n_O") / 2.0);
      prop.fval_p("nu_CO2", 1.0);
      prop.fval_p("nu_H2O", prop.fp("n_H") / 2.0);
      prop.fval_p("V_stoich", fuel.F.M * (1.0 - fuel.k[H2O].Y) *
                                  (1.0 - fuel.k[ash].Y) *
                                  (prop.fp("nu_O2") / 0.21) *
                                  (0.02241 / prop.fp("W_CHxOy")));
      prop.fval_p("vn_CO2_m", (1.0 - fuel.k[H2O].Y) * prop.fp("nu_CO2") *
                                  (0.02241 / prop.fp("W_CHxOy")));
      prop.fval_p("vn_H2O_m", (1.0 - fuel.k[H2O].Y) * prop.fp("nu_H2O") *
                                      (0.02241 / prop.fp("W_CHxOy")) +
                                  fuel.k[H2O].Y * (0.02241 / 0.018));
    }
  }
}

void solid_fuel_boiler(vector<flow> &fuel, vector<flow> &comb_air,
                       flow &flue_gas, flow &bottom_ash, flow &fly_ash,
                       object &comb) {

  vector<flow> air;
  vector<flow> fg;
  vector<flow> ba;
  vector<flow> fa;

  if (air.size() == 0) {
    for (size_t n = 0; n < fuel.size(); n++) {
      air.push_back(flow("dry_air"));
      fg.push_back(flow("flue_gas", "flue_gas"));
      ba.push_back(flow("bottom_ash", "ash"));
      fa.push_back(flow("fly_ash", "ash"));
    }
  }

  for (size_t n = 0; n < fuel.size(); n++) {
    size_t C = index_species(fuel[n].i, "C");
    size_t H = index_species(fuel[n].i, "H");
    size_t O = index_species(fuel[n].i, "O");
    size_t N = index_species(fuel[n].i, "N");
    size_t H2O = index_species(fuel[n].k, "H2O");
    size_t ash = index_species(fuel[n].k, "ASH");

    size_t O2g = index_species(fg[n].j, "O2");
    size_t N2g = index_species(fg[n].j, "N2");
    size_t CO2g = index_species(fg[n].j, "CO2");
    size_t H2Og = index_species(fg[n].j, "H2O");

    object comb_f("fuel", fuel[n].def);
    calculate_fuel_combustion_properties(fuel[n], comb_f);

    double T_0 = 25.0;
    air[n].F.VN = comb.fp("lambda") * comb_f.fp("V_stoich");
    air[n].F.T = comb.fp("T_ox");
    air[n].F.P = comb.fp("P_bar");
    air[n].molec_def = "X";
    air[n].calculate_flow("PT");

    fg[n].F.T = comb.fp("T_g");
    fg[n].F.P = comb.fp("P_bar");
    fg[n].j[O2g].F.VN = (comb.fp("lambda") - 1) * comb_f.fp("V_stoich") * 0.21;
    fg[n].j[N2g].F.VN = comb.fp("lambda") * comb_f.fp("V_stoich") * 0.79;
    fg[n].j[CO2g].F.VN = fuel[n].F.M * comb_f.fp("vn_CO2_m");
    fg[n].j[H2Og].F.VN = fuel[n].F.M * comb_f.fp("vn_H2O_m");
    fg[n].F.VN = 0.0;
    for (size_t nj = 0; nj < fg[n].j.size(); nj++) {
      fg[n].F.VN = fg[n].F.VN + fg[n].j[nj].F.VN;
    }
    for (size_t nj = 0; nj < fg[n].j.size(); nj++) {
      fg[n].j[nj].X = fg[n].j[nj].F.VN / fg[n].F.VN;
    }
    // fg[n].molec_def = "X";
    fg[n].calculate_flow("PT");

    // double T_ba = 1000.0, T_fa = 150.0, yC_ash = 0.03, f_ba = 0.1;

    double f_ba = comb.fp("f_ba");
    double T_ba = comb.fp("T_ba");
    double T_fa = comb.fp("T_fa");
    double yC_ba = comb.fp("yC_ba");

    // double T_ba = 1000.0, T_fa = 150.0, yC_ash = 0.03, f_ba = 0.1;

    ba[n].F.M =
        fuel[n].F.M * (1.0 - fuel[n].k[H2O].Y) * (fuel[n].k[ash].Y * f_ba);
    ba[n].P.cp = 1.25; // kJ/kg*k
    ba[n].F.Ht =
        ba[n].F.M * (ba[n].P.cp * (T_ba - 25.0) * 1.0e3 + yC_ba * 34.1 * 1.0e6);

    fa[n].F.M = fuel[n].F.M * (1.0 - fuel[n].k[H2O].Y) * fuel[n].k[ash].Y *
                (1.0 - f_ba);
    fa[n].P.cp = 1.25; // kJ/kg*k
    fa[n].F.Ht = fa[n].F.M * fa[n].P.cp * (T_fa - 25.0);
  }

  double comb_Hf = 0.0;
  for (size_t n = 0; n < fuel.size(); n++) {
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
  comb.fval_p("Q_loss", flue_gas.F.Ht * comb.fp("q_loss"));
  comb.fval_p("Q_out", comb.fp("Hf") * 1.0e6 + comb_air[0].F.Ht -
                           flue_gas.F.Ht - bottom_ash.F.Ht - comb.fp("Q_loss"));

  object boiler("equipment", "biomass_stoker_boiler_power");
  boiler.fval_p("S", comb.fp("M_fuel") * 3.6);
  equipment_cost(boiler);
  boiler.fval_p("W_el", comb.fp("M_fuel") * 3.6 * boiler.fp("w_el"));
  comb.c.push_back(boiler);

  object prep("equipment", "biomass_storage&feeding");
  prep.fval_p("S", comb.fp("M_fuel") * 3.6);
  equipment_cost(prep);
  comb.c.push_back(prep);

  air.clear();
  fg.clear();
  ba.clear();
  fa.clear();
}
