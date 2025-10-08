#include "Parameters.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <toml++/toml.hpp>

#include "utils.h"

/**
 * @brief Object initializing functions
 * @param type string defining the type of object
 * @param def string defining the name of object
 * @param file string defining the name of the file with object's data
 */
object::object(std::string type, std::string def, std::string file) {
  sys_type = type;
  sys_def = def;
  sys_file = file;
  get_parameters(p, type, def, file);
}

/**
 * @brief Object initializing function
 * @param type string defining the type of object
 * @param def string defining the name of object
 */
object::object(std::string type, std::string def) {
  sys_type = type;
  sys_def = def;
  std::string file = "";
  if (type == "equipment")
    file = get_database_path("Equipment_database.toml");
  else if (type == "consumable" || type == "solid_residue")
    file = get_database_path("Consumables_database.toml");

  sys_file = file;
  if (!file.empty()) get_parameters(p, type, def, file);
}

/**
 * @brief object function to get the index of a sub-object
 *
 * @param type string defining the type of the object to look for
 * @param def string defining the name of the object to look for
 *
 * @return index of the sub-object
 */
int object::ic(std::string type, std::string def) {
  for (std::size_t n = 0; n < c.size(); n++) {
    if (c[n].sys_type == type && c[n].sys_def == def) {
      return static_cast<int>(n);
    }
  }
  return -1;
}

/**
 * @brief object function to get the index of parameter within the object
 *
 * @param symb name of the parameter to look for
 *
 * @return index of the parameter
 */
int object::ip(std::string symb) {
  for (std::size_t np = 0; np < p.size(); np++) {
    if (p[np].data_id == symb) {
      return static_cast<int>(np);
    }
  }
  return -1;
}

/**
 * @brief object function to find out if a parameter within the object exists
 *
 * @param symb name of the parameter to look for
 *
 * @return true if it exists, false otherwise
 */
bool object::bp(std::string symb) {
  for (std::size_t np = 0; np < p.size(); np++) {
    if (p[np].data_id == symb) {
      return true;
    }
  }
  return false;
}

/**
 * @brief object function to get the numerical value of a parameter within the
 * object
 *
 * @param symb name of the parameter to look for
 *
 * @return value of the parameter as double
 */
double object::fp(std::string symb) {
  return get_num_parameter(p, sys_type, sys_def, symb);
}

/**
 * @brief object function to get a numerical vector of a parameter
 *
 * @param symb parameter name to get
 *
 * @return vector with numerical values
 */
std::vector<double> object::vctp(std::string symb) {
  return fp_vct(p, sys_type, sys_def, symb);
}

/**
 * @brief object function to get the value of a parameter within the object as
 * string
 *
 * @param symb name of the parameter to look for
 *
 * @return value of the parameter as string
 */
std::string object::sp(std::string symb) {
  return get_str_parameter(p, sys_type, sys_def, symb);
}

// **********************************************************

/**
 * @brief object function to get the numerical vector of values of a parameter
 * within the object
 *
 * @param symb name of the parameter to look for
 *
 * @return value of the parameter as double vector
 */
std::vector<std::string> object::svct(std::string symb) {
  return sp_vct(p, sys_type, sys_def, symb);
}

/**
 * @brief object function to populate a vector of parameters with a input
 * numerical parameter
 *
 * @param symb parameter name to populate
 * @param val numerical value
 *
 */
void object::fval_p(std::string symb, double val, std::string data_def) {
  val_p(p, data_def, sys_type, sys_def, symb, val);
}

/**
 * @brief object function to populate a vector of parameters with a input string
 * parameter
 *
 * @param symb parameter name to populate
 * @param val string value
 *
 */
void object::sval_p(std::string symb, std::string val) {
  str_p(p, "prop", sys_type, sys_def, symb, val);
}

/**
 * @brief object function to populate a vector of parameters with an input
 * numerical vector parameter
 *
 * @param symb parameter name to populate
 * @param vct vector with numerical values
 */
void object::vct_fp(std::string symb, std::vector<double> vct) {
  fvct_p(p, "prop", sys_type, sys_def, symb, vct);
}

/**
 * @brief object function to populate a vector of parameters with an input
 * string vector parameter
 *
 * @param symb parameter name to populate
 * @param vct vector with string values
 */
void object::vct_sp(std::string symb, std::vector<std::string> vct) {
  svct_p(p, "prop", sys_type, sys_def, symb, vct);
}

