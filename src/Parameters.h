#pragma once

#include <cstddef>
#include <string>
#include <vector>

/**
 * @struct parameter
 * @brief Structure defining a parameter
 * @var parameter::sys_type
 * Type of system where parameter is defined (e.g., plant)
 * @var parameter::sys_def
 * Defined name of system where parameter is defined (e.g., bioCHP)
 * @var parameter::data_def
 * Defined type of parameter (e.g., input, prop, output)
 * @var parameter::data_id
 * Identification name of parameter (e.g., W_el, Q, C_pi)
 * @var parameter::data_type
 * Type of parameter value ("num" = numerical, "str" = string)
 * @var parameter::data_info
 * String with information about parameter (e.g. a reference)
 * @var parameter::str
 * vector of string specifications
 * @var parameter::vct
 * vector of numerical specifications
 * @var parameter::pos
 * actual position in vct
 */
struct parameter {
 public:
  std::string sys_type = "", sys_def = "";
  std::string data_def = "", data_id = "", data_type = "", data_info = "";
  std::size_t pos = 0;
  std::vector<std::string> str;
  std::vector<double> vct;
};

/**
 * @struct object
 * @brief Structure defining a object
 * @var object::sys_type
 * Type of system (e.g., plant)
 * @var object::sys_def
 * Name of system where parameter is defined (e.g., bioCHP)
 * @var object::sys_file
 * Name of file where parameter is found
 * @var object::c
 * Vector of object components
 * @var object::p
 * Vector of parameters defining the object
 */
struct object {
 public:
  std::string sys_type = "", sys_def = "", sys_file = "";
  std::vector<object> c;
  std::vector<parameter> p;
  object(std::string type, std::string def, std::string file);
  object(std::string type, std::string def);
  object() {}
  std::size_t ic(std::string type, std::string def);
  std::size_t ip(std::string symb);
  bool bp(std::string symb);
  double fp(std::string symb);
  std::vector<double> vctp(std::string symb);
  std::string sp(std::string symb);
  std::vector<std::string> svct(std::string symb);
  void fval_p(std::string symb, double val, std::string data_def = "prop");
  void vct_fp(std::string symb, std::vector<double> vct);
  void vct_sp(std::string symb, std::vector<std::string> vct);
};

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
                              std::string sys_def, std::string data_id);

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
                         std::string sys_def, std::string data_id);

/**
 * @brief function to get parameter values from database
 *
 * @param par vector of parameters to get
 * @param sys_type system type of the parameter to look for
 * @param sys_def system name of the parameter to look for
 * @param input_file parameter name to file containing the parameters of the
 * system
 */
void get_parameters(std::vector<parameter> &par, std::string sys_type,
                    std::string sys_def, std::string input_file);

/**
 * @brief function to export parameter values to a text file
 *
 * @param obj object containing parameters to export
 */
void export_output_parameters(object &obj, std::string file);

/**
 * @brief function printing one parameter
 *
 * @param p parameter to print out
 */
void print_parameter(parameter &p);

/**
 * @brief function printing all parameter of one object
 *
 * @param obj object with the parameters to print out
 */
void print_parameters(object &obj);

/**
 * @brief function to transfer a parameter between objects
 *
 * @param symb parameter name to populate
 * @param from object to transfer from
 * @param to object to transfer to
 */
void transfer_parameter(std::string symb, object from, object &to);
