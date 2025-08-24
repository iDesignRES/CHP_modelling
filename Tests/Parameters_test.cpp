#include <iomanip>  // for setprecision

#include "../src/bioCHP.cpp"

// using namespace std;

int main() {

  parameter p("prop plant bioCHP_PLANT T_stm num 450.0");
  print_parameter(p);

  vector<string> fuel_def = {"spruce_chips", "spruce_T&B"};
  vector<double> Yj = {0.5};
  vector<double> YH2Oj = {0.3, 0.6};
  double W_el = 300.0000;
  vector<double> Qk = {30.0, 40.0};
  vector<double> Tk_in = {70.0, 70.0};
  vector<double> Tk_out = {90.0, 90.0};

  object bioCHP("plant", "bioCHP_PLANT", DIR + "Database/bioCHP_inputs");

  bioCHP.vct_sp("fuel_def", fuel_def);
  bioCHP.vct_fp("Yj", Yj);
  bioCHP.vct_fp("YH2Oj", YH2Oj);
  bioCHP.fval_p("W_el", W_el);
  bioCHP.vct_fp("Qk", Qk);
  bioCHP.vct_fp("Tk_in", Tk_in);
  bioCHP.vct_fp("Tk_out", Tk_out);

  cout << "index of Qk = " << bioCHP.ip("Qk") << endl;

  print_parameters(bioCHP);

  object CHP("plant", "CHP");
  transfer_parameter("Qk",bioCHP,CHP);
  str_p(CHP.p, "prop", "plant", "CHP", "land","Norway");
  CHP.sval_p("city", "Trondheim");
  print_parameters(CHP);

}
