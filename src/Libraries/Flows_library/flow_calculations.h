

void flow::calculate_flow(string state_def) {
  if (prop_data == "solid_fuel" || prop_data == "bio_oil" || prop_data == "ash") {
    calculate_solid_fuel();
  }

  if (prop_data == "gas_fuel") {
    calculate_flow_composition();
    calculate_flow_parameters();
    calculate_gas_fuel();
  }

  if (prop_data != "solid_fuel" && prop_data != "gas_fuel" && prop_data != "bio_oil" &&
      prop_data != "ash") {
    calculate_flow_composition();
    if (molec_def == "Y" || molec_def == "X") {
      calculate_flow_properties(state_def);
    }
    calculate_flow_parameters();
  }
}

void flow::calculate_flow_composition() {
  double sum_Y, sum_X, sum_X_MW, sum_Y_MW;
  P.MW = 0;
  if (j.size() == 1) {
    P.MW = j[0].P.MW;
    j[0].Y = 1.0;
    j[0].X = 1.0;
  }

  if (j.size() > 1) {
    sum_Y = 0.0;
    for (size_t n = 0; n < j.size(); n++) {
      sum_Y += j[n].Y;
    }
    sum_X = 0.0;
    for (size_t n = 0; n < j.size(); n++) {
      sum_X += j[n].X;
    }
    if (molec_def != "Y" && sum_Y > 0.0) {
      molec_def = "Y";
    }
    if (molec_def != "X" && sum_Y == 0.0 && sum_X > 1e-6) {
      molec_def = "X";
    }

    if (molec_def == "Y") {
      sum_Y_MW = 0.0;
      for (size_t n = 0; n < j.size(); n++) {
        if (j[n].Y > 0 && j[n].P.MW > 0) {
          sum_Y_MW += j[n].Y / j[n].P.MW;
        }
      }
      for (size_t n = 0; n < j.size(); n++) {
        if (j[n].Y > 0 && j[n].P.MW > 0 && sum_Y_MW > 0) {
          j[n].X = (j[n].Y / j[n].P.MW) / sum_Y_MW;
        }
        if (j[n].Y == 0) {
          j[n].X = 0;
        }
      }
    } else if (molec_def == "X") {
      sum_X_MW = 0.0;
      for (size_t n = 0; n < j.size(); n++) {
        if (j[n].X > 0 && j[n].P.MW > 0) {
          sum_X_MW += j[n].X * j[n].P.MW;
        }
      }
      for (size_t n = 0; n < j.size(); n++) {
        if (j[n].X > 0 && j[n].P.MW > 0 && sum_X_MW > 0) {
          j[n].Y = (j[n].X * j[n].P.MW) / sum_X_MW;
        }
        if (j[n].X == 0) {
          j[n].Y = 0;
        }
      }
    }

    for (size_t n = 0; n < j.size(); n++) {
      P.MW += j[n].X * j[n].P.MW;
    }
  }
}

void flow::calculate_flow_properties(string state_def) {
  double cp, hVap, BP, DP;
  cp = 0.0;
  hVap = 0.0;
  BP = 0.0;
  DP = 0.0;
  P.cp = 0;
  P.rho = 0;

  if (prop_data != "refprop" && prop_data != "NASA" && prop_data != "solid_fuel" &&
      prop_data != "NIST") {
    define_flow_prop_data();
  }

  // cout << "flow prop_data: " << prop_data << endl;

  if (prop_data == "solid_fuel") {
    calculate_solid_fuel();
  }

  if (prop_data == "NIST") {

    calculate_species_properties(state_def);

    P.cp = 0.0;
    P.h = 0.0;
    P.s = 0.0;

    for (size_t n = 0; n < j.size(); n++) {
      P.cp += j[n].Y * j[n].P.cp;
      P.h += j[n].Y * j[n].P.h;
      P.s += j[n].Y * j[n].P.s;
    }

    P.ht = P.cp * (F.T - 25.0);
  }

  if (prop_data == "hybrid") {

    calculate_species_properties(state_def);

    if (j.size() == 1) {
      P.cp = j[0].P.cp;
      P.rho = j[0].P.rho;
      P.h = j[0].P.h;
      P.ht = j[0].P.cp * (F.T - 25.0);
      if (P.h == 0 && P.ht > 0 && P.hf == 0) {
        j[0].P.h = j[0].P.ht;
      }
      P.s = j[0].P.s;
      P.g = j[0].P.g;
    }

    if (j.size() > 1) {
      for (size_t n = 0; n < j.size(); n++) {
        // update flow properties (assume mass basis at first (MUST BE CHANGED LATER)):
        // P.cp += j[n].X * j[n].P.cp;
        P.cp += j[n].Y * j[n].P.cp;
        P.h += j[n].Y * j[n].P.h;
        P.ht += j[n].Y * j[n].P.ht;
        P.hf += j[n].Y * j[n].P.hf;
        P.s += j[n].Y * j[n].P.s;
        P.g += j[n].Y * j[n].P.g;
        P.hVap += j[n].P.hVap * j[n].Y;

        // Bubble point && dew point will be the minimum && maximum for the individual
        // components
        if (n == 0) {
          BP = j[n].P.Tsat;
          DP = j[n].P.Tsat;
        }

        if (j[n].P.Tsat < BP) {
          BP = j[n].P.Tsat;
        }

        if (j[n].P.Tsat > DP) {
          DP = j[n].P.Tsat;
        }
      }

      double sum_Y_rho = 0.0;
      for (size_t n = 0; n < j.size(); n++) {
        sum_Y_rho += j[n].Y / j[n].P.rho;
      }
      P.rho = 1.0 / sum_Y_rho;
    }
  }
}

