#include "flow_definitions.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../../utils.h"

#include <mutex>

std::once_flag warn_flag_valences;

species::species(std::string sid) {
  id = sid;
  Y = 0.0;
  X = 0.0;
}

species::species(std::string sid, double sY) {
  id = sid;
  Y = sY;
  X = 0.0;
}

species::species(std::string sid, double val, std::string def) {
  id = sid;
  X = 0.0;
  if (def == "Y") {
    Y = val;
  }
  if (def == "X") {
    X = val;
  }
}

std::size_t index_species(std::vector<species>& spc, std::string spc_id) {
  for (std::size_t i = 0; i < spc.size(); i++) {
    if (spc[i].id == spc_id) {
      return i;
    }
  }
  // returns a negative number if species does not exist in the vector
  return static_cast<std::size_t>(-1);
}

flow::flow(std::string flw_def) {
  def = flw_def;
  F = flow_parameters();
  P = properties();
  F.T = 25.0;
  F.P = 1.01325;
  get_flow_data(flw_def);
  if (cls == "woody_biomass" || cls == "sludge") {
    calculate_solid_fuel();
  }
}

flow::flow(std::string flw_id, std::string flw_def) {
  id = flw_id;
  def = flw_def;
  F = flow_parameters();
  P = properties();
  F.T = 25.0;
  F.P = 1.01325;
  get_flow_data(flw_def);
  if (cls == "woody_biomass" || cls == "sludge") {
    calculate_solid_fuel();
  }
}

/**
 * @brief function to interpret the molecular composition of a flow
 */
