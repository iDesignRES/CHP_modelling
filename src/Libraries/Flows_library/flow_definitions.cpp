#include "flow_definitions.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>

#include "../../utils.h"

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

std::size_t index_species(std::vector<species> &spc, std::string spc_id) {
  for (std::size_t i = 0; i < spc.size(); i++) {
    if (spc[i].id == spc_id) {
      return i;
    }
  }
  return static_cast<std::size_t>(-1);  // returns a negative number if species does not exist in the vector
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

  // std::cout << "Molecule interpreter. No. molecules: " << j.size() << std::endl;

  if (j.size() > 0) {
    for (std::size_t n = 0; n < j.size(); n++) {
      // std::cout << "molecule formula: " << j[n].formula << std::endl;
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
                molec_ID[pos1 + 1] == atom_ID[1] && molec_ID[pos1] == atom_ID[0]) {
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

void flow::initialize_species(std::vector<species> &spc) {
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

  // std::cout << "getting data for species: " << id << std::endl;
  std::ifstream db;

  if (spc_type == "molecule") {
    db.open(get_database_path("Flows_database/Molecules_db.txt"));
    if (!db.good()) {
      std::cout << "molecule file not found" << std::endl;
      db.close();
      return;
    }

    bool molecule_found = false;
    while (molecule_found == false) {
      getline(db, line_txt);
      std::stringstream sst(line_txt);
      sst >> txt;
      if (txt == "Species_id") {
        while (sst >> txt) {
          if (txt == id) {
            molecule_found = true;
            break;
          }
        }
      }
      if (db.eof()) {
        break;
      }
    }
    if (molecule_found == false) {
      std::cout << "molecule " << id << " not found" << std::endl;
      db.close();
      return;
    }

    db >> txt;
    if (txt == "Composition:") {
      db >> formula;
    }
    db >> txt;
    if (txt == "inputs") {
      db >> n;

      if (n > 0) {
        input = true;
        for (int i = 0; i < n; i++) {
          db >> symb;
          db >> num;
          if (symb == "MW") {
            P.MW = num;
          }
          if (symb == "hf") {
            P.hf = num;
          }
          if (symb == "rho_m") {
            P.rho = num;
          }
          if (symb == "cp_m") {
            P.cp = num;
          }
          if (symb == "k") {
            P.k = num;
          }
          if (symb == "visc") {
            P.visc = num;
          }
          if (symb == "HHV") {
            P.HHV = num;
          }
          if (symb == "LHV") {
            P.LHV = num;
          }
          if (symb == "Tsat") {
            P.Tsat = num;
          }
          if (symb == "hv") {
            P.hVap = num;
          }
        }
      } else {
        input = false;
      }
    }

    db.close();
  }

  if (spc_type == "atom") {
    db.open(get_database_path("Flows_database/atoms_db.txt"));
    if (!db.good()) {
      std::cout << "atom_file not found" << std::endl;
      db.close();
      return;
    }

    bool atom_found = false;
    while (atom_found == false) {
      getline(db, line_txt);
      std::stringstream sst(line_txt);
      sst >> txt;
      if (txt == id) {
        sst >> txt;
        P.MW = atof(txt.c_str());
        if (getline(sst, txt, ' ') && txt == "valences:") {
          while (sst >> txt) {
            val.push_back(atof(txt.c_str()));
          }
        }
        atom_found = true;
        break;
      }
      if (db.eof()) {
        break;
      }
    }
    if (atom_found == false) {
      std::cout << "atom id " << id << " not found in the database" << std::endl;
      db.close();
      return;
    }

    db.close();
  }
}

void flow::get_flow_composition(std::vector<species> &spc, std::string input) {
  std::string txt, symb, val, line_txt;
  int n_spc;
  species sp;

  std::ifstream flow_file;
  flow_file.open(flow_db);

  if (!flow_file.good()) {
    std::cout << "flow_file problem" << std::endl;
  }
  if (flow_file.good()) {
    bool flow_found = false;
    while (flow_found == false) {
      getline(flow_file, line_txt);
      std::stringstream sst(line_txt);
      sst >> txt;
      if (txt == "Flow_def" || txt == "Flow_def:" || txt == "flow_def" ||
          txt == "flow_def:") {
        while (sst >> txt) {
          if (txt == def) {
            flow_found = true;
            break;
          }
        }
      }
      if (flow_file.eof()) {
        break;
      }
    }
    if (flow_found == false) {
      std::cout << "flow def not found" << std::endl;
      flow_file.close();
      return;
    }

    while (!flow_file.eof()) {
      getline(flow_file, line_txt);
      std::stringstream sst(line_txt);
      sst >> txt;
      if (txt == input) {
        break;
      }
      if (txt == "END") {
        flow_file.close();
        n_spc = 0;
        return;
      }
    }

    getline(flow_file, line_txt);
    std::stringstream sst(line_txt);
    sst >> txt;
    sst >> txt;
    n_spc = atoi(txt.c_str());
    if (n_spc > 0) {
      for (int n = 0; n < n_spc; n++) {
        getline(flow_file, line_txt);
        std::stringstream sst2(line_txt);
        sst2 >> txt;
        if (sst2 >> symb) {
          sst2 >> val;
          spc.push_back(species(txt, atof(val.c_str()), symb));
          if (symb == "Y" && input == "MOLECULES") {
            molec_def = "Y";
          }
          if (symb == "X" && input == "MOLECULES") {
            molec_def = "X";
          }
          if (symb == "Y" && input == "ATOMS") {
            atom_def = "Y";
          }
          if (symb == "X" && input == "ATOMS") {
            atom_def = "X";
          }
          if (symb == "Y" && input == "PROXIMATE") {
            prox_def = "Y";
          }
          if (symb == "X" && input == "PROXIMATE") {
            prox_def = "X";
          }

        } else {
          spc.push_back(species(txt));
        }

        if (input == "MOLECULES") {
          spc[spc.size() - 1].get_species_data_("molecule");
        }
        if (input == "ATOMS") {
          spc[spc.size() - 1].get_species_data_("atom");
        }
        for (int m = 0; m < 3; m++) {
          if (input == "ATOMS") {
            ph[m].i.push_back(spc[spc.size() - 1]);
          }
          if (input == "MOLECULES") {
            ph[m].j.push_back(spc[spc.size() - 1]);
          }
          if (input == "PROXIMATE") {
            ph[m].k.push_back(spc[spc.size() - 1]);
          }
        }
      }
    }

    flow_file.close();
  }
}

void flow::get_flow_properties() {
  std::string txt, line_txt, symb;
  double val;
  int n;

  std::ifstream flow_file;
  flow_file.open(flow_db);

  if (!flow_file.good()) {
    std::cout << "flow_file problem" << std::endl;
    flow_file.close();
    return;
  }
  if (flow_file.good()) {
    bool flow_found = false;
    while (flow_found == false) {
      getline(flow_file, line_txt);
      std::stringstream sst(line_txt);
      sst >> txt;
      if (txt == "Flow_def" || txt == "Flow_def:" || txt == "flow_def" ||
          txt == "flow_def:") {
        while (sst >> txt) {
          if (txt == def) {
            flow_found = true;
            break;
          }
        }
      }
      if (flow_file.eof()) {
        break;
      }
    }
    if (flow_found == false) {
      std::cout << "flow def not found" << std::endl;
      flow_file.close();
      return;
    }

    while (!flow_file.eof()) {
      flow_file >> txt;
      if (txt == "PROPERTIES") {
        break;
      }
      if (txt == "END") {
        flow_file.close();
        return;
      }
    }

    flow_file >> txt;
    flow_file >> n;
    if (n > 0) {
      for (int np = 0; np < n; np++) {
        flow_file >> symb;
        flow_file >> val;
        if (symb == "MW") {
          P.MW = val;
        }
        if (symb == "rho_m") {
          P.rho = val;
        }
        if (symb == "cp_m") {
          P.cp = val;
        }
        if (symb == "k") {
          P.k = val;
        }
        if (symb == "visc") {
          P.visc = val;
        }
        if (symb == "HHV") {
          P.HHV = val;
        }
        if (symb == "LHV") {
          P.LHV = val;
        }
        if (symb == "HHV_dry") {
          P.HHV_dry = val;
        }
        if (symb == "LHV_dry") {
          P.LHV_dry = val;
        }
        if (symb == "Tsat") {
          P.Tsat = val;
        }
        if (symb == "hv") {
          P.hVap = val;
        }
      }
    }

    flow_file.close();

    if (P.LHV == 0.0 && P.HHV > 0) {
      size_t H = index_species(i, "H");
      P.LHV = P.HHV - i[H].Y * (18 / 2) * 2.5;
    }
  }
}

void flow::get_flow_data(std::string input_def) {
  std::string line_txt, txt, symb;
  species spc;

  def = input_def;

  std::ifstream db_file;

  // Get the directory of the current source file
  db_file.open(get_database_path("Flows_database/Flow_list.txt"));

  if (!db_file.good()) {
    std::cout << "flow_list file not found" << std::endl;
    db_file.close();
    return;
  }
  if (db_file.good()) {
    bool flow_found = false;
    while (flow_found == false) {
      getline(db_file, line_txt);
      std::stringstream sst(line_txt);
      sst >> txt;
      if (txt == input_def) {
        flow_found = true;
        sst >> cls;
        sst >> flow_db;
        break;
      }
      if (db_file.eof()) {
        break;
      }
    }

    if (flow_found == false) {
      std::cout << "flow is not in the list" << std::endl;
      db_file.close();
      return;
    }
    db_file.close();
  }

  flow_db = get_database_path(flow_db);

  std::ifstream db;
  db.open(flow_db);
  if (!db.good()) {
    std::cout << "flow_db file problem" << std::endl;
    db.close();
    return;
  }
  if (db.good()) {
    bool flow_found = false;
    while (flow_found == false) {
      getline(db, line_txt);
      std::stringstream sst(line_txt);
      sst >> txt;
      if (txt == "Flow_def" || txt == "Flow_def:" || txt == "flow_def" ||
          txt == "flow_def:") {
        while (sst >> txt) {
          if (txt == input_def) {
            flow_found = true;
            break;
          }
        }
      }
      if (db.eof()) {
        break;
      }
    }
    if (flow_found == false) {
      std::cout << input_def + " not found in " + flow_db << std::endl;
      db.close();
      return;
    }

    bool flow_cls_defined = false;

    while (flow_cls_defined == false) {
      getline(db, line_txt);
      std::stringstream sst(line_txt);
      getline(sst, txt, ' ');
      if (txt == "Flow_cls" || txt == "Flow_cls:" || txt == "flow_cls" ||
          txt == "flow_cls:") {
        getline(sst, cls, ' ');
        flow_cls_defined = true;
      }
    }

    bool prop_data_defined = false;

    while (prop_data_defined == false) {
      getline(db, line_txt);
      std::stringstream sst(line_txt);
      sst >> txt;
      if (txt == "Prop_data" || txt == "Prop_data:" || txt == "prop_data" ||
          txt == "prop_data:") {
        sst >> prop_data;
        prop_data_defined = true;
      }
    }

    db.close();
  }

  if (prop_data != "IDEAL_GAS") {
    get_flow_composition(j, "MOLECULES");
    get_flow_composition(i, "ATOMS");
    interpret_molecules();
    get_flow_composition(k, "PROXIMATE");
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
           << " Y: " << j[n].Y << " cp: " << j[n].P.cp << " ht: " << j[n].P.ht
           << " h: " << j[n].P.h << " s: " << j[n].P.s << std::endl;
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
  std::ifstream db_file;

  std::string line_txt, txt, flow_cls, flow_db,
      error = input_def + " not found in the database Database/";

  db_file.open(get_database_path("Flows_database/Flow_list.txt"));

  if (!db_file.good()) {
    std::cout << "Flow list not found" << std::endl;
    db_file.close();
  }
  if (db_file.good()) {
    bool flow_found = false;
    while (flow_found == false) {
      getline(db_file, line_txt);
      std::stringstream sst(line_txt);
      sst >> txt;
      if (txt == input_def) {
        flow_found = true;
        sst >> flow_cls;
        sst >> flow_db;
        break;
      }
      if (db_file.eof()) {
        break;
      }
    }

    if (flow_found == false) {
      std::cout << input_def + " not found in Flow_list" << std::endl;
      db_file.close();
      return false;
    }
  }

  db_file.close();
  std::ifstream db;
  db.open(get_database_path(flow_db));
  if (!db.good()) {
    std::cout << "Database/" + flow_db + " not found" << std::endl;
    db.close();
    return false;
  }
  if (db.good()) {
    bool flow_found = false;
    while (flow_found == false) {
      getline(db, line_txt);
      std::stringstream sst(line_txt);
      sst >> txt;
      if (txt == "Flow_def" || txt == "Flow_def:" || txt == "flow_def" ||
          txt == "flow_def:") {
        while (sst >> txt) {
          if (txt == input_def) {
            flow_found = true;
            return true;
          }
        }
      }
      if (db.eof()) {
        break;
      }
    }
    if (flow_found == false) {
      std::cout << error << std::endl;
      db.close();
    }
  }

  return false;
}
