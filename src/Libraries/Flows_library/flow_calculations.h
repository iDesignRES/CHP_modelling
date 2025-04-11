

void mix_same_type_flows(flow &f1, flow &f2, flow &f){

  f = f1;

  f.F.M = f1.F.M + f2.F.M;

  f.F.N = f1.F.N + f2.F.N;

  f.F.VN = f1.F.VN + f2.F.VN;

  f.P.LHV = (f1.F.M * f1.P.LHV + f2.F.M * f2.P.LHV) / f.F.M;

  f.P.HHV = (f1.F.M * f1.P.HHV + f2.F.M * f2.P.HHV) / f.F.M;

  f.F.Ht = f1.F.Ht + f2.F.Ht;

  f.P.cp = (f1.F.M * f1.P.cp + f2.F.M * f2.P.cp) / f.F.M;

  f.P.rho = f.F.M / ((f1.F.M / f1.P.rho) + (f2.F.M / f2.P.rho));

  f.F.T = 25.0 + f.F.Ht / (f.F.M * f.P.cp);

  /*
  if(f.j.size() > 0){ for (size_t n = 0; n < f.j.size(); n++) {
    f.j[n].Y = (f1.j[n].Y * f1.F.M  + f2.j[n].Y * f2.F.M) / f.F.M;
    f.j[n].X = (f1.j[n].X * f1.F.N  + f2.j[n].X * f2.F.N) / f.F.N;
  }}
  */	
}

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

void flow::calculate_solid_fuel() {
  // cout << "calculating solid fuel properties" << endl;

  double sum_y = 0.0;
  double kC, kH, kO, kS, kN, kH2O, kA;
  double yC = 0.0, yH = 0.0, yS = 0.0, yN = 0.0, yO = 0.0;  // atomic components
  double yA = 0.0, yH2O = 0.0, yDM = 0.0;                   // proximates

  if (index_species(k, "H2O") == -1 && index_species(k, "DM") != -1) {
    yH2O = 1.0 - k[index_species(k, "DM")].Y;
    k.push_back(species("H2O", yH2O));
  }

  if (index_species(k, "H2O") != -1 && index_species(k, "DM") == -1) {
    yDM = 1.0 - k[index_species(k, "H2O")].Y;
    k.push_back(species("DM", yDM));
  }

  if (index_species(i, "O") == -1) {
    double sum_Yi = 0.0;
    for (size_t ni = 0; ni < i.size(); ni++) {
      sum_Yi += i[ni].Y;
    }
    double YO = 1.0 - sum_Yi;
    i.push_back(species("O", YO));
  }

  kC = 34.1;
  kH = 102;
  kO = -9.85;
  kS = 19.1;
  kN = 0;
  kH2O = -2.5;
  kA = 0.0;
  // kC = 34.1; kH = 110.4; kO = -12; kS = 6.86; kN = -12; kH2O = -2.442; kA = -1.53;  //
  // Milne's formulae (from Phyllis) kC = 34.8; kH = 93.9; kO = -10.8; kS = 10.5; kN = 6.3;
  // kH2O = -2.45; kA = 0.0; 

  // fetching the weight fractions:
  for (size_t n = 0; n < i.size(); n++) {
    if (i[n].id == "C") {
      yC = i[n].Y;
    } else if (i[n].id == "H") {
      yH = i[n].Y;
    } else if (i[n].id == "O") {
      yO = i[n].Y;
    } else if (i[n].id == "N") {
      yN = i[n].Y;
    } else if (i[n].id == "S") {
      yS = i[n].Y;
    }
  }

  // fetching the proximates:
  for (size_t n = 0; n < k.size(); n++) {
    if (k[n].id == "ash") {
      yA = k[n].Y;
    } else if (k[n].id == "H2O") {
      yH2O = k[n].Y;
    }
  }

  if (yH2O > 1.0) {
    yH2O = yH2O / 100;
  }

  if (P.LHV_dry == 0) {
    P.LHV_dry = kC * yC + kH * yH + kS * yS + kN * yN + kO * yO + kA * yA;
  }

  P.LHV = P.LHV_dry * (1.0 - yH2O) + kH2O * yH2O;
  P.HHV_dry = P.LHV_dry - kH2O * yH * (18 / 2);
  P.HHV = P.LHV - kH2O * (1 - yH2O) * yH * (18 / 2);

  if (yH2O > 0) {
    P.cp = 1.2e3 * (1.0 - yH2O) + 4.18e3 * yH2O;
    P.rho = 1.0 / ((1.0 - yH2O) / (0.5e3) + yH2O / 1e3);
  }

  F.V = F.M / P.rho;
  P.ht = P.cp * (F.T - 25.0);
  F.Ht = F.M * P.ht;
  P.hf = P.LHV * 1e6;
  F.Hf = F.M * P.hf;
  F.H = F.Ht + F.Hf;
}

void flow::calculate_gas_fuel() {
  if (P.LHV == 0) {
    for (size_t nj = 0; nj < j.size(); nj++) {
      P.LHV += j[nj].P.LHV;
    }
  }

  P.LHV = 0;
  P.MW = 0.0;
  for (size_t nj = 0; nj < j.size(); nj++) {
    P.LHV += j[nj].P.LHV * j[nj].Y;
    P.MW += j[nj].P.MW * j[nj].X;
    P.cp += j[nj].P.cp * j[nj].Y;
  }

  double sum_Y_rho = 0.0;
  for (size_t n = 0; n < j.size(); n++) {
    sum_Y_rho += j[n].Y / j[n].P.rho;
  }
  P.rho = 1.0 / sum_Y_rho;

  P.LHV_dry = P.LHV;

  P.ht = P.cp * (F.T - 25.0);
  P.hf = P.LHV * 1e6;
  P.h = P.ht + P.hf;

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

}

void flow::calculate_species_properties(string state_def) {

  // calculating individual species properties:
  for (size_t n = 0; n < j.size(); n++) {
    j[n].F.T = F.T;
    j[n].F.P = F.P;

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

      j[n].P.hf = thermodynamic_property(j[n].id, "hf", j[n].F.T + 273.15, j[n].F.P, "J/mol");
      if (j[n].P.hf == -1.0) {
        j[n].P.hf = 0.0;
        //cout << "Error: property: h cannot be calculated for species: " << j[n].id << endl;
      }

      j[n].P.s =
          thermodynamic_property(j[n].id, "s", j[n].F.T + 273.15, j[n].F.P, "J/molK");
      if (j[n].P.s == -1.0) {
        j[n].P.s = 0.0;
        //cout << "Error: property: s cannot be calculated for species: " << j[n].id << endl;
      }


    j[n].P.cp = j[n].P.cp / j[n].P.MW;
    j[n].P.h = j[n].P.h / j[n].P.MW;
    j[n].P.ht = j[n].P.ht / j[n].P.MW;
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
