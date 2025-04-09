#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

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
  if(data_type == "str"){
    string txt;
    while (sst >> txt) {
      str.push_back(txt);
    }
  }
  if(data_type == "num"){
    double val;
    while (sst >> val) {
      vct.push_back(val);
    }
  }
	
  pos = 0;
}

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

string get_str_parameter(vector<parameter> &par, string sys_type, string sys_def,
                           string data_id) {
  bool found = false;
  for (size_t np = 0; np < par.size(); np++) {
    if (par[np].sys_type == sys_type && par[np].sys_def == sys_def &&
        par[np].data_id == data_id) {
      string val = par[np].str[par[np].pos];
      // cout << "value : " << val << endl;
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

double get_num_parameter(vector<parameter> &par, string sys_type, string sys_def,
                           string data_id) {
  bool found = false;
  for (size_t np = 0; np < par.size(); np++) {
    if (par[np].sys_type == sys_type && par[np].sys_def == sys_def &&
        par[np].data_id == data_id) {
      double val = par[np].vct[par[np].pos];
      // cout << "value : " << val << endl;
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

void print_parameter_value(vector<parameter> &p, string sys_type, string sys_def,
                           string data_id) {
  bool found = false;
  for (size_t np = 0; np < p.size(); np++) {
    if (p[np].sys_type == sys_type && p[np].sys_def == sys_def &&
        p[np].data_id == data_id) {
      found = true;
      cout << p[np].sys_type << " " << p[np].sys_def << " " << p[np].data_def << " "
           << p[np].data_id << " " << p[np].str[p[np].pos] << endl;
    }
  }
  if (found == false) {
    cout << "none found" << endl;
  }
}

// **********************************************************

/*
void export_parameter(vector<parameter> &par, string data_def, string sys_type,
                      string sys_def, string data_id, string data_type, string val) {
  // cout << "exporting parameter" << endl;
  bool found = false;
  for (size_t np = 0; np < par.size(); np++) {
    if (par[np].sys_type == sys_type && par[np].sys_def == sys_def &&
        par[np].data_id == data_id) {
      if( data_type == "str" ) { par[np].str.push_back(val); }
      if( data_type == "num" ) { par[np].vct.push_back(stod(val)); }
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
    p.data_type = data_type;
    if( data_type == "str" ) { p.str.push_back(val); }
    if( data_type == "num" ) { p.vct.push_back(stod(val)); }
    p.pos = 0;
    par.push_back(p);
  }
}
*/

// **********************************************************

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
          //cout << "data_def: " << txt << " data_id: " << p.data_id << " data_type: " << p.data_type << endl;

          bool str_complete = false;

            p.data_info = "";
            while (sst >> str) {
              vector<char> cstr(str.begin(), str.end());
              if (!str_complete && cstr[0] != '#') {

                if(p.data_type == "str"){ p.str.push_back(str); }
                if(p.data_type == "num"){ p.vct.push_back(stod(str)); }

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

void export_output_parameters(object &obj, string file) {

  ofstream output_parameters(file);

  for (size_t np = 0; np < obj.p.size(); np++) {
    if (obj.p[np].data_def == "output") {
      output_parameters << obj.p[np].sys_type << " " << obj.p[np].sys_def << " "
                        << obj.p[np].data_id << " " << obj.p[np].data_type;

      if( obj.p[np].data_type == "str" ){	
        for (size_t ns = 0; ns < obj.p[np].str.size(); ns++) {
          output_parameters << " " << obj.p[np].str[ns];
        }
      }

      if( obj.p[np].data_type == "num" ){	
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

void print_parameters(vector<parameter> &p) {

  cout << "no. parameters: " << p.size() << endl;

  for (size_t np = 0; np < p.size(); np++) {
    cout << p[np].sys_type << " " << p[np].sys_def << " " << p[np].data_def << " "
         << " " << p[np].data_id << p[np].data_type;

    if( p[np].data_type == "str" ){	
      for (size_t ns = 0; ns < p[np].str.size(); ns++) {
        cout << " " << p[np].str[ns];
      }
    }

    if( p[np].data_type == "num" ){	
      for (size_t ns = 0; ns < p[np].vct.size(); ns++) {
        cout << " " << p[np].vct[ns];
      }
    }
    cout << endl;

  }
}

// **********************************************************

double fp(vector<parameter> &par, string sys_type, string sys_def, string data_id) {
  get_num_parameter(par, sys_type, sys_def, data_id);
}

// **********************************************************

string sp(vector<parameter> &par, string sys_type, string sys_def, string data_id) {
  return get_str_parameter(par, sys_type, sys_def, data_id);
}

// **********************************************************

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


int object::ic(string type, string def) {
  for (size_t n = 0; n < c.size(); n++) {
    if (c[n].sys_type == type && c[n].sys_def == def) {
      return n;
    }
  }
  return -1;
}

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

bool object::bp(string symb) {
  bool found = false;
  for (size_t np = 0; np < p.size(); np++) {
    if (p[np].data_id == symb) {
      return true;
    }
  }
  // if( found == false ) { return false; }
  return false;
}

// **********************************************************

double object::fp(string symb) {
  return get_num_parameter(p, sys_type, sys_def, symb);
}

// **********************************************************

string object::sp(string symb) { return get_str_parameter(p, sys_type, sys_def, symb); }

// **********************************************************

vector<string> object::svct(string symb) { return sp_vct(p, sys_type, sys_def, symb); }


// **********************************************************

void val_p(vector<parameter> &par, string data_def, string sys_type, string sys_def,
            string data_id, double val) {

  //export_parameter(par, data_def, sys_type, sys_def, data_id, "num", val);

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

void str_p(vector<parameter> &par, string data_def, string sys_type, string sys_def,
            string data_id, string val) {

  //export_parameter(par, data_def, sys_type, sys_def, data_id, "str", val);

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
    // cout << "not found in the list... creating" << endl;
    parameter p;
    p.data_def = data_def;
    p.sys_type = sys_type;
    p.sys_def = sys_def;
    p.data_id = data_id;
    p.data_type = "num";
    p.str.push_back(val); 
    p.pos = 0;
    par.push_back(p);
  }

}

// **********************************************************


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

vector<double> object::vctp(string symb) { return fp_vct(p, sys_type, sys_def, symb); }

// **********************************************************

void fvct_p(vector<parameter> &par, string data_def, string sys_type, string sys_def,
            string data_id, vector<double> val) {
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
    // cout << "not found in the list... creating" << endl;
    parameter p;
    p.data_def = data_def;
    p.sys_type = sys_type;
    p.sys_def = sys_def;
    p.data_id = data_id;
    p.pos = 0;
    for (size_t n = 0; n < val.size(); n++) {
      p.vct.push_back(val[n]);
    }
    par.push_back(p);
    // print_parameter_value(par,par[par.size()-1].sys_type, par[par.size()-1].sys_def,
    // par[par.size()-1].data_id);
  }
}

// **********************************************************

void object::vct_fp(string symb, vector<double> vct) {
  if (divide_string(symb, '-').size() == 1) {
    fvct_p(p, "prop", sys_type, sys_def, symb, vct);
  }
  if (divide_string(symb, '-').size() == 2) {
    fvct_p(p, divide_string(symb, '-')[0], sys_type, sys_def, divide_string(symb, '-')[1],
           vct);
  }
}

// **********************************************************


void svct_p(vector<parameter> &par, string data_def, string sys_type, string sys_def,
            string data_id, vector<string> val) {
  // cout << "exporting parameter" << endl;
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
    // cout << "not found in the list... creating" << endl;
    parameter p;
    p.data_def = data_def;
    p.sys_type = sys_type;
    p.sys_def = sys_def;
    p.data_id = data_id;
    p.pos = 0;
    for (size_t n = 0; n < val.size(); n++) {
      p.str.push_back(val[n]);
    }
    par.push_back(p);
    // print_parameter_value(par,par[par.size()-1].sys_type, par[par.size()-1].sys_def,
    // par[par.size()-1].data_id);
  }
}

void object::vct_sp(string symb, vector<string> vct) {
  if (divide_string(symb, '-').size() == 1) {
    svct_p(p, "prop", sys_type, sys_def, symb, vct);
  }
  if (divide_string(symb, '-').size() == 2) {
    svct_p(p, divide_string(symb, '-')[0], sys_type, sys_def, divide_string(symb, '-')[1],
           vct);
  }
}

void transfer_parameter(string symb, object from, object &to) {
  to.p.push_back(from.p[from.ip(symb)]);
}
