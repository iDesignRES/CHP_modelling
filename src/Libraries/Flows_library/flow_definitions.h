#pragma once

#include <vector>
#include <string>

/**
 * @brief Structure to define flow or species properties
 *
 * @param "LHV" = Low Heating Value (MJ/kg)
 * @param "LHV_dry" = Low Heating Value dry basis (MJ/kg)
 * @param "HHV" = High Heating Value (MJ/kg)
 * @param "HHV_dry" = High Heating Value dry basis (MJ/kg)
 * @param "cp" = specific molar heat at constant pressure (J/kg K)
 * @param "rho" = density (kg/m3)
 * @param "MW" = molecular weight (kg/mol)
 * @param "h" = specific enthalpy (J/kg)
 * @param "hf" = specific formation enthalpy (J/kg)
 * @param "ht" = specific thermal enthalpy (J/kg)
 * @param "s" = specific entropy (J/kgK)
 * @param "Tsat" = saturation temperature (deg.C)
 * @param "q" = vapor compostion (kg/kg)
 * @param "k" = thermal conductivity (W/m*K)
 * @param "visc" = viscosity (Pa*s)
 * @param "hVap" = vaporization heat (J/kg)
 */
struct properties {
 public:
  double LHV = 0.0, LHV_dry, HHV = 0.0, HHV_dry;
  double cp, rho, MW;
  double hf, ht, h;
  double s;
  double Tsat, q, hVap;
  double k, visc;
};

/**
 * @brief Structure to define flow parameters
 *
 * @param "P" = pressure (bar)
 * @param "T" = temperature (deg.C)
 * @param "M" = mass flow rate (kg/s)
 * @param "N" = molar flow rate (mol/s)
 * @param "VN" = standard volumetric flow rate (Nm3/s)
 * @param "V" = volumetric flow rate (m3/s)
 * @param "H" = Enthalpy flow (J/s)
 * @param "Hf" = Formation enthalpy flow (J/s)
 * @param "Ht" = Thermal enthalpy flow (J/s)
 */
struct flow_parameters {
 public:
  double P, T;
  double M, N, VN, V;
  double H, Ht, Hf;
};

/**
 * @brief Structure to define species parameters
 *
 * @param string "id" = name: Ex. vapor
 * @param string "def" = definition in database: Ex. water
 * @param string "formula" = chemical formula: Ex. H2O
 * @param "Y" = mass fraction (kg/kg)
 * @param "X" = molar fraction (mol/mol)
 * @param "val" = valences for atoms
 */
struct species {
 public:
  std::string id, def, formula;
  double Y, X;
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

/**
 * @brief Structure to define the parameters of one flow phase
 *
 * @param "Y" = mass fraction (kg/kg)
 * @param "X" = molar fraction (mol/mol)
 * @param "C" = molar concentration (mol/l)
 * @param "phi" = volume fraction (m3/m3)
 * @param "i" = atoms vector
 * @param "j" = molecules vector
 * @param "k" = proximate composition vector
 */
struct phase {
 public:
  std::string id;
  double C, Y, X, phi;
  properties P;
  flow_parameters F;
  std::vector<species> i, j, k;
};

/**
 * @brief Structure to define the parameters of one flow
 *
 * @param string "id" = name: Ex. wood
 * @param string "def" = definition in database: Ex. spruce_chips
 * @param string "cls" = flow class: Ex. hardwood
 * @param string "flow_db" = file with flow data in database
 * @param string "prop_data" = type of properties: Ex. solid_fuel
 * @param "Y" = mass fraction (kg/kg)
 * @param "X" = molar fraction (mol/mol)
 * @param "C" = molar concentration (mol/l)
 * @param "phi" = volume fraction (m3/m3)
 * @param "i" = atoms vector
 * @param "j" = molecules vector
 * @param "k" = proximate composition vector
 * @param "ph[3]" = phase vector: 1.solid, 2.liquid, 3.gas
 * @param string "atom_def" = type of atomic composition: "Y" mass fraction, "X"
 * mol fraction
 * @param string "molec_def" = type of molecular composition: "Y" mass fraction,
 * "X" mol fraction
 * @param string "prox_def" = type of proximate composition: "Y" mass fraction,
 * "X" mol fraction
 */
struct flow {
 public:
  std::string id, def, cls;
  std::string prop_data, flow_db;
  std::vector<species> i, j, k, l;
  std::string atom_def, molec_def, prox_def;  // i=atoms,j=molec,k=prox,l=const
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
  void calculate_properties();
  void calculate_thermodynamic_properties();
  void calculate_MW();
  void print_flow();
};

/**
 * @brief Boolean function to find out if a flow exists in the database
 *
 * @param input_def string with the name of the flow in the database
 * @return true if found, false otherwise
 */
bool find_flow(std::string input_def);

