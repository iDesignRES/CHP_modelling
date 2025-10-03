#pragma once

#include <string>
#include <vector>

/**
 * @struct properties
 * @brief Structure to define flow or species properties
 * @var properties::LHV
 * Low Heating Value (MJ/kg)
 * @var properties::LHV_dry
 * Low Heating Value dry basis (MJ/kg)
 * @var properties::HHV
 * High Heating Value (MJ/kg)
 * @var properties::HHV_dry
 * High Heating Value dry basis (MJ/kg)
 * @var properties::cp
 * Specific molar heat at constant pressure (J/kg K)
 * @var properties::rho
 * Density (kg/m3)
 * @var properties::MW
 * Molecular weight (kg/mol)
 * @var properties::h
 * Specific enthalpy (J/kg)
 * @var properties::hf
 * Specific formation enthalpy (J/kg)
 * @var properties::ht
 * Specific thermal enthalpy (J/kg)
 * @var properties::s
 * Specific entropy (J/kgK)
 * @var properties::Tsat
 * Saturation temperature (deg.C)
 * @var properties::q
 * Vapor composition (kg/kg)
 * @var properties::k
 * Thermal conductivity (W/m*K)
 * @var properties::visc
 * Viscosity (Pa*s)
 * @var properties::hVap
 * Vaporization heat (J/kg)
 */
struct properties {
 public:
  double LHV = 0.0, LHV_dry = 0.0, HHV = 0.0, HHV_dry = 0.0;
  double cp = 0.0, rho = 0.0, MW = 0.0;
  double hf = 0.0, ht = 0.0, h = 0.0;
  double s = 0.0;
  double Tsat = 0.0, q = 0.0, hVap = 0.0;
  double k = 0.0, visc = 0.0;
};

/**
 * @struct flow_parameters
 * @brief Structure to define flow parameters
 * @var flow_parameters::P
 * Pressure (bar)
 * @var flow_parameters::T
 * Temperature (deg.C)
 * @var flow_parameters::M
 * Mass flow rate (kg/s)
 * @var flow_parameters::N
 * Molar flow rate (mol/s)
 * @var flow_parameters::VN
 * Standard volumetric flow rate (Nm3/s)
 * @var flow_parameters::V
 * Volumetric flow rate (m3/s)
 * @var flow_parameters::H
 * Enthalpy flow (J/s)
 * @var flow_parameters::Hf
 * Formation enthalpy flow (J/s)
 * @var flow_parameters::Ht
 * Thermal enthalpy flow (J/s)
 */
struct flow_parameters {
 public:
  double P = 0.0, T = 0.0;
  double M = 0.0, N = 0.0, VN = 0.0, V = 0.0;
  double H = 0.0, Ht = 0.0, Hf = 0.0;
};

/**
 * @struct species
 * @brief Structure to define species parameters
 * @var species::id
 * Name of the species (e.g., vapor)
 * @var species::def
 * Definition in database (e.g., water)
 * @var species::formula
 * Chemical formula (e.g., H2O)
 * @var species::Y
 * Mass fraction (kg/kg)
 * @var species::X
 * Molar fraction (mol/mol)
 * @var species::val
 * Valences for atoms
 * @var species::P
 * Properties of the species
 * @var species::F
 * Flow parameters of the species
 */
struct species {
 public:
  std::string id = "", def = "", formula = "";
  double Y = 0.0, X = 0.0;
  std::vector<double> val;
  properties P;
  flow_parameters F;
  species() {}
  species(std::string sid);
  species(std::string sid, double sY);
  species(std::string sid, double val, std::string def);
  void calculate_refprop(std::string);
  void get_species_data_(std::string);
  void calculate_thermodynamic_properties();
};

std::size_t index_species(std::vector<species> &spc, std::string spc_id);

/**
 * @struct phase
 * @brief Structure to define the parameters of one flow phase
 * @var phase::id
 * Phase identifier
 * @var phase::C
 * Molar concentration (mol/l)
 * @var phase::Y
 * Mass fraction (kg/kg)
 * @var phase::X
 * Molar fraction (mol/mol)
 * @var phase::phi
 * Volume fraction (m3/m3)
 * @var phase::P
 * Properties of the phase
 * @var phase::F
 * Flow parameters of the phase
 * @var phase::i
 * Atoms vector
 * @var phase::j
 * Molecules vector
 * @var phase::k
 * Proximate composition vector
 */
struct phase {
 public:
  std::string id = "";
  double C = 0.0, Y = 0.0, X = 0.0, phi = 0.0;
  properties P;
  flow_parameters F;
  std::vector<species> i, j, k;
  phase() {}
};

/**
 * @struct flow
 * @brief Structure to define the parameters of one flow
 * @var flow::id
 * Name of the flow (e.g., wood)
 * @var flow::def
 * Definition in database (e.g., spruce_chips)
 * @var flow::cls
 * Flow class (e.g., hardwood)
 * @var flow::prop_data
 * Type of properties (e.g., solid_fuel)
 * @var flow::flow_db
 * File with flow data in database
 * @var flow::i
 * Atoms vector
 * @var flow::j
 * Molecules vector
 * @var flow::k
 * Proximate composition vector
 * @var flow::l
 * Constituent vector
 * @var flow::atom_def
 * Type of atomic composition ("Y" mass fraction, "X" mol fraction)
 * @var flow::molec_def
 * Type of molecular composition ("Y" mass fraction, "X" mol fraction)
 * @var flow::prox_def
 * Type of proximate composition ("Y" mass fraction, "X" mol fraction)
 * @var flow::P
 * Properties of the flow
 * @var flow::F
 * Flow parameters
 * @var flow::ph
 * Phase vector: 0=solid, 1=liquid, 2=gas
 */
struct flow {
 public:
  std::string id = "", def = "", cls = "";
  std::string prop_data = "", flow_db = "";
  std::vector<species> i, j, k, l;  // i=atoms,j=molec,k=prox,l=const
  std::string atom_def = "", molec_def = "", prox_def = "";
  properties P;
  flow_parameters F;
  phase ph[3];
  flow(std::string flw_def);
  flow(std::string flw_def, std::string flw_id);
  flow() {}
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