void flow::interpret_molecules() {
  std::string molec_ID;
  std::string atom_ID;

  std::vector<std::string> atoms_ID;
  std::vector<int> atoms_N;
  std::string txt;
  int ctr_atom, ctr_atom1, ctr_atom2, ctr_molec, pos1, pos2;

  if (j.size() > 0) {
    for (std::size_t n = 0; n < j.size(); n++) {
      ctr_atom = 0;
      ctr_atom1 = 0;
      ctr_atom2 = 0;
      ctr_molec = 0;
      molec_ID = j[n].formula;
      j[n].P.MW = 0.0;
      pos1 = 0;
      pos2 = 0;
      while (pos1 < j[n].formula.length()) {
        if (molec_ID[pos1] == 'T' &&
            (molec_ID[pos1 + 1] == 'X' || molec_ID[pos1 + 1] == '-')) {
          pos1 = pos1 + 2;
        }
        if (molec_ID[pos1] == 'S' &&
            (molec_ID[pos1 + 1] == 'X' || molec_ID[pos1 + 1] == '-')) {
          pos1 = pos1 + 2;
        }
        if ((molec_ID[pos1] == '+' || molec_ID[pos1] == '-') &&
            (molec_ID[pos1 + 1] >= '1' && molec_ID[pos1 + 1] <= '9')) {
          break;
        }
        if ((molec_ID[pos1] == '+' || molec_ID[pos1] == '-') &&
            pos1 == j[n].formula.length() - 1) {
          break;
        }

        // finding a match in the atoms list
        for (std::size_t m = 0; m < i.size(); m++) {
          atom_ID = i[m].id;

          if (i[m].id.length() == 1 && ctr_atom2 == 0) {
            if (molec_ID[pos1] == atom_ID[0]) {
              ctr_atom1 = 1;
              atoms_ID.push_back(i[m].id);
            }
          }

          if (i[m].id.length() == 2) {
            if (pos1 + 1 <= j[n].formula.length() - 1 &&
                molec_ID[pos1 + 1] == atom_ID[1] &&
                molec_ID[pos1] == atom_ID[0]) {
              ctr_atom2 = 1;
              pos1 = pos1 + 1;
              if (ctr_atom1 == 0) {
                atoms_ID.push_back(i[m].id);
              }
              if (ctr_atom1 == 1) {
                atoms_ID[atoms_ID.size() - 1] = i[m].id;
                ctr_atom1 = 0;
              }
            }
          }
        }

        if (ctr_atom1 == 1 || ctr_atom2 == 1) {
          ctr_atom1 = 0;
          ctr_atom2 = 0;
          if (pos1 == j[n].formula.length() - 1) {
            atoms_N.push_back(1);
            ctr_molec = 1;
          }

          if ((pos1 < j[n].formula.length() - 1) &&
              !(molec_ID[pos1 + 1] >= '0' && molec_ID[pos1 + 1] <= '9')) {
            atoms_N.push_back(1);
            pos2 = pos1 + 1;
          }

          if ((pos1 < j[n].formula.length() - 1) &&
              (molec_ID[pos1 + 1] >= '0' && molec_ID[pos1 + 1] <= '9')) {
            if ((pos1 + 1) == (j[n].formula.length() - 1)) {
              ctr_molec = 1;
              atoms_N.push_back(molec_ID[pos1 + 1] - '0');
            }
            if ((pos1 + 1) < (j[n].formula.length() - 1)) {
              int v = 0;
              while (molec_ID[pos1 + 1] >= '0' && molec_ID[pos1 + 1] <= '9') {
                v = 10 * v + molec_ID[pos1 + 1] - '0';
                if ((pos1 + 1) == j[n].formula.length() - 1) {
                  ctr_molec = 1;
                  break;
                }
                pos1 = pos1 + 1;
              }
              atoms_N.push_back(v);
              pos2 = pos1 + 1;
            }
          }
          pos1 = pos2;
        }

        if (ctr_molec == 1) {
          ctr_molec = 0;
          break;
        }
      }

      std::size_t index;

      double sum_N = 0;
      double sum_M = 0;
      for (std::size_t ni = 0; ni < atoms_ID.size(); ni++) {
        index = index_species(i, atoms_ID[ni]);
        sum_N = sum_N + atoms_N[ni];
        sum_M = sum_M + atoms_N[ni] * i[index].P.MW;
        i[index].X = 0;
        i[index].Y = 0;  // initialize
      }

      for (std::size_t ni = 0; ni < atoms_ID.size(); ni++) {
        index = index_species(i, atoms_ID[ni]);
        if (molec_def == "X") {
          i[index].X = i[index].X + j[n].X * atoms_N[ni] / sum_N;
        }
        if (molec_def == "Y") {
          i[index].Y =
              i[index].Y + j[n].Y * atoms_N[ni] * i[index].P.MW / sum_M;
        }
      }

      for (std::size_t ni = 0; ni < atoms_ID.size(); ni++) {
        index = index_species(i, atoms_ID[ni]);
        j[n].P.MW = j[n].P.MW + atoms_N[ni] * i[index].P.MW;
      }

      double sum_Y_MW = 0.0;
      for (std::size_t ni = 0; ni < i.size(); ni++) {
        sum_Y_MW += i[ni].Y / i[ni].P.MW;
      }
      if (molec_def == "Y") {
        for (std::size_t ni = 0; ni < i.size(); ni++) {
          i[ni].X = (i[ni].Y / i[ni].P.MW) / sum_Y_MW;
        }
      }

      atoms_ID.erase(atoms_ID.begin(), atoms_ID.begin() + atoms_ID.size());
      atoms_N.erase(atoms_N.begin(), atoms_N.begin() + atoms_N.size());
    }
  }
}

void flow::initialize_species(std::vector<species>& spc) {
  size_t n_spc = spc.size();
  if (n_spc > 0)
    for (std::size_t ns = 0; ns < n_spc; ns++) {
      {
        spc[ns].Y = 0;
        spc[ns].X = 0;
        spc[ns].P = properties();
        spc[ns].F = flow_parameters();
      }
    }
}