std::string get_str_parameter(std::vector<parameter> &par, std::string sys_type,
                              std::string sys_def, std::string data_id) {
  bool found = false;
  for (std::size_t np = 0; np < par.size(); np++) {
    if (par[np].sys_type == sys_type && par[np].sys_def == sys_def &&
        par[np].data_id == data_id) {
      std::string val = par[np].str[par[np].pos];
      if (par[np].pos < par[np].str.size() - 1) {
        par[np].pos = par[np].pos + 1;
      }
      found = true;
      return val;
    }
  }
  if (found == false) {
    return "null";
  }
  return "null";
}

double get_num_parameter(std::vector<parameter> &par, std::string sys_type,
                         std::string sys_def, std::string data_id) {
  bool found = false;
  for (std::size_t np = 0; np < par.size(); np++) {
    if (par[np].sys_type == sys_type && par[np].sys_def == sys_def &&
        par[np].data_id == data_id) {
      double val = par[np].vct[par[np].pos];
      if (par[np].pos < par[np].vct.size() - 1) {
        par[np].pos = par[np].pos + 1;
      }
      found = true;
      return val;
    }
  }
  return -1;
}

void get_parameters(std::vector<parameter> &par, std::string sys_type,
                    std::string sys_def, std::string input_file) {
  toml::table tbl = get_toml_table(input_file);

  if (auto items = tbl[sys_type][sys_def].as_array()) {
    for (const auto &item : *items) {
      auto item_tbl = item.as_table();

      parameter p;
      p.sys_type = sys_type;
      p.sys_def = sys_def;
      p.data_def = item_tbl->at("data_def").value_or("");
      p.data_id = item_tbl->at("data_id").value_or("");
      p.data_type = item_tbl->at("data_type").value_or("");
      p.data_info = item_tbl->at("data_info").value_or("");

      auto vals = item_tbl->at("values").as_array();

      for (const auto &val : *vals) {
        if (p.data_type == "str")
          p.str.push_back(val.value_or(""));
        else
          p.vct.push_back(static_cast<double>(val.value_or(0.0)));
      }
      par.push_back(p);
    }
  }
}

void export_output_parameters(object &obj, std::string file) {
  std::ofstream output_parameters(file);

  for (std::size_t np = 0; np < obj.p.size(); np++) {
    if (obj.p[np].data_def == "output") {
      output_parameters << obj.p[np].sys_type << " " << obj.p[np].sys_def << " "
                        << obj.p[np].data_id << " " << obj.p[np].data_type;

      if (obj.p[np].data_type == "str") {
        for (std::size_t ns = 0; ns < obj.p[np].str.size(); ns++) {
          output_parameters << " " << obj.p[np].str[ns];
        }
      }

      if (obj.p[np].data_type == "num") {
        for (std::size_t ns = 0; ns < obj.p[np].vct.size(); ns++) {
          output_parameters << " " << obj.p[np].vct[ns];
        }
      }

      output_parameters << std::endl;
    }
  }

  output_parameters.close();
}

void print_parameter(parameter &p) {
  std::cout << p.data_id << " " << p.data_type;

  if (p.data_type == "str") {
    for (std::size_t ns = 0; ns < p.str.size(); ns++) {
      std::cout << " " << p.str[ns];
    }
    std::cout << std::endl;
  }

  if (p.data_type == "num") {
    for (std::size_t ns = 0; ns < p.vct.size(); ns++) {
      std::cout << " " << p.vct[ns];
    }
    std::cout << std::endl;
  }
}

void print_parameters(object &obj) {
  std::cout << " -------------------------------------- " << std::endl;
  std::cout << obj.sys_type << " " << obj.sys_def
            << " parameters: " << std::endl;
  std::cout << " -------------------------------------- " << std::endl;

  for (std::size_t np = 0; np < obj.p.size(); np++) {
    print_parameter(obj.p[np]);
  }
  std::cout << " -------------------------------------- " << std::endl;

  if (obj.c.size() > 0) {
    for (std::size_t nc = 0; nc < obj.c.size(); nc++) {
      print_parameters(obj.c[nc]);
    }
  }
}

