#include <iomanip>  // for setprecision

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#include "../src/Libraries/Thermodynamic_library/Library_Water_Correlations.h"
#include "../src/Libraries/Thermodynamic_library/species_thermodynamics.h"
#include "../src/Parameters.h"
#include "../src/bioCHP.h"

int main() {
  std::cout << "NIST thermodynamics test" << std::endl;
  double TK1 = 25.0 + 273.15, TK2 = 1500.0 + 273.15;
  std::vector<double> P = {1.01325};
  std::vector<double> TK = {TK1, TK2};
  std::vector<std::string> p = {"cp", "h", "hf", "s"};
  std::vector<std::string> u = {"J/mol*K", "J/mol", "J/mol", "J/mol*K"};
  std::vector<std::string> s = {"CO2", "CO",  "H2O", "H2",  "CH4",
                                "N2",  "O2",  "SO2", "SO3", "HCl",
                                "Cl2", "NH3", "NO2", "N2O4"};

  for (std::size_t np = 0; np < P.size(); np++) {
    for (std::size_t nt = 0; nt < TK.size(); nt++) {
      std::cout << "P (bar-g) = " << P[np] << " , T (K) = " << TK[nt]
                << std::endl;
      for (std::size_t ns = 0; ns < s.size(); ns++) {
        std::cout << "Species: " << s[ns] << std::endl;
        for (std::size_t npr = 0; npr < p.size(); npr++) {
          std::cout << '\t' << p[npr] << ", " << u[npr] << " = "
                    << thermodynamic_property(s[ns], p[npr], TK[nt])
                    << std::endl;
        }
      }
    }
  }

  std::cout << "Water thermodynamics correlations test" << std::endl;
  P.push_back(15.0);
  P.push_back(30.0);
  P.push_back(50.0);
  P.push_back(90.0);

  for (std::size_t np = 0; np < P.size(); np++) {
    std::cout << "P (bar-g) = " << P[np]
              << " , T sat (deg.C) = " << TSatWater(P[np]) << std::endl;
    double Tsat = TSatWater(P[np]);
    TK1 = TSatWater(P[np]) - 5.0, TK2 = TSatWater(P[np]) - 5.0;
    std::cout << "hPWater[kJ/kg] = " << hPWater(P[np]) << std::endl;
    std::cout << "hTWater[kJ/kg] = " << hTWater(TK1) << std::endl;
    std::cout << "HTSteam[kJ/kg] = " << HTSteam(TK2) << std::endl;
    std::cout << "PSatWater = " << PSatWater(TK1) << std::endl;
    std::cout << "hPTSupSteam[kJ/kg] = " << hPTSupSteam(P[np], TK2)
              << std::endl;
    double h = hPTSupSteam(P[np], TK2);
    std::cout << "sPhSupSteam[kJ/kg*K] = " << sPhSupSteam(P[np], h)
              << std::endl;
    std::cout << "TPhSupSteam = " << TPhSupSteam(P[np], h) << std::endl;
    std::cout << "hPSatSteam[kJ/kg] = " << hPSatSteam(P[np]) << std::endl;
    std::cout << "sPTSupSteam[kJ/kg*K] = " << sPTSupSteam(P[np], TK2)
              << std::endl;
    std::cout << "sPWater[kJ/kg*K] = " << sPWater(P[np]) << std::endl;
    std::cout << "sPSatSteam[kJ/kg*K] = " << sPSatSteam(P[np]) << std::endl;
    std::cout << "sTWater[kJ/kg*K] = " << sTWater(TK1) << std::endl;
    std::cout << "vTWater[m3/kg] = " << vTWater(TK1) << std::endl;
    std::cout << "vTSteam[m3/kg] = " << vTSteam(TK2) << std::endl;
    std::cout << "HVapH2O[kJ/kg] = " << HVapH2O(Tsat) << std::endl;
    std::cout << "CpWater[kJ/kg*K] = " << CpWater(TK1) << std::endl;
  }

  return 0;
}
