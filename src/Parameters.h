#pragma once

#include <cstddef>
#include <string>
#include <vector>

struct parameter {
 public:
  std::string sys_type, sys_def;
  std::string data_def, data_id, data_type, data_info;
  std::size_t pos;
  std::vector<std::string> str;
  std::vector<double> vct;
  parameter(std::string line);
  parameter(){};
};

struct object {
 public:
  std::string sys_type, sys_def, sys_file;
  std::vector<object> c;
  std::vector<parameter> p;
  object(std::string type, std::string def, std::string file);
  object(std::string type, std::string def);
  object();
  int ic(std::string type, std::string def);
  int ip(std::string symb);
  bool bp(std::string symb);
  double fp(std::string symb);
  std::vector<double> vctp(std::string symb);
  std::string sp(std::string symb);
  std::vector<std::string> svct(std::string symb);
  void fval_p(std::string symb, double val);
  void sval_p(std::string symb, std::string val);
  void vct_fp(std::string symb, std::vector<double> vct);
  void vct_sp(std::string symb, std::vector<std::string> vct);
};

// Function declarations
std::string get_str_parameter(std::vector<parameter> &par, std::string sys_type,
                         std::string sys_def, std::string data_id);

double get_num_parameter(std::vector<parameter> &par, std::string sys_type,
                         std::string sys_def, std::string data_id);

void get_parameters(std::vector<parameter> &par, std::string sys_type, std::string sys_def,
                    std::string input_file);

void export_output_parameters(object &obj, std::string file);

void print_parameter(parameter &p);

void print_parameters(object &obj);

std::vector<std::string> sp_vct(std::vector<parameter> &par, std::string sys_type, std::string sys_def,
                      std::string data_id);

void val_p(std::vector<parameter> &par, std::string data_def, std::string sys_type,
           std::string sys_def, std::string data_id, double val);

void str_p(std::vector<parameter> &par, std::string data_def, std::string sys_type,
           std::string sys_def, std::string data_id, std::string val);

std::vector<double> fp_vct(std::vector<parameter> &par, std::string sys_type, std::string sys_def,
                      std::string data_id);

void fvct_p(std::vector<parameter> &par, std::string data_def, std::string sys_type,
            std::string sys_def, std::string data_id, std::vector<double> val);

void svct_p(std::vector<parameter> &par, std::string data_def, std::string sys_type,
            std::string sys_def, std::string data_id, std::vector<std::string> val);

void transfer_parameter(std::string symb, object from, object &to);
