#include <iomanip>  // for setprecision

#include <cstddef>
#include <iostream>

#include "../src/Parameters.h"
#include "../src/bioCHP.h"
#include "../src/utils.h"

parameter construct_parameter(std::string line) {
  parameter p;
  std::stringstream sst(line);
  sst >> p.data_def;
  sst >> p.sys_type;
  sst >> p.sys_def;
  sst >> p.data_id;
  sst >> p.data_type;
  if (p.data_type == "str") {
    std::string txt;
    while (sst >> txt) {
      p.str.push_back(txt);
    }
  }
  if (p.data_type == "num") {
    double val;
    while (sst >> val) {
      p.vct.push_back(val);
    }
  }

  p.pos = 0;
  return p;
}

int main() {
  parameter p = construct_parameter("prop plant bioCHP_PLANT T_stm num 450.0");
  print_parameter(p);

  std::vector<std::string> fuel_def = {"spruce_chips", "spruce_T&B"};
  std::vector<double> Yj = {0.5};
  std::vector<double> YH2Oj = {0.3, 0.6};
  double W_el = 300.0000;
  std::vector<double> Qk = {30.0, 40.0};
  std::vector<double> Tk_in = {70.0, 70.0};
  std::vector<double> Tk_out = {90.0, 90.0};

  object bioCHP("plant", "bioCHP_PLANT",
                get_database_path("bioCHP_inputs.toml"));

  bioCHP.vct_sp("fuel_def", fuel_def);
  bioCHP.vct_fp("Yj", Yj);
  bioCHP.vct_fp("YH2Oj", YH2Oj);
  bioCHP.fval_p("W_el", W_el);
  bioCHP.vct_fp("Qk", Qk);
  bioCHP.vct_fp("Tk_in", Tk_in);
  bioCHP.vct_fp("Tk_out", Tk_out);

  std::cout << "index of Qk = " << bioCHP.ip("Qk") << std::endl;

  print_parameters(bioCHP);

  object CHP("plant", "CHP");
  transfer_parameter("Qk", bioCHP, CHP);
  str_p(CHP.p, "prop", "plant", "CHP", "land", "Norway");
  CHP.sval_p("city", "Trondheim");
  print_parameters(CHP);

  return 0;
}
