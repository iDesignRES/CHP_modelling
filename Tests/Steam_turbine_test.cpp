#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include "../src/Libraries/Processes_library/Rankine_cycle.h"
#include "../src/Libraries/Thermodynamic_library/Library_Water_Correlations.h"
#include "test_utils.h"

int main() {
  flow bfw("bfw", "water");
  bfw.F.T = 105.0;
  bfw.P.h = hTWater(bfw.F.T);
  flow in("in", "steam");
  flow out("out", "steam");
  in.F.M = 1.0;
  in.F.P = 25.0;

  in.F.T = TSatWater(in.F.P);
  steam_turbine_parameters ST;
  ST.mu_isent = 0.5;
  ST.qi = 0.99;
  ST.Po = 0.1;
  steam_turbine(in, out, ST);

  std::cout << "steam turbine stage with qi < 1 " << std::endl;
  object par;
  par.fval_p("Po", 0.1);
  par.fval_p("mu_isent", 0.5);
  par.fval_p("q_stm", 1e5);
  par.fval_p("Q_stm", 1e5);
  steam_turbine_model(in, out, par);
  std::cout << "W_el (W): " << par.fp("W_el") << std::endl;

  std::cout << "steam turbine stage with qi = 1 and Po = 10, 1, 0.1 "
            << std::endl;
  in.F.T = 400.0;
  ST.qi = 1.0;
  std::vector<double> Po = {10.0, 1.0, 0.1};
  std::vector<double> expected_W = {111166.522855557560, 310849.179846769257,
                                    456951.741239636904};

  for (std::size_t n = 0; n < Po.size(); n++) {
    ST.Po = Po[n];
    steam_turbine(in, out, ST);
    std::cout << "W_el (W): " << ST.W << std::endl;
    assert(approxEqual(ST.W, expected_W[n]) || mismatch("W", ST.W));
  }

  par = object();
  std::vector<std::string> bleeds = {"1st", "2nd"};
  std::vector<double> Pb = {15.0, 5.0};
  std::vector<double> Mb = {0.0, 0.0};
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
  [[maybe_unused]] double expected_Wel = 496341.464455725218;
  steam_turbine_model(in, out, par);
  std::cout << "W_el (W): " << par.fp("W_el") << std::endl;
  assert(approxEqual(par.fp("W_el") * 1e6, expected_Wel) ||
         mismatch("W_el", par.fp("W_el") * 1e6));
  // export_output_parameters(par, "steam_turbine_test");

  par.remove("W_el");
  in.P.h = hPTSupSteam(in.F.P, in.F.T);
  par.fval_p("q_stm", (in.P.h - bfw.P.h) * 1e3);           // J/kg
  par.fval_p("Q_stm", in.F.M * (in.P.h - bfw.P.h) * 1e3);  // J/kg
  in.F.M = 0.0;
  steam_turbine_model(in, out, par);
  assert(approxEqual(par.fp("W_el") * 1e6, expected_Wel) ||
         mismatch("W_el", par.fp("W_el") * 1e6));

  return 0;
}
