#include "Parameters.h"

#include <fstream>
#include <iostream>
#include <limits>  // for numeric_limits
#include <sstream>
#include <toml++/toml.hpp>

#include "utils.h"

parameter::parameter() {}

/**
 * @brief Parameter constructor
 * @param stype string defining the type of object where parameter belongs
 * @param sdef string defining the name of object where parameter belongs
 * @param ddef Defined type of parameter (e.g., input, prop, output)
 * @param symb string defining the identification name of parameter (e.g., W_el,
 * Q, C_pi)
 * @param dtype string defining the type of parameter value ("num" = numerical,
 * "str" = string)
 * @param val double with the parameter value
 */
parameter::parameter(std::string stype, std::string sdef, std::string ddef,
                     std::string symb, std::string dtype, double val) {
  sys_type = stype;
  sys_def = sdef;
  data_def = ddef;
  data_id = symb;
  data_type = dtype;
  vct.push_back(val);
}

/**
 * @brief Parameter constructor
 * @param stype string defining the type of object where parameter belongs
 * @param sdef string defining the name of object where parameter belongs
 * @param ddef Defined type of parameter (e.g., input, prop, output)
 * @param symb string defining the identification name of parameter (e.g., W_el,
 * Q, C_pi)
 * @param dtype string defining the type of parameter value ("num" = numerical,
 * "str" = string)
 * @param val vector double with the parameter values
 */
parameter::parameter(std::string stype, std::string sdef, std::string ddef,
                     std::string symb, std::string dtype,
                     std::vector<double> val) {
  sys_type = stype;
  sys_def = sdef;
  data_def = ddef;
  data_id = symb;
  data_type = dtype;
  for (std::size_t n = 0; n < val.size(); n++) {
    vct.push_back(val[n]);
  }
}

/**
 * @brief Parameter constructor
 * @param stype string defining the type of object where parameter belongs
 * @param sdef string defining the name of object where parameter belongs
 * @param ddef Defined type of parameter (e.g., input, prop, output)
 * @param symb string defining the identification name of parameter (e.g., W_el,
 * Q, C_pi)
 * @param dtype string defining the type of parameter value ("num" = numerical,
 * "str" = string)
 * @param val string with the parameter value
 */
parameter::parameter(std::string stype, std::string sdef, std::string ddef,
                     std::string symb, std::string dtype, std::string val) {
  sys_type = stype;
  sys_def = sdef;
  data_def = ddef;
  data_id = symb;
  data_type = dtype;
  str.push_back(val);
}

/**
 * @brief Parameter constructor
 * @param stype string defining the type of object where parameter belongs
 * @param sdef string defining the name of object where parameter belongs
 * @param ddef Defined type of parameter (e.g., input, prop, output)
 * @param symb string defining the identification name of parameter (e.g., W_el,
 * Q, C_pi)
 * @param dtype string defining the type of parameter value ("num" = numerical,
 * "str" = string)
 * @param val vector string with the parameter values
 */
parameter::parameter(std::string stype, std::string sdef, std::string ddef,
                     std::string symb, std::string dtype,
                     std::vector<std::string> val) {
  sys_type = stype;
  sys_def = sdef;
  data_def = ddef;
  data_id = symb;
  data_type = dtype;
  for (std::size_t n = 0; n < val.size(); n++) {
    str.push_back(val[n]);
  }
}

/**
 * @brief Object initializing function
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
  sys_file = get_database_path("costs.toml");

  if (type == "consumable" || type == "solid_residue" || type == "equipment")
    get_parameters(p, type, def, sys_file);
}

/**
 * @brief object function to get the index of a sub-object
 *
 * @param type string defining the type of the object to look for
 * @param def string defining the name of the object to look for
 *
 * @return index of the sub-object
 */
std::size_t object::ic(std::string type, std::string def) {
  for (std::size_t n = 0; n < c.size(); n++) {
    if (c[n].sys_type == type && c[n].sys_def == def) return n;
  }
  throw std::out_of_range("Sub-object of type " + type + " and definition " +
                          def + " not found.");
}

/**
 * @brief object function to find out if a parameter within the object exists
 *
 * @param symb name of the parameter to look for
 *
 * @return true if it exists, false otherwise
 */
bool object::bp(std::string symb) {
  for (std::size_t n = 0; n < p.size(); n++) {
    if (p[n].data_id == symb) return true;
  }
  return false;
}

/**
 * @brief object function to remove a parameter within the object if exists
 *
 * @param symb name of the parameter to look for
 *
 * @return true if it exists, false otherwise
 */