std::vector<std::string> sp_vct(std::vector<parameter> &par,
                                std::string sys_type, std::string sys_def,
                                std::string data_id) {
  bool found = false;
  std::vector<std::string> vct;
  for (std::size_t np = 0; np < par.size(); np++) {
    if (par[np].sys_type == sys_type && par[np].sys_def == sys_def &&
        par[np].data_id == data_id) {
      for (std::size_t n = 0; n < par[np].str.size(); n++) {
        vct.push_back(par[np].str[n]);
      }
      found = true;
      return vct;
    }
  }
  if (found == false) {
    return vct;
  }
  return vct;
}

void val_p(std::vector<parameter> &par, std::string data_def,
           std::string sys_type, std::string sys_def, std::string data_id,
           double val) {
  bool found = false;
  for (std::size_t np = 0; np < par.size(); np++) {
    if (par[np].sys_type == sys_type && par[np].sys_def == sys_def &&
        par[np].data_id == data_id) {
      par[np].vct.push_back(val);
      par[np].pos = par[np].str.size() - 1;
      found = true;
    }
  }
  if (found == false) {
    parameter p;
    p.data_def = data_def;
    p.sys_type = sys_type;
    p.sys_def = sys_def;
    p.data_id = data_id;
    p.data_type = "num";
    p.vct.push_back(val);
    par.push_back(p);
  }
}

void str_p(std::vector<parameter> &par, std::string data_def,
           std::string sys_type, std::string sys_def, std::string data_id,
           std::string val) {
  bool found = false;
  for (std::size_t np = 0; np < par.size(); np++) {
    if (par[np].sys_type == sys_type && par[np].sys_def == sys_def &&
        par[np].data_id == data_id) {
      par[np].str.push_back(val);
      par[np].pos = par[np].str.size() - 1;
      found = true;
    }
  }
  if (found == false) {
    parameter p;
    p.data_def = data_def;
    p.sys_type = sys_type;
    p.sys_def = sys_def;
    p.data_id = data_id;
    p.data_type = "str";
    p.str.push_back(val);
    par.push_back(p);
  }
}

std::vector<double> fp_vct(std::vector<parameter> &par, std::string sys_type,
                           std::string sys_def, std::string data_id) {
  bool found = false;
  std::vector<double> vct;
  for (std::size_t np = 0; np < par.size(); np++) {
    if (par[np].sys_type == sys_type && par[np].sys_def == sys_def &&
        par[np].data_id == data_id) {
      for (std::size_t n = 0; n < par[np].vct.size(); n++) {
        vct.push_back(par[np].vct[n]);
      }
      found = true;
      return vct;
    }
  }
  return vct;
}

void fvct_p(std::vector<parameter> &par, std::string data_def,
            std::string sys_type, std::string sys_def, std::string data_id,
            std::vector<double> val) {
  bool found = false;
  for (std::size_t np = 0; np < par.size(); np++) {
    if (par[np].sys_type == sys_type && par[np].sys_def == sys_def &&
        par[np].data_id == data_id) {
      par[np].vct.clear();
      for (std::size_t n = 0; n < val.size(); n++) {
        par[np].vct.push_back(val[n]);
      }
      par[np].pos = 0;
      found = true;
    }
  }
  if (found == false) {
    parameter p;
    p.data_def = data_def;
    p.sys_type = sys_type;
    p.sys_def = sys_def;
    p.data_id = data_id;
    p.data_type = "num";
    for (std::size_t n = 0; n < val.size(); n++) {
      p.vct.push_back(val[n]);
    }
    par.push_back(p);
  }
}

void svct_p(std::vector<parameter> &par, std::string data_def,
            std::string sys_type, std::string sys_def, std::string data_id,
            std::vector<std::string> val) {
  bool found = false;
  for (std::size_t np = 0; np < par.size(); np++) {
    if (par[np].sys_type == sys_type && par[np].sys_def == sys_def &&
        par[np].data_id == data_id) {
      par[np].str.clear();
      for (std::size_t n = 0; n < val.size(); n++) {
        par[np].str.push_back(val[n]);
      }
      par[np].pos = 0;
      found = true;
    }
  }
  if (found == false) {
    parameter p;
    p.data_def = data_def;
    p.sys_type = sys_type;
    p.sys_def = sys_def;
    p.data_id = data_id;
    p.data_type = "str";
    for (std::size_t n = 0; n < val.size(); n++) {
      p.str.push_back(val[n]);
    }
    par.push_back(p);
  }
}

void transfer_parameter(std::string symb, object from, object &to) {
  to.p.push_back(from.p[from.ip(symb)]);
}
