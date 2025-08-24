#include <iomanip>  // for setprecision

#include "../src/bioCHP.cpp"

// using namespace std;

int main() {
  vector<string> fuel_def = {"spruce_chips", "spruce_T&B"};
  vector<double> Yj = {0.5, 0.5};
  vector<double> YH2Oj = {0.3, 0.6};
  double W_el = 300.0000;
  vector<double> Qk = {30.0, 40.0};
  vector<double> Tk_in = {70.0, 70.0, 70.0};
  vector<double> Tk_out = {90.0, 90.0};

  vector<double> Mj;
  double Q_prod;
  double W_el_prod;
  double C_inv;
  double C_op;
  double C_op_var;

  if (bioCHP_plant(fuel_def, Yj, YH2Oj, W_el, Qk, Tk_in, Tk_out, Mj, Q_prod,
                   W_el_prod, C_inv, C_op, C_op_var)) {
    cout << "No errors" << endl;

  } else {
    cout << "error" << endl;
  }
}