void species::get_species_data_(std::string spc_type) {
  P = properties();
  F = flow_parameters();
  std::string txt, line_txt, symb;
  int n;
  double num;
  bool input;

  std::ifstream db;

  if (spc_type == "molecule") {
    try {
      toml::table tbl =
          get_toml_table(get_database_path("Flows_database/Molecules_db.toml"));

      auto mol_table = tbl["molecules"][id];
      if (!mol_table) throw std::runtime_error("Molecule " + id + " not found");

      // Required fields
      if (auto comp = mol_table["composition"].value<std::string>())
        formula = *comp;
      else
        throw std::runtime_error("Molecule " + id +
                                 " missing required field 'composition'");

      if (auto mw = mol_table["MW"].value<double>())
        P.MW = *mw;
      else
        throw std::runtime_error("Molecule " + id +
                                 " missing required field 'MW'");

      // Optional fields
      if (auto rho = mol_table["rho_m"].value<double>()) P.rho = *rho;
      if (auto hf = mol_table["hf"].value<double>()) P.hf = *hf;
      if (auto hf = mol_table["hf_n"].value<double>()) P.hf = *hf / P.MW;
      if (auto cp = mol_table["cp_m"].value<double>()) P.cp = *cp;
      if (auto cp = mol_table["cp_n"].value<double>()) P.cp = *cp / P.MW;
      if (auto s = mol_table["s_m"].value<double>()) P.s = *s;
      if (auto s = mol_table["s_n"].value<double>()) P.s = *s / P.MW;
      if (auto k = mol_table["k"].value<double>()) P.k = *k;
      if (auto visc = mol_table["visc"].value<double>()) P.visc = *visc;
      if (auto hhv = mol_table["HHV"].value<double>()) P.HHV = *hhv;
      if (auto lhv = mol_table["LHV"].value<double>()) P.LHV = *lhv;
      if (auto Tsat = mol_table["Tsat"].value<double>()) P.Tsat = *Tsat;
      if (auto hv = mol_table["hv"].value<double>()) P.hVap = *hv;
    } catch (const toml::parse_error& err) {
      throw std::runtime_error(std::string("TOML parse error: ") + err.what());
    }
  } else if (spc_type == "atom") {
    try {
      toml::table tbl =
          get_toml_table(get_database_path("Flows_database/atoms_db.toml"));

      auto atom_table = tbl["atoms"][id];
      if (!atom_table) {
        throw std::runtime_error("Atom " + id + " not found");
      }

      if (auto mw = atom_table["atomic_weight"].value<double>()) P.MW = *mw;

      if (auto vals = atom_table["valences"].as_array()) {
        // for (auto&& v : *vals) {
        //   if (auto n = v.value<int>())
        //     val.push_back(*n);
        // }
        std::call_once(warn_flag_valences, [] {
          std::cout << "Redundantly added empty parameter.\n";
        });
      }
    } catch (const toml::parse_error& err) {
      throw std::runtime_error(std::string("TOML parse error: ") + err.what());
    }
  }
}

void flow::get_flow_composition(std::vector<species>& spc, std::string input) {
  std::string file_path = get_database_path(flow_db);
  toml::table tbl = get_toml_table(file_path);

  auto flow_tbl = tbl["flow"][def];
  if (!flow_tbl || !flow_tbl.is_table())
    throw std::runtime_error("flow def " + def + " not found in " + file_path);

  auto section_node = flow_tbl[input];
  if (!section_node || !section_node.is_table()) return;

  toml::table section = *section_node.as_table();

  for (auto&& [key, val] : section) {
    std::string name = std::string(key.str());
    std::string x_or_y;

    if (val.is_table() && !val.as_table()->empty()) {
      // inline table like {Y = 0.523} or {X = 0.7812}
      auto& subtbl = *val.as_table();
      double number = 0.0;
      for (auto&& [subkey, subval] : subtbl) {
        x_or_y = std::string(subkey.str());  // e.g. "Y" or "X"
        if (auto num = subval.value<double>()) number = *num;
      }
      spc.push_back(species(name, number, x_or_y));

      if (input == "molecules")
        molec_def = x_or_y;
      else if (input == "atoms")
        atom_def = x_or_y;
      else if (input == "proximate")
        prox_def = x_or_y;
    } else
      spc.push_back(species(name));

    if (input == "molecules")
      spc.back().get_species_data_("molecule");
    else if (input == "atoms")
      spc.back().get_species_data_("atom");

    for (int m = 0; m < 3; m++) {
      if (input == "atoms") {
        ph[m].i.push_back(spc.back());
      } else if (input == "molecules") {
        ph[m].j.push_back(spc.back());
      } else if (input == "proximate") {
        ph[m].k.push_back(spc.back());
      }
    }
  }
}

