#pragma once

#include <vector>
#include <string>

struct properties {
 public:
  double LHV = 0.0, LHV_dry, HHV = 0.0, HHV_dry;
  double cp, rho, MW;
  double hf, ht, h;              // J/mol (REFPROP)
  double s, g, gf, e;            // (REFPROP)
  double DP, BP, hVap, Tsat, q;  // dew point, bubble point, heat of evaporation
  double k, visc;
  double em_psr;       // energy particle size reducton
  std::vector<double> val;  // valences
};

struct flow_parameters {
 public:
  double P, T;
  double M, N, VN, V;
  double H, Ht, Hf;
};

struct species {
 public:
  std::string id, def, formula;
  std::string prop_data;
  bool refprop, NASA, thermoPkg, input;
  std::string molec_db, refprop_file = "NONE", NASA_file, thermoPkg_file;
  double TC, Y, X;
  std::vector<double> val;
  properties P;
  flow_parameters F;
  species(){};
  species(std::string sid);
  species(std::string sid, double sY);
  species(std::string sid, double val, std::string def);
  void calculate_refprop(std::string);
  void get_species_data_(std::string);
  void calculate_thermodynamic_properties();
};

std::size_t index_species(std::vector<species> &spc, std::string spc_id);

struct phase {
 public:
  std::string id;
  double C, Y, X, phi;
  properties P;
  flow_parameters F;
  std::vector<species> i, j, k;
};

struct flow {
 public:
  std::string id, def, cls, prop_data;
  std::string flow_db, thermo_file, trans_file, chem_file;
  std::vector<species> i, j, k, l;
  int n_i, n_j, n_k, n_l;
  std::string atom_def, molec_def, prox_def;
  properties P;
  flow_parameters F;
  phase ph[3];
  flow(std::string flw_def);
  flow(std::string flw_def, std::string flw_id);
  flow(){};
  void get_flow_data(std::string);
  void initialize_species(std::vector<species> &);
  void get_flow_species(std::vector<species> &, std::string);
  void get_flow_composition(std::vector<species> &, std::string);
  void get_flow_properties();
  void get_flow_chemistry();
  void interpret_molecules();
  void mix_flows(flow &, flow &);
  void flows_database(std::string);
  void get_species_data();
  void get_species_MW();
  void define_flow_prop_data();
  void calculate_flow(std::string);
  void calculate_flow_composition();
  void calculate_flow_properties(std::string);
  void calculate_species_properties(std::string);
  void calculate_flow_parameters();
  void calculate_solid_fuel();
  void calculate_gas_fuel();
  void calculate_properties();
  void calculate_gas_thermodynamics();
  void calculate_thermodynamic_properties();
  void calculate_MW();
  void print_flow();
};

bool find_flow(std::string input_def);

