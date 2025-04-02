//#include "../src/bioCHP.cpp"
#include <string.h>
#include <iostream>
#include <vector>

using namespace std;

vector<string> divide_string(string str, char c) {
  // cout << "divide " << str << endl;
  vector<string> list;
  vector<char> cstr(str.begin(), str.end());
  string element = "";
  bool element_found = false;
  int l = 0;
  while (l < str.length()) {
    if (cstr[l] != c) {
      element = element + cstr[l];
    }
    if (cstr[l] == c) {
      list.push_back(element);
      element = "";
    }
    if (l == str.length() - 1) {
      list.push_back(element);
      break;
    }
    // cout << l << " " << cstr[l] << " " << element << endl;
    l = l + 1;
  }
  cstr.clear();
  return list;
}

#include "../src/pathfinder.h"
using namespace MyPaths;
std::string DIR = getActualDir(string(__FILE__));
std::string project = project_name();

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
