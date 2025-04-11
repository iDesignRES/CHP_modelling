

void flow::calculate_flow(string state_def) {
  if (prop_data == "solid_fuel" || prop_data == "bio_oil" ||
      prop_data == "ash") {
    calculate_solid_fuel();
  }

  if (prop_data == "gas_fuel") {
    calculate_flow_composition();
    calculate_flow_parameters();
    calculate_gas_fuel();
  }

  if (prop_data != "solid_fuel" && prop_data != "gas_fuel" &&
      prop_data != "bio_oil" && prop_data != "ash") {
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
      P.ht += j[n].Y * j[n].P.ht;
      P.s += j[n].Y * j[n].P.s;
    }
  }
}

void flow::calculate_species_properties(string state_def) {
  for (size_t n = 0; n < j.size(); n++) {
    j[n].F.T = F.T;

    j[n].F.P = F.P;

    j[n].P.cp = thermodynamic_property(j[n].id, "cp", j[n].F.T + 273.15,
                                       j[n].F.P, "J/molK");
    if (j[n].P.cp == -1.0) {
      j[n].P.cp = 0.0;
      // cout << "Error: property: cp cannot be calculated for species: " <<
      // j[n].id << endl;
    }

    j[n].P.h = thermodynamic_property(j[n].id, "h", j[n].F.T + 273.15, j[n].F.P,
                                      "J/mol");
    if (j[n].P.h == -1.0) {
      j[n].P.h = 0.0;
      // cout << "Error: property: h cannot be calculated for species: " <<
      // j[n].id << endl;
    }

    j[n].P.hf = thermodynamic_property(j[n].id, "hf", j[n].F.T + 273.15,
                                       j[n].F.P, "J/mol");
    if (j[n].P.hf == -1.0) {
      j[n].P.hf = 0.0;
      // cout << "Error: property: h cannot be calculated for species: " <<
      // j[n].id << endl;
    }

    j[n].P.s = thermodynamic_property(j[n].id, "s", j[n].F.T + 273.15, j[n].F.P,
                                      "J/molK");
    if (j[n].P.s == -1.0) {
      j[n].P.s = 0.0;
      // cout << "Error: property: s cannot be calculated for species: " <<
      // j[n].id << endl;
    }

    j[n].P.cp = j[n].P.cp / j[n].P.MW;
    j[n].P.h = j[n].P.h / j[n].P.MW;
    j[n].P.ht = j[n].P.cp * (j[n].F.T - 25.0) / j[n].P.MW;
    j[n].P.hf = j[n].P.hf / j[n].P.MW;
    j[n].P.s = j[n].P.s / j[n].P.MW;
  }
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