void flow::get_flow_properties() {
  try {
    std::string file_path = get_database_path(flow_db);
    toml::table tbl = get_toml_table(file_path);

    auto props = tbl["flow"][def]["properties"];
    if (!props || !props.is_table()) return;

    if (auto v = props["MW"].value<double>()) P.MW = *v;
    if (auto v = props["rho_m"].value<double>()) P.rho = *v;
    if (auto v = props["cp_m"].value<double>()) P.cp = *v;
    if (auto v = props["k"].value<double>()) P.k = *v;
    if (auto v = props["visc"].value<double>()) P.visc = *v;
    if (auto v = props["HHV"].value<double>()) P.HHV = *v;
    if (auto v = props["LHV"].value<double>()) P.LHV = *v;
    if (auto v = props["HHV_dry"].value<double>()) P.HHV_dry = *v;
    if (auto v = props["LHV_dry"].value<double>()) P.LHV_dry = *v;
    if (auto v = props["Tsat"].value<double>()) P.Tsat = *v;
    if (auto v = props["hv"].value<double>()) P.hVap = *v;

    if (P.LHV == 0.0 && P.HHV > 0) {
      size_t H = index_species(i, "H");
      P.LHV = P.HHV - i[H].Y * (18 / 2) * 2.5;
    }
  } catch (const toml::parse_error& err) {
    throw std::runtime_error(std::string("TOML parse error: ") + err.what());
  }
}

void flow::get_flow_data(std::string input_def) {
  def = input_def;

  std::string file_path = get_database_path("Flows_database/Flow_list.toml");
  toml::table tbl = get_toml_table(file_path);

  auto flow_group = tbl["flow"][input_def];
  if (!flow_group || !flow_group.is_table())
    throw std::runtime_error("Flow " + input_def + " not found in " +
                             file_path);

  if (auto cls_val = flow_group["cls"].value<std::string>())
    cls = *cls_val;
  else
    throw std::runtime_error("cls missing for " + input_def);

  if (auto db_val = flow_group["flow_db"].value<std::string>())
    flow_db = *db_val;
  else
    throw std::runtime_error("flow_db missing for " + input_def);

  try {
    file_path = get_database_path(flow_db);
    tbl = get_toml_table(file_path);

    auto flow_tbl = tbl["flow"][input_def];
    if (!flow_tbl || !flow_tbl.is_table())
      throw std::runtime_error(input_def + " not found in " + file_path);

    if (auto v = flow_tbl["class"].value<std::string>())
      cls = *v;
    else
      throw std::runtime_error("Flow_cls missing for " + input_def);

    if (auto v = flow_tbl["prop_data"].value<std::string>())
      prop_data = *v;
    else
      throw std::runtime_error("prop_data missing for " + input_def);

  } catch (const toml::parse_error& err) {
    throw std::runtime_error("TOML parse error in " + file_path + ": " +
                             std::string(err.description()));
  }

  if (prop_data != "IDEAL_GAS") {
    get_flow_composition(j, "molecules");
    get_flow_composition(i, "atoms");
    interpret_molecules();
    get_flow_composition(k, "proximate");
    get_flow_properties();
  }

  calculate_flow_composition();
}

/**
 * @brief function to print the parameters of a flow
 *
 */
