#include "Parameters.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "utils.h"

parameter::parameter(std::string line) {
  std::stringstream sst(line);
  sst >> data_def;
  sst >> sys_type;
  sst >> sys_def;
  sst >> data_id;
  sst >> data_type;
  if (data_type == "str") {
    std::string txt;
    while (sst >> txt) {
      str.push_back(txt);
    }
  }
  if (data_type == "num") {
    double val;
    while (sst >> val) {
      vct.push_back(val);
    }
  }

  pos = 0;
}

/**
 * @brief function to get the value of a parameter value as string
 *
 * @param par vector of parameters to look into
 * @param sys_type system type of the parameter to look for
 * @param sys_def system name of the parameter to look for
 * @param data_id parameter name to look for
 *
 * @return member of str vector where the index pos is pointing to
 */
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

/**
 * @brief function to get a parameter value as numerical
 *
 * @param par vector of parameters to look into
 * @param sys_type system type of the parameter to look for
 * @param sys_def system name of the parameter to look for
 * @param data_id parameter name to look for
 *
 * @return member of vct vector where the index pos is pointing to
 */
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

// **********************************************************

/**
 * @brief function to get parameter values from database
 *
 * @param par vector of parameters to get
 * @param sys_type system type of the parameter to look for
 * @param sys_def system name of the parameter to look for
 * @param input_file parameter name to file containing the parameters of the
 * system
 */
void get_parameters(std::vector<parameter> &par, std::string sys_type, std::string sys_def,
                    std::string input_file) {
  // std::cout << "getting parameter for " << sys_type << " " << sys_def << std::endl;
  std::ifstream p_file;
  p_file.open(input_file);
  if (!p_file.good()) {
    std::cout << "input file " << input_file << " not found " << std::endl;
    p_file.close();
    return;
  }

  parameter p;
  std::string line_txt, type, def, txt, str;
  bool par_set_found = false, sys_found = false;

  while (!sys_found) {
    std::getline(p_file, line_txt);
    std::stringstream sst(line_txt);
    sst >> type;
    sst >> def;
    if (type == sys_type && def == sys_def) {
      // std::cout << sys_type << " " << sys_def << std::endl;
      sys_found = true;

      while (!par_set_found) {
        std::getline(p_file, line_txt);  // std::cout << "line_txt: " << line_txt << std::endl;
        std::stringstream sst2(line_txt);
        sst2 >> txt;

        if (txt == "input" || txt == "prop" || txt == "output") {
          p.sys_type = sys_type;
          p.sys_def = sys_def;
          p.data_def = txt;
          sst2 >> p.data_id;
          sst2 >> p.data_type;

          bool str_complete = false;

          p.data_info = "";
          while (sst2 >> str) {
            std::vector<char> cstr(str.begin(), str.end());
            if (!str_complete && cstr[0] != '#') {
              if (p.data_type == "str") {
                p.str.push_back(str);
              }
              if (p.data_type == "num") {
                p.vct.push_back(std::stod(str));
              }
            }
            if (cstr[0] == '#') {
              str_complete = true;
            }
            if (str_complete && cstr[0] != '#') {
              p.data_info = str + " ";
            }
          }

          p.pos = 0;
          par.push_back(p);
          p = parameter();
          p.str.clear();
          p.vct.clear();
        }

        if (txt != "input" && txt != "output" && txt != "prop") {
          par_set_found = true;
          p_file.close();
          return;
        }
      }
    }
    if (p_file.eof()) {
      p_file.close();
      return;
    }
  }
}

// **********************************************************

/**
 * @brief Object initializing functions
 *
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

object::object(std::string type, std::string def) {
  sys_type = type;
  sys_def = def;
  if (type == "equipment") {
    get_parameters(p, type, def, get_database_path("Equipment_database"));
  }
  if (type == "consumable") {
    get_parameters(p, type, def, get_database_path("Consumables_database"));
  }
  if (type == "solid_residue") {
    get_parameters(p, type, def, get_database_path("Consumables_database"));
  }
}

// **********************************************************

/**
 * @brief function to export parameter values to a text file
 *
 * @param obj object containing parameters to export
 */
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

