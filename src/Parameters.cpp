#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

/**
 * @brief Structure defining a parameter
 *
 */
struct parameter {
 public:
  string sys_type, sys_def;
  string data_def, data_id, data_type, data_info;
  size_t pos;
  vector<string> str;
  vector<double> vct;
  parameter(string line);
  parameter(){};
};

parameter::parameter(string line) {
  stringstream sst(line);
  sst >> data_def;
  sst >> sys_type;
  sst >> sys_def;
  sst >> data_id;
  sst >> data_type;
  if (data_type == "str") {
    string txt;
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
 * @brief Structure defining a object
 *
 */
struct object {
 public:
  string sys_type, sys_def, sys_file;
  vector<object> c;
  vector<parameter> p;
  object(string type, string def, string file);
  object(string type, string def);
  object(){};
  int ic(string type, string def);
  int ip(string symb);
  bool bp(string symb);
  double fp(string symb);
  vector<double> vctp(string symb);
  string sp(string symb);
  vector<string> svct(string symb);
  void fval_p(string symb, double val);
  void sval_p(string symb, string val);
  void vct_fp(string symb, vector<double> vct);
  void vct_sp(string symb, vector<string> vct);
};

// **********************************************************

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
string get_str_parameter(vector<parameter> &par, string sys_type,
                         string sys_def, string data_id) {
  bool found = false;
  for (size_t np = 0; np < par.size(); np++) {
    if (par[np].sys_type == sys_type && par[np].sys_def == sys_def &&
        par[np].data_id == data_id) {
      string val = par[np].str[par[np].pos];
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
double get_num_parameter(vector<parameter> &par, string sys_type,
                         string sys_def, string data_id) {
  bool found = false;
  for (size_t np = 0; np < par.size(); np++) {
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
void get_parameters(vector<parameter> &par, string sys_type, string sys_def,
                    string input_file) {
  // cout << "getting parameter for " << sys_type << " " << sys_def << endl;
  ifstream p_file;
  p_file.open(input_file);
  if (!p_file.good()) {
    cout << "input file not found " << endl;
    p_file.close();
    return;
  }

  parameter p;
  string line_txt, type, def, txt, str;
  double num;
  bool par_set_found = false, sys_found = false;

  while (!sys_found) {
    getline(p_file, line_txt);
    stringstream sst(line_txt);
    sst >> type;
    sst >> def;
    if (type == sys_type && def == sys_def) {
      // cout << sys_type << " " << sys_def << endl;
      sys_found = true;

      while (!par_set_found) {
        getline(p_file, line_txt);  // cout << "line_txt: " << line_txt << endl;
        stringstream sst(line_txt);
        sst >> txt;

        if (txt == "input" || txt == "prop" || txt == "output") {
          p.sys_type = sys_type;
          p.sys_def = sys_def;
          p.data_def = txt;
          sst >> p.data_id;
          sst >> p.data_type;

          bool str_complete = false;

          p.data_info = "";
          while (sst >> str) {
            vector<char> cstr(str.begin(), str.end());
            if (!str_complete && cstr[0] != '#') {
              if (p.data_type == "str") {
                p.str.push_back(str);
              }
              if (p.data_type == "num") {
                p.vct.push_back(stod(str));
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
object::object(string type, string def, string file) {
  sys_type = type;
  sys_def = def;
  sys_file = file;
  get_parameters(p, type, def, file);
}

object::object(string type, string def) {
  sys_type = type;
  sys_def = def;
  if (type == "equipment") {
    get_parameters(p, type, def, DIR + "Database/Equipment_database");
  }
  if (type == "consumable") {
    get_parameters(p, type, def, DIR + "Database/Consumables_database");
  }
  if (type == "solid_residue") {
    get_parameters(p, type, def, DIR + "Database/Consumables_database");
  }
}

// **********************************************************

/**
 * @brief function to export parameter values to a text file
 *
 * @param obj object containing parameters to export
 */
void export_output_parameters(object &obj, string file) {
  ofstream output_parameters(file);

  for (size_t np = 0; np < obj.p.size(); np++) {
    if (obj.p[np].data_def == "output") {
      output_parameters << obj.p[np].sys_type << " " << obj.p[np].sys_def << " "
                        << obj.p[np].data_id << " " << obj.p[np].data_type;

      if (obj.p[np].data_type == "str") {
        for (size_t ns = 0; ns < obj.p[np].str.size(); ns++) {
          output_parameters << " " << obj.p[np].str[ns];
        }
      }

      if (obj.p[np].data_type == "num") {
        for (size_t ns = 0; ns < obj.p[np].vct.size(); ns++) {
          output_parameters << " " << obj.p[np].vct[ns];
        }
      }

      output_parameters << endl;
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
  cout << p.data_id << " " << p.data_type;

  if (p.data_type == "str") {
    for (size_t ns = 0; ns < p.str.size(); ns++) {
      cout << " " << p.str[ns];
    }
    cout << endl;
  }

  if (p.data_type == "num") {
    for (size_t ns = 0; ns < p.vct.size(); ns++) {
      cout << " " << p.vct[ns];
    }
    cout << endl;
  }
}

// **********************************************************

/**
 * @brief function printing all parameter of one object
 *
 * @param obj object with the parameters to print out
 */
void print_parameters(object &obj) {
  cout << " -------------------------------------- " << endl;
  cout << obj.sys_type << " " << obj.sys_def << " parameters: " << endl;
  cout << " -------------------------------------- " << endl;

  for (size_t np = 0; np < obj.p.size(); np++) {
    print_parameter(obj.p[np]);
  }
  cout << " -------------------------------------- " << endl;

  if (obj.c.size() > 0) {
    for (size_t nc = 0; nc < obj.c.size(); nc++) {
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
double fp(vector<parameter> &par, string sys_type, string sys_def,
          string data_id) {
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
string sp(vector<parameter> &par, string sys_type, string sys_def,
          string data_id) {
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
vector<string> sp_vct(vector<parameter> &par, string sys_type, string sys_def,
                      string data_id) {
  bool found = false;
  vector<string> vct;
  for (size_t np = 0; np < par.size(); np++) {
    if (par[np].sys_type == sys_type && par[np].sys_def == sys_def &&
        par[np].data_id == data_id) {
      for (size_t n = 0; n < par[np].str.size(); n++) {
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
int object::ic(string type, string def) {
  for (size_t n = 0; n < c.size(); n++) {
    if (c[n].sys_type == type && c[n].sys_def == def) {
      return n;
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
int object::ip(string symb) {
  bool found = false;
  for (size_t np = 0; np < p.size(); np++) {
    if (p[np].data_id == symb) {
      return np;
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
bool object::bp(string symb) {
  bool found = false;
  for (size_t np = 0; np < p.size(); np++) {
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
double object::fp(string symb) {
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
string object::sp(string symb) {
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
vector<string> object::svct(string symb) {
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
void val_p(vector<parameter> &par, string data_def, string sys_type,
           string sys_def, string data_id, double val) {
  bool found = false;
  for (size_t np = 0; np < par.size(); np++) {
    if (par[np].sys_type == sys_type && par[np].sys_def == sys_def &&
        par[np].data_id == data_id) {
      par[np].vct.push_back(val);
      par[np].pos = par[np].str.size() - 1;
      found = true;
    }
  }
  if (found == false) {
    // cout << "not found in the list... creating" << endl;
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
void object::fval_p(string symb, double val) {
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
void str_p(vector<parameter> &par, string data_def, string sys_type,
           string sys_def, string data_id, string val) {
  bool found = false;
  for (size_t np = 0; np < par.size(); np++) {
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
void object::sval_p(string symb, string val) {
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
vector<double> fp_vct(vector<parameter> &par, string sys_type, string sys_def,
                      string data_id) {
  bool found = false;
  vector<double> vct;
  for (size_t np = 0; np < par.size(); np++) {
    if (par[np].sys_type == sys_type && par[np].sys_def == sys_def &&
        par[np].data_id == data_id) {
      for (size_t n = 0; n < par[np].vct.size(); n++) {
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
vector<double> object::vctp(string symb) {
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
void fvct_p(vector<parameter> &par, string data_def, string sys_type,
            string sys_def, string data_id, vector<double> val) {
  // cout << "exporting parameter" << endl;
  bool found = false;
  for (size_t np = 0; np < par.size(); np++) {
    if (par[np].sys_type == sys_type && par[np].sys_def == sys_def &&
        par[np].data_id == data_id) {
      par[np].vct.clear();
      for (size_t n = 0; n < val.size(); n++) {
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
    for (size_t n = 0; n < val.size(); n++) {
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
void object::vct_fp(string symb, vector<double> vct) {
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
void svct_p(vector<parameter> &par, string data_def, string sys_type,
            string sys_def, string data_id, vector<string> val) {
  bool found = false;
  for (size_t np = 0; np < par.size(); np++) {
    if (par[np].sys_type == sys_type && par[np].sys_def == sys_def &&
        par[np].data_id == data_id) {
      par[np].str.clear();
      for (size_t n = 0; n < val.size(); n++) {
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
    for (size_t n = 0; n < val.size(); n++) {
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
void object::vct_sp(string symb, vector<string> vct) {
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
void transfer_parameter(string symb, object from, object &to) {
  to.p.push_back(from.p[from.ip(symb)]);
}