void flow::calculate_species_properties(string state_def) {

  for (size_t n = 0; n < j.size(); n++) {

    j[n].F.T = F.T;

    j[n].F.P = F.P;

    //if (!j[n].refprop && j[n].thermoPkg) {
      j[n].P.cp =
          thermodynamic_property(j[n].id, "cp", j[n].F.T + 273.15, j[n].F.P, "J/molK");
      if (j[n].P.cp == -1.0) {
        j[n].P.cp = 0.0;
        //cout << "Error: property: cp cannot be calculated for species: " << j[n].id << endl;
      }

      j[n].P.h = thermodynamic_property(j[n].id, "h", j[n].F.T + 273.15, j[n].F.P, "J/mol");
      if (j[n].P.h == -1.0) {
        j[n].P.h = 0.0;
        //cout << "Error: property: h cannot be calculated for species: " << j[n].id << endl;
      }

      j[n].P.s =
          thermodynamic_property(j[n].id, "s", j[n].F.T + 273.15, j[n].F.P, "J/molK");
      if (j[n].P.s == -1.0) {
        j[n].P.s = 0.0;
        //cout << "Error: property: s cannot be calculated for species: " << j[n].id << endl;
      }

    //}

    j[n].P.cp = j[n].P.cp / j[n].P.MW;
    j[n].P.h = j[n].P.h / j[n].P.MW;
    j[n].P.ht = j[n].P.cp * (j[n].F.T - 25.0) / j[n].P.MW;
    j[n].P.hf = j[n].P.hf / j[n].P.MW;
    j[n].P.s = j[n].P.s / j[n].P.MW;

    //cout << j[n].def << " cp = " << j[n].P.cp << " ht = " << j[n].P.ht << endl;  	

  }
}

void flow::define_flow_prop_data() {
  // determine the flow calculation method: refprop, hybrid, NASA
  // HYBRID incur whenever we have two || more different calculation methods

  // cout << "flow: " << def << " has prop_data: " << prop_data << endl;

  if (j.size() == 0) {
    cout << '\n'
         << "Error in method: calculate_flow(string). No molecular composition has been "
            "provided!"
         << endl;
    return;
  }

  else if (j.size() == 1) {
    if (j[0].refprop) {
      prop_data = "refprop";
    }
    if (j[0].NASA && !j[0].refprop) {
      prop_data = "NASA";
    }
    if (!j[0].NASA && !j[0].refprop) {
      prop_data = "hybrid";
    }
  }

  else if (j.size() > 1) {
    bool refprop = true;
    bool NASA = true;
    bool thermoPkg = true;

    for (size_t n = 0; n < j.size(); n++) {
      if (!j[n].refprop) {
        refprop = false;
      }
      if (!j[n].NASA) {
        NASA = false;
      }
    }

    if (refprop) {
      prop_data = "refprop";
    }
    if (NASA && !refprop) {
      prop_data = "NASA";
    }
    if (!NASA && !refprop) {
      prop_data = "hybrid";
    }
  }

  // cout << "flow: " << def << " has prop_data: " << prop_data << endl;
}

void flow::calculate_flow_parameters() {
  if (F.M == 0 && F.VN > 0 && F.N > 0 && P.MW > 0) {
    F.M = F.N * P.MW;
  } else if (F.N == 0 && F.VN > 0 && F.M > 0 && P.MW > 0) {
    F.N = F.M / P.MW;
  } else if (F.VN > 0 && F.M == 0 && F.N == 0 && P.MW > 0) {
    F.N = F.VN / 0.02214;
    F.M = F.N * P.MW;
  }

  F.H = F.M * P.h;
  F.Ht = F.M * P.ht;
  F.Hf = F.M * P.hf;

  if (F.H == 0 && F.Ht > 0) {
    F.H = F.Ht;
  }
  if (F.Hf == 0 && F.H > 0 && F.Ht > 0) {
    F.Hf = F.H - F.Ht;
  }
}