// **********************************************************

/**
 * @brief function printing one parameter
 *
 * @param p parameter to print out
 */
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

// **********************************************************

/**
 * @brief function printing all parameter of one object
 *
 * @param obj object with the parameters to print out
 */
void print_parameters(object &obj) {
  std::cout << " -------------------------------------- " << std::endl;
  std::cout << obj.sys_type << " " << obj.sys_def << " parameters: " << std::endl;
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

// **********************************************************

/**
 * @brief function to get the numerical value of a parameter
 *
 * @param sys_type system type of the parameter to look for
 * @param sys_def system name of the parameter to look for
 * @param data_id parameter name to get
 * @param par vector of parameters to look for
 */
/*
double fp(std::vector<parameter> &par, std::string sys_type, std::string sys_def,
          std::string data_id) {
  return get_num_parameter(par, sys_type, sys_def, data_id);
}
*/
// **********************************************************

/**
 * @brief function to get the string value of a parameter
 *
 * @param sys_type system type of the parameter to look for
 * @param sys_def system name of the parameter to look for
 * @param data_id parameter name to get
 * @param par vector of parameters to look for
 */
/*
std::string sp(std::vector<parameter> &par, std::string sys_type, std::string sys_def,
          std::string data_id) {
  return get_str_parameter(par, sys_type, sys_def, data_id);
}
*/
// **********************************************************

/**
 * @brief function to get the numerical vector of a parameter
 *
 * @param sys_type system type of the parameter to look for
 * @param sys_def system name of the parameter to look for
 * @param data_id parameter name to get
 * @param par vector of parameters to look for
 */
std::vector<std::string> sp_vct(std::vector<parameter> &par, std::string sys_type, std::string sys_def,
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

// **********************************************************

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
  // if( found == false ) { return -1; }
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

// **********************************************************

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

// **********************************************************

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

// **********************************************************

/**
 * @brief function to populate a vector of parameters with a input numerical
 * parameter
 *
 * @param par existing vector of parameters
 * @param sys_type system type of the parameter to look for
 * @param sys_def system name of the parameter to look for
 * @param data_type type of parameter: numerical or string
 * @param data_id parameter name to populate
 *
 */
void val_p(std::vector<parameter> &par, std::string data_def, std::string sys_type,
           std::string sys_def, std::string data_id, double val) {
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
    // std::cout << "not found in the list... creating" << std::endl;
    parameter p;
    p.data_def = data_def;
    p.sys_type = sys_type;
    p.sys_def = sys_def;
    p.data_id = data_id;
    p.data_type = "num";
    p.vct.push_back(val);
    p.pos = 0;
    par.push_back(p);
  }
}

// **********************************************************

/**
 * @brief object function to populate a vector of parameters with a input
 * numerical parameter
 *
 * @param symb parameter name to populate
 * @param val numerical value
 *
 */
void object::fval_p(std::string symb, double val) {
  if (divide_string(symb, '-').size() == 1) {
    val_p(p, "prop", sys_type, sys_def, symb, val);
  }
  if (divide_string(symb, '-').size() == 2) {
    val_p(p, divide_string(symb, '-')[0], sys_type, sys_def,
          divide_string(symb, '-')[1], val);
  }
}

// **********************************************************

/**
 * @brief function to populate a vector of parameters with a input string
 * parameter
 *
 * @param par existing vector of parameters
 * @param sys_type system type of the parameter to look for
 * @param sys_def system name of the parameter to look for
 * @param data_type type of parameter: numerical or string
 * @param data_id parameter name to populate
 *
 */
void str_p(std::vector<parameter> &par, std::string data_def, std::string sys_type,
           std::string sys_def, std::string data_id, std::string val) {
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
    p.pos = 0;
    par.push_back(p);
  }
}