void flow::print_flow() {
  std::cout << "------ Flow -------------- " << std::endl;
  std::cout << "id: " << id << std::endl;
  std::cout << "def: " << def << std::endl;
  std::cout << "cls: " << cls << std::endl;
  std::cout << "prop_data: " << prop_data << std::endl;
  std::cout << "M: " << F.M << std::endl;
  std::cout << "T: " << F.T << std::endl;
  std::cout << "P: " << F.P << std::endl;
  std::cout << "LHV dry: " << P.LHV_dry << std::endl;
  std::cout << "LHV: " << P.LHV << std::endl;
  std::cout << "MW: " << P.MW << std::endl;
  std::cout << "cp: " << P.cp << std::endl;
  std::cout << "rho: " << P.rho << std::endl;
  std::cout << "h: " << P.h << std::endl;
  std::cout << "ht: " << P.ht << std::endl;
  std::cout << "s: " << P.s << std::endl;
  std::cout << "H : " << F.H << std::endl;
  std::cout << "Ht : " << F.Ht << std::endl;
  std::cout << "-------------------- " << std::endl;
  if (i.size() == 0) {
    std::cout << " atomic composition is not defined" << std::endl;
  }
  if (i.size() > 0) {
    std::cout << "Atomic composition: " << std::endl;
    std::cout << "------------------- " << std::endl;
    for (std::size_t n = 0; n < i.size(); n++) {
      std::cout << i[n].id << " X: " << i[n].X << " Y: " << i[n].Y << std::endl;
    }
  }
  std::cout << "-------------------- " << std::endl;
  if (j.size() == 0) {
    std::cout << " molecular composition is not defined" << std::endl;
  }
  if (j.size() > 0) {
    std::cout << "Molecular composition: " << std::endl;
    std::cout << "------------------- " << std::endl;
    for (std::size_t n = 0; n < j.size(); n++) {
      std::cout << j[n].id << " MW: " << j[n].P.MW << " X: " << j[n].X
                << " Y: " << j[n].Y << " cp: " << j[n].P.cp
                << " ht: " << j[n].P.ht << " h: " << j[n].P.h
                << " s: " << j[n].P.s << std::endl;
    }
  }
  std::cout << "-------------------- " << std::endl;
  if (k.size() == 0) {
    std::cout << " proximate composition is not defined" << std::endl;
  }
  if (k.size() > 0) {
    std::cout << "Proximate composition: " << std::endl;
    std::cout << "------------------- " << std::endl;
    for (std::size_t n = 0; n < k.size(); n++) {
      std::cout << k[n].id << " X: " << k[n].X << " Y: " << k[n].Y << std::endl;
    }
  }
  std::cout << "-------------------- " << std::endl;
}

bool find_flow(std::string input_def) {
  std::string file_path = get_database_path("Flows_database/Flow_list.toml");
  toml::table tbl = get_toml_table(file_path);

  auto flow_group = tbl["flow"][input_def];
  if (!flow_group || !flow_group.is_table())
    throw std::runtime_error(input_def + " not found in " + file_path);

  if (!flow_group["cls"].value<std::string>())
    throw std::runtime_error("cls missing for flow " + input_def + " in " +
                             file_path);

  std::string flow_db;
  if (auto v = flow_group["flow_db"].value<std::string>())
    flow_db = *v;
  else
    throw std::runtime_error("flow_db missing for flow " + input_def + " in " +
                             file_path);

  try {
    file_path = get_database_path(flow_db);
    toml::table tbl = get_toml_table(file_path);

    auto flow_tbl = tbl["flow"][input_def];
    if (!flow_tbl || !flow_tbl.is_table())
      throw std::runtime_error(input_def + " not found in " + file_path);

    return true;

  } catch (const toml::parse_error& err) {
    throw std::runtime_error("TOML parse error in " + file_path + ": " +
                             std::string(err.description()));
  }

  return false;
}
