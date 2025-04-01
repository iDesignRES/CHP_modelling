#include "../src/bioCHP.cpp"

//using namespace std;

int main() {
  vector<string> fuel_def = {"spruce_chips"};
  vector<double> Yj = {1.0};
  vector<double> YH2Oj = {0.3};
  double W_el = 300.0;
  vector<double> Qk = {};
  vector<double> Tk_in = {};
  vector<double> Tk_out = {};

  vector<double> Mj;
  double Q_prod;
  double W_el_prod;
  double C_inv;
  double C_op;
  double C_op_var;

  if (bioCHP_plant(fuel_def, Yj, YH2Oj, W_el, Qk, Tk_in, Tk_out, Mj, Q_prod, W_el_prod, C_inv, C_op, C_op_var)) {
    for (int nj = 0; nj < Mj.size(); nj++) {
      cout << "M_" + fuel_def[nj] + " = " << Mj[nj] << endl;
    }
    cout << "Q_prod (MW) = " << Q_prod << endl;
    cout << "W_el_prod (MW) = " << W_el_prod << endl;
    cout << "C_inv (M$) = " << C_inv << endl;
    cout << "C_op (M$) = " << C_op << endl;
    cout << "C_op_var (M$) = " << C_op_var << endl;

  } else {
    cout << "error" << endl;
  }
}