// **********************************************************

/**
 * @brief object function to populate a vector of parameters with a input string
 * parameter
 *
 * @param symb parameter name to populate
 * @param val string value
 *
 */
void object::sval_p(std::string symb, std::string val) {
  if (divide_string(symb, '-').size() == 1) {
    str_p(p, "prop", sys_type, sys_def, symb, val);
  }
  if (divide_string(symb, '-').size() == 2) {
    str_p(p, divide_string(symb, '-')[0], sys_type, sys_def,
          divide_string(symb, '-')[1], val);
  }
}

// **********************************************************

/**
 * @brief function to get a numerical vector of a parameter
 *
 * @param par existing vector of parameters
 * @param sys_type system type of the parameter to look for
 * @param sys_def system name of the parameter to look for
 * @param data_id parameter name to get
 *
 * @return vector with numerical values
 */
std::vector<double> fp_vct(std::vector<parameter> &par, std::string sys_type, std::string sys_def,
                      std::string data_id) {
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

// **********************************************************

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

// **********************************************************

/**
 * @brief function to populate a vector of parameters with an input numerical
 * vector parameter
 *
 * @param par existing vector of parameters
 * @param sys_type system type of the parameter to look for
 * @param sys_def system name of the parameter to look for
 * @param data_type type of parameter: numerical or string
 * @param data_id parameter name to populate
 * @param val vector with numerical values
 */
void fvct_p(std::vector<parameter> &par, std::string data_def, std::string sys_type,
            std::string sys_def, std::string data_id, std::vector<double> val) {
  // std::cout << "exporting parameter" << std::endl;
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
    p.pos = 0;
    for (std::size_t n = 0; n < val.size(); n++) {
      p.vct.push_back(val[n]);
    }
    par.push_back(p);
  }
}

// **********************************************************

/**
 * @brief object function to populate a vector of parameters with an input
 * numerical vector parameter
 *
 * @param symb parameter name to populate
 * @param vct vector with numerical values
 */
void object::vct_fp(std::string symb, std::vector<double> vct) {
  if (divide_string(symb, '-').size() == 1) {
    fvct_p(p, "prop", sys_type, sys_def, symb, vct);
  }
  if (divide_string(symb, '-').size() == 2) {
    fvct_p(p, divide_string(symb, '-')[0], sys_type, sys_def,
           divide_string(symb, '-')[1], vct);
  }
}

// **********************************************************

/**
 * @brief function to populate a vector of parameters with an input string
 * vector parameter
 *
 * @param par existing vector of parameters
 * @param sys_type system type of the parameter to look for
 * @param sys_def system name of the parameter to look for
 * @param data_type type of parameter: numerical or string
 * @param data_id parameter name to populate
 * @param val vector with string values
 */
void svct_p(std::vector<parameter> &par, std::string data_def, std::string sys_type,
            std::string sys_def, std::string data_id, std::vector<std::string> val) {
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
    p.pos = 0;
    for (std::size_t n = 0; n < val.size(); n++) {
      p.str.push_back(val[n]);
    }
    par.push_back(p);
  }
}

/**
 * @brief object function to populate a vector of parameters with an input
 * string vector parameter
 *
 * @param symb parameter name to populate
 * @param vct vector with string values
 */
void object::vct_sp(std::string symb, std::vector<std::string> vct) {
  if (divide_string(symb, '-').size() == 1) {
    svct_p(p, "prop", sys_type, sys_def, symb, vct);
  }
  if (divide_string(symb, '-').size() == 2) {
    svct_p(p, divide_string(symb, '-')[0], sys_type, sys_def,
           divide_string(symb, '-')[1], vct);
  }
}

/**
 * @brief function to transfer a parameter between objects
 *
 * @param symb parameter name to populate
 * @param from object to transfer from
 * @param to object to transfer to
 */
void transfer_parameter(std::string symb, object from, object &to) {
  to.p.push_back(from.p[from.ip(symb)]);
}
