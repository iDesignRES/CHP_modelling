#include <string.h>

#include <iostream>
#include <vector>

using namespace std;

vector<string> divide_string(string str, char c) {
  // cout << "divide " << str << endl;
  vector<string> list;
  vector<char> cstr(str.begin(), str.end());
  string element = "";
  bool element_found = false;
  int l = 0;
  while (l < str.length()) {
    if (cstr[l] != c) {
      element = element + cstr[l];
    }
    if (cstr[l] == c) {
      list.push_back(element);
      element = "";
    }
    if (l == str.length() - 1) {
      list.push_back(element);
      break;
    }
    // cout << l << " " << cstr[l] << " " << element << endl;
    l = l + 1;
  }
  cstr.clear();
  return list;
}

#include "pathfinder.h"
using namespace MyPaths;
// std::string DIR = getActualDir(string(__FILE__));
// std::string DIR = getExecutableDir()+"../src/";
// std::string DIR = getCurrentDirectory();
std::string DIR = getFileDirectory() + "/";
// std::string DIR = MAIN_DIR+"/";
std::string project = project_name();

// #include "Parameters.h"
#include "Parameters.cpp"
#include "Flows.h"
#include "Cost.h"
#include "Processes.h"

bool bioCHP_plant(vector<string> fuel_def, vector<double> Yj, vector<double> YH2Oj,
                  double W_el, vector<double> Qk, vector<double> Tk_in,
                  vector<double> Tk_out, vector<double> &Mj, double &Q_prod,
                  double &W_el_prod, double &C_inv, double &C_op, double &C_op_var) {
  // INPUTS
  // feed_def: name of each biomass feedstock
  // Yj: mass fraction of each biomass feedstock
  // YH2Oj: moisture of each biomass feedstock
  // W_el: electric power output (MW_el)
  // Qk: heat demand (MW)
  // Tk_in: Return temperature for each heat demand (district heating)
  // Tk_in: Supply temperature for each heat demand (district heating)

  // OUTPUTS
  // Mj: Required mass flow of each biomass feedstock
  // Q_prod: calculated heat production (MW)
  // W_el_prod: calculated electric power production (MW)
  // C_inv: Investment cost
  // C_op: Total operating cost
  // C_op_var: Variable operating cost

  // cout << "calculating the bioCHP plant model" << endl;
  // cout << "Executable path: " << getExecutablePath() << endl;
  // cout << "Actual DIR: " << DIR << endl;
  // cout << "Project name: " << project << endl;

  // Check specificatins of feedstock
  if (fuel_def.size() != Yj.size()) {
    cout << "number of specifications for Yj and fuel_def re different" << endl;
    return false;
  }

  // Check that all feedstock exist in the database
  for (size_t nf = 0; nf < fuel_def.size(); nf++) {
    if (!find_flow(fuel_def[nf])) {
      for (size_t nff = 0; nff < fuel_def.size(); nff++) {
        Mj.push_back(0.0);
      }
      Q_prod = 0.0;
      W_el_prod = 0.0;
      C_inv = 0.0;
      C_op = 0.0;
      C_op_var = 0.0;
      return false;
    }
  }

  // Check specificatins of heat demands
  if (Qk.size() != Tk_in.size()) {
    cout << "number of specifications for Tk_in and Qk are different" << endl;
    return false;
  }
  if (Qk.size() != Tk_out.size()) {
    cout << "number of specifications for Tk_out and Qk are different" << endl;
    return false;
  }
  if (Tk_in.size() != Tk_out.size()) {
    cout << "number of specifications for Tk_in and Tk_out are different" << endl;
    return false;
  }
  for (size_t nk = 0; nk < Tk_in.size(); nk++) {
    if (Tk_in[nk] > Tk_out[nk]) {
      cout << "return temperature of heat demand no. " << nk
           << " is higher than supply temperature" << endl;
      return false;
    }
  }

  // Check that there is sufficient heat available from Rankine cycle
  double sum_Qk = 0.0;
  for (size_t nk = 0; nk < Qk.size(); nk++) {
    sum_Qk = sum_Qk + Qk[nk];
  }
  if (sum_Qk > 0.5 * (W_el / 0.2)) {
    cout << "there is not sufficient heat available from Rankine cycle to supply the "
            "specifiy heat demand"
         << endl;
    cout << "Reducing proportionally the heat demands" << endl;
    for (size_t nk = 0; nk < Qk.size(); nk++) {
      Qk[nk] = Qk[nk] * (0.5 * (W_el / 0.2)) / sum_Qk;
    }

    double sum_Qk = 0.0;
    for (size_t nk = 0; nk < Qk.size(); nk++) {
      sum_Qk = sum_Qk + Qk[nk];
    }
  }

  object bioCHP("plant", "bioCHP_PLANT", DIR + "Database/bioCHP_inputs");
  bioCHP.vct_sp("fuel_def", fuel_def); 
  bioCHP.vct_fp("Yj", Yj);  
  bioCHP.vct_fp("YH2Oj", YH2Oj); 
  bioCHP.fval_p("W_el", W_el); 
  bioCHP.vct_fp("Qk", Qk); 
  bioCHP.vct_fp("Tk_in", Tk_in);
  bioCHP.vct_fp("Tk_out", Tk_out);

  bioCHP_plant_model(bioCHP);

  Mj = bioCHP.vctp("Mj");
  Q_prod = bioCHP.fp("Heat_production_(MW)");
  W_el_prod = bioCHP.fp("Electricity_production_(MW)");
  C_inv = bioCHP.fp("C_inv") * 1e-6;
  C_inv = bioCHP.fp("C_inv") * 1e-6;
  C_op = bioCHP.fp("C_op") * 1e-6;
  C_op_var = bioCHP.fp("C_op_var") * 1e-6;

  export_output_parameters(bioCHP, getExecutableDir() + "Output-bioCHP_" + project);

  return true;
}