bool object::remove(std::string symb) {
  for (std::size_t n = 0; n < p.size(); n++) {
    if (p[n].data_id == symb) {
      p.erase(p.begin() + n);
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
  std::vector<double> vct;
  for (std::size_t np = 0; np < p.size(); np++) {
    if (p[np].sys_type == sys_type && p[np].sys_def == sys_def &&
        p[np].data_id == symb) {
      for (std::size_t n = 0; n < p[np].vct.size(); n++) {
        vct.push_back(p[np].vct[n]);
      }
      return vct;
    }
  }
  return vct;
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
  std::vector<std::string> vct;
  for (std::size_t np = 0; np < p.size(); np++) {
    if (p[np].sys_type == sys_type && p[np].sys_def == sys_def &&
        p[np].data_id == symb) {
      for (std::size_t n = 0; n < p[np].str.size(); n++) {
        vct.push_back(p[np].str[n]);
      }
      return vct;
    }
  }
  return vct;
}

/**
 * @brief object function to populate a vector of parameters with a input
 * numerical parameter
 *
 * @param symb parameter name to populate
 * @param val numerical value
 * @param data_def type of parameter (e.g., input, prop, output)
 *
 */
void object::fval_p(std::string symb, double val, std::string data_def) {
  bool found = false;
  for (std::size_t np = 0; np < p.size(); np++) {
    if (p[np].data_id == symb) {
      p[np].vct.push_back(val);
      p[np].pos = p[np].str.size() - 1;
      found = true;
    }
  }
  if (!found) {
    p.push_back(parameter(sys_type, sys_def, data_def, symb, "num", val));
  }
}

/**
 * @brief object function to populate a vector of parameters with an input
 * numerical vector parameter
 *
 * @param symb parameter name to populate
 * @param vct vector with numerical values
 * @param data_def type of parameter (e.g., input, prop, output)
 */
void object::vct_fp(std::string symb, std::vector<double> vct,
                    std::string data_def) {
  bool found = false;
  for (std::size_t np = 0; np < p.size(); np++) {
    if (p[np].data_id == symb) {
      p[np].vct.clear();
      for (std::size_t n = 0; n < vct.size(); n++) {
        p[np].vct.push_back(vct[n]);
      }
      p[np].pos = 0;
      found = true;
    }
  }
  if (!found) {
    p.push_back(parameter(sys_type, sys_def, data_def, symb, "num", vct));
  }
}

/**
 * @brief object function to populate a vector of parameters with an input
 * string vector parameter
 *
 * @param symb parameter name to populate
 * @param vct vector with string values
 * @param data_def type of parameter (e.g., input, prop, output)
 */
void object::vct_sp(std::string symb, std::vector<std::string> vct,
                    std::string data_def) {
  bool found = false;
  for (std::size_t np = 0; np < p.size(); np++) {
    if (p[np].data_id == symb) {
      p[np].str.clear();
      for (std::size_t n = 0; n < vct.size(); n++) {
        p[np].str.push_back(vct[n]);
      }
      p[np].pos = 0;
      found = true;
    }
  }
  if (!found) {
    p.push_back(parameter(sys_type, sys_def, data_def, symb, "str", vct));
  }
}

std::string get_str_parameter(std::vector<parameter> &par, std::string sys_type,
                              std::string sys_def, std::string data_id) {
  for (std::size_t np = 0; np < par.size(); np++) {
    if (par[np].sys_type == sys_type && par[np].sys_def == sys_def &&
        par[np].data_id == data_id) {
      std::string val = par[np].str[par[np].pos];
      if (par[np].pos < par[np].str.size() - 1) par[np].pos++;
      return val;
    }
  }
  throw std::out_of_range("Parameter with data_id " + data_id +
                          " not found for sys_type " + sys_type +
                          " and sys_def " + sys_def + ".");
}

double get_num_parameter(std::vector<parameter> &par, std::string sys_type,
                         std::string sys_def, std::string data_id) {
  for (std::size_t np = 0; np < par.size(); np++) {
    if (par[np].sys_type == sys_type && par[np].sys_def == sys_def &&
        par[np].data_id == data_id) {
      double val = par[np].vct[par[np].pos];
      if (par[np].pos < par[np].vct.size() - 1) par[np].pos++;

      return val;
    }
  }
  throw std::out_of_range("Parameter with data_id " + data_id +
                          " not found for sys_type " + sys_type +
                          " and sys_def " + sys_def + ".");
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
  } else {
    throw std::runtime_error("No parameters found for sys_type " + sys_type +
                             " and sys_def " + sys_def + " in " + input_file);
  }
}

void export_output_parameters(object &obj, std::string file) {
  std::ofstream output_parameters(file);

  for (std::size_t np = 0; np < obj.p.size(); np++) {
    if (obj.p[np].data_def == "output") {
      output_parameters << obj.p[np].sys_type << " " << obj.p[np].sys_def << " "
                        << obj.p[np].data_id << " " << obj.p[np].data_type;

      if (obj.p[np].data_type == "str") {
        for (std::size_t ns = 0; ns < obj.p[np].str.size(); ns++)
          output_parameters << " " << obj.p[np].str[ns];
      }

      if (obj.p[np].data_type == "num") {
        for (std::size_t ns = 0; ns < obj.p[np].vct.size(); ns++)
          output_parameters << " " << obj.p[np].vct[ns];
      }

      output_parameters << std::endl;
    }
  }

  output_parameters.close();
}