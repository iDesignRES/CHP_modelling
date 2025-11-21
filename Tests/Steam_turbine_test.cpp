#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include "../src/Libraries/Processes_library/Rankine_cycle.h"
#include "test_utils.h"

int main() {
  flow in("in", "steam");

  in.F.M = 1.0;
  in.F.T = 400.0;
  in.F.P = 25.0;
  steam_turbine_parameters ST;
  ST.mu_isent = 0.5;
  std::vector<double> Po = {10.0, 1.0, 0.1};
  std::vector<double> expected_W = {111166.480514052819, 311805.848024832609,
                                    457608.539262635109};

  for (std::size_t n = 0; n < Po.size(); n++) {
    flow out("out", "steam");
    ST.Po = Po[n];
    steam_turbine(in, out, ST);
    std::cout << "W_el (W): " << ST.W << std::endl;
    assert(approxEqual(ST.W, expected_W[n]) || mismatch("W", ST.W));
  }

  std::vector<std::string> bleeds = {"1st", "2nd"};
  std::vector<double> Pb = {15.0, 5.0};
  std::vector<double> Mb = {0.0, 0.0};
  object par;
  par.p.push_back(
      parameter("process", "steam_turbine", "output", "name", "str", "ST"));
  par.fval_p("Po", 0.1);
  par.fval_p("mu_isent", 0.5);
  par.vct_sp("bleeds", bleeds);
  par.vct_fp("P_bleed", Pb);
  par.vct_fp("M_bleed", Mb);
  par.vct_sp("bleeds", bleeds, "output");
  par.vct_fp("P_bleed", Pb);
  par.vct_fp("M_bleed", Mb);
  [[maybe_unused]] double expected_Wel = 496858.635288289865;
  flow out("out", "steam");
  steam_turbine_model(in, out, par);
  assert(approxEqual(par.fp("W_el") * 1e6, expected_Wel) ||
         mismatch("W_el", par.fp("W_el") * 1e6));
  export_output_parameters(par, "steam_turbine_test");
  return 0;
}
