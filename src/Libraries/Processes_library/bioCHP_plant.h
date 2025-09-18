/**
 * @brief function to get feedstock defined in plant object
 *
 * @input &plant pointer to object with feedstock specification
 *
 * @return &f = flow vector containing all feedstock
 */
void get_feedstock(vector<flow> &f, object &plant) {
  double LHV = 0.0;
  for (size_t nf = 0; nf < plant.svct("fuel_def").size(); nf++) {
    f.push_back(flow("feed", plant.svct("fuel_def")[nf]));
    f[nf].F.T = 25.0;
    f[nf].F.P = 1.01325;
    LHV += f[nf].P.LHV * plant.vctp("Yj")[nf];

    if (index_species(f[nf].k, "H2O") < 0) {
      f[nf].k.push_back(species("H2O", plant.vctp("YH2Oj")[nf]));
    } else if (index_species(f[nf].k, "H2O") >= 0) {
      size_t H2O = index_species(f[nf].k, "H2O");
      f[nf].k[H2O].Y = plant.vctp("YH2Oj")[nf];
    }
  }

  plant.fval_p("LHV_f", LHV);
}

/**
 * @brief function to calculate the bioCHP plant model
 *
 * @input &bioCHP object to object with bioCHP input parameters
 *
 * @return &bioCHP object with calculated output parameters
 */
