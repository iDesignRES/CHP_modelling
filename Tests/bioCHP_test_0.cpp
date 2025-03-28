//#include "../src/bioCHP.cpp"
#include <string.h>
#include <iostream>
#include <vector>

using namespace std;

int main() {
  vector<string> fuel_def = {"spruce_chips"};
  vector<double> Yj = {1.0};
  double W_el = 300.0;
  vector<double> Qk = {};
  vector<double> Tk_in = {};
  vector<double> Tk_out = {};

  vector<double> Mj;
  double C_inv;
  double C_op;

  for (size_t nj = 0; nj < Yj.size(); nj++) {
      cout << "Y_" + fuel_def[nj] + " = " << Yj[nj] << endl;
  }

}