void bioCHP_plant_model(object &bioCHP) {
  object boiler("system", "solid_fuel_boiler", DIR + "Database/bioCHP_inputs");
  object rankine("process", "Rankine_cycle", DIR + "Database/bioCHP_inputs");
  object scrubber("process", "flue_gas_cleaning",
                  DIR + "Database/bioCHP_inputs");

  cout << "Getting the feedstock data: " << endl;

  vector<flow> feed;
  get_feedstock(feed, bioCHP);
  double LHV_f = bioCHP.fp("LHV_f");
  vector<double> Yj = bioCHP.vctp("Yj");

  cout << "Feedstock LHV: " << bioCHP.fp("LHV_f") << endl;

  flow flue_gas, bottom_ash, fly_ash, dh_in, dh_out;
  vector<flow> comb_air;

  cout << "Importing Rankine cycle parameters: " << endl;

  rankine.fval_p("P_stm", bioCHP.fp("P_stm"));
  rankine.fval_p("T_stm", bioCHP.fp("T_stm"));
  rankine.vct_fp("Qk", bioCHP.vctp("Qk"));
  rankine.vct_fp("Tk_in", bioCHP.vctp("Tk_in"));
  rankine.vct_fp("Tk_out", bioCHP.vctp("Tk_out"));

  vector<double> Qk = bioCHP.vctp("Qk");
  double sum_Qk = 0.0;
  for (size_t nk = 0; nk < Qk.size(); nk++) {
    sum_Qk += Qk[nk];
  }

  if (bioCHP.bp("W_el")) {
    cout << "bioCHP PLANT calculation using W_el = " << bioCHP.fp("W_el")
         << endl;

    cout << "Estimating the required feedstock mass flow rate" << endl;

    double W_el = bioCHP.fp("W_el");

    double Hf = W_el / 0.2 + sum_Qk / 0.9;

    double ratio = 0.0;

    double W_el_prod = 0.0;

    for (int n = 0; n < 10; n++) {
      double Mf = Hf / LHV_f;

      for (size_t nf = 0; nf < Yj.size(); nf++) {
        feed[nf].F.M = Yj[nf] * Mf;
        feed[nf].F.Hf = feed[nf].F.M * feed[nf].P.LHV;
      }

      object b = boiler;
      b.fval_p("M_fuel", Mf);
      solid_fuel_boiler(feed, comb_air, flue_gas, bottom_ash, fly_ash, b);

      // cout << "Boiler Q_out (W) = " << b.fp("Q_out") << endl;

      object r = rankine;
      r.fval_p("Q_stm", b.fp("Q_out"));
      rankine_cycle(r);

      W_el_prod = r.fp("W_el");

      // cout << '\t' << "Hf (MW) = " << Hf << " W_el (MW) = " << W_el
      //       << " W_el_prod (MW) = " << W_el_prod << endl;

      Hf = Hf * W_el / W_el_prod;

      // cout << '\t' << "Hf (MW) = " << Hf << endl;
    }

    bioCHP.fval_p("Hf", Hf);
  }

  bioCHP.fval_p("M_fuel", bioCHP.fp("Hf") / bioCHP.fp("LHV_f"));

  for (size_t nf = 0; nf < feed.size(); nf++) {
    feed[nf].F.M = bioCHP.vctp("Yj")[nf] * bioCHP.fp("M_fuel");
    feed[nf].F.Hf = feed[nf].F.M * feed[nf].P.LHV;
    bioCHP.c.push_back(object("consumable", feed[nf].def));
    int f = bioCHP.ic("consumable", feed[nf].def);
    bioCHP.c[f].fval_p("Q_annual", feed[nf].F.M * 3.6 * 8000);
    material_cost(bioCHP.c[f]);
  }

  boiler.fval_p("M_fuel", bioCHP.fp("M_fuel"));
  solid_fuel_boiler(feed, comb_air, flue_gas, bottom_ash, fly_ash, boiler);

  cout << "-------------" << endl;
  cout << "Boiler: " << endl;
  cout << "-------------" << endl;
  cout << "Mass balance" << endl;
  cout << "------------" << endl;
  cout << "fuel M (kg/s): " << boiler.fp("M_fuel") << endl;
  for (size_t nf = 0; nf < feed.size(); nf++) {
    cout << '\t' << feed[nf].def << " (kg/s): " << feed[nf].F.M << endl;
  }
  cout << "Combustion air M: " << comb_air[0].F.M << endl;
  cout << "flue gas M: " << flue_gas.F.M << endl;
  cout << "bottom ash M: " << bottom_ash.F.M << endl;
  cout << "fly ash M: " << fly_ash.F.M << endl;

  cout << "-------------" << endl;
  cout << "energy balance" << endl;
  cout << "-------------" << endl;
  cout << "Fuel Hf (MW): " << boiler.fp("Hf") << endl;
  for (size_t nf = 0; nf < feed.size(); nf++) {
    cout << '\t' << feed[nf].def << " (MW): " << feed[nf].F.Hf << endl;
  }
  cout << "Q_out: (MW) " << boiler.fp("Q_out") * 1.0e-6 << endl;
  cout << "Q_loss: (MW) " << boiler.fp("Q_loss") * 1.0e-6 << endl;
  cout << "H_air: (MW) " << comb_air[0].F.Ht * 1.0e-6 << endl;
  cout << "H_fg: (MW) " << flue_gas.F.Ht * 1.0e-6 << endl;
  cout << "H_ba: (MW) " << bottom_ash.F.Ht * 1.0e-6 << endl;
  cout << "H_fa: (MW) " << fly_ash.F.Ht * 1.0e-6 << endl;
  cout << "-------------" << endl;

  rankine.fval_p("Q_stm", boiler.fp("Q_out"));
  rankine_cycle(rankine);

  scrubber.fval_p("M_fuel", bioCHP.fp("M_fuel"));

  dry_scrubber_model(flue_gas, flue_gas, scrubber);

  bioCHP.c.push_back(boiler);
  bioCHP.c.push_back(rankine);
  bioCHP.c.push_back(scrubber);

  for (size_t nf = 0; nf < feed.size(); nf++) {
    bioCHP.fval_p("output-Mj", feed[nf].F.M);
    bioCHP.fval_p("output-Hfj", feed[nf].F.Hf);
  }

  bioCHP.fval_p("output-Biomass_mass_input_(t/h)", bioCHP.fp("M_fuel") * 3.6);
  bioCHP.fval_p("output-Biomass_energy_input_(MW)", bioCHP.fp("Hf"));
  bioCHP.fval_p("output-Heat_production_(MW)", sum_Qk);
  bioCHP.fval_p("output-Electricity_production_(MW)", rankine.fp("W_el"));

  cost(bioCHP);
}
