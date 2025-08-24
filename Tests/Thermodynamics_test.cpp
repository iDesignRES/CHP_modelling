#include <iomanip>  // for setprecision

#include "../src/bioCHP.cpp"

// using namespace std;

int main() {
  cout << "NIST thermodynamics test" << endl;
  double TK1 = 25.0 + 273.15, TK2 = 1500.0 + 273.15;
  vector<double> P = {1.01325};
  vector<double> TK = {TK1, TK2};
  vector<string> p = {"cp","h","hf","s"};
  vector<string> u = {"J/mol*K","J/mol","J/mol","J/mol*K"};
  vector<string> s = {"CO2","CO","H2O","H2","CH4","N2","O2",
    "SO2","SO3","HCl","Cl2","NH3","NO2","N2O4"};

  for(size_t np = 0; np < P.size(); np++){
  for(size_t nt = 0; nt < TK.size(); nt++){
    cout << "P (bar-g) = " << P[np] << " , T (K) = " << TK[nt] << endl;
    for(size_t ns = 0; ns < s.size(); ns++){
      cout << "Species: " << s[ns] << endl; 		
      for(size_t npr = 0; npr < p.size(); npr++){
        cout << '\t' << p[npr] << ", " << u[npr] << " = " << 		
        thermodynamic_property(s[ns], p[npr], TK[nt], P[np], "J/mol*K") << endl;	
      }
    }
  }
  }	  	

  cout << "Water thermodynamics correlations test" << endl;
  P.push_back(15.0);	
  P.push_back(30.0);	
  P.push_back(50.0);
  P.push_back(90.0);	

  for(size_t np = 0; np < P.size(); np++){
    cout << "P (bar-g) = " << P[np] << " , T sat (deg.C) = " << TSatWater(P[np]) << endl;
    double Tsat = TSatWater(P[np]); 
    TK1 = TSatWater(P[np]) - 5.0, TK2 = TSatWater(P[np]) - 5.0;
    cout << "hPWater[kJ/kg] = " << hPWater(P[np]) << endl; 
    cout << "hTWater[kJ/kg] = " << hTWater(TK1) << endl; 
    cout << "HTSteam[kJ/kg] = " << HTSteam(TK2) << endl; 
    cout << "PSatWater = " << PSatWater(TK1) << endl; 
    cout << "hPTSupSteam[kJ/kg] = " << hPTSupSteam(P[np], TK2) << endl;
    double h = hPTSupSteam(P[np], TK2); 
    cout << "sPhSupSteam[kJ/kg*K] = " << sPhSupSteam(P[np], h) << endl; 
    cout << "TPhSupSteam = " << TPhSupSteam(P[np], h) << endl; 
    cout << "hPSatSteam[kJ/kg] = " << hPSatSteam(P[np]) << endl; 
    cout << "sPTSupSteam[kJ/kg*K] = " << sPTSupSteam(P[np], TK2) << endl; 
    cout << "sPWater[kJ/kg*K] = " << sPWater(P[np]) << endl; 
    cout << "sPSatSteam[kJ/kg*K] = " << sPSatSteam(P[np]) << endl; 
    cout << "sTWater[kJ/kg*K] = " << sTWater(TK1) << endl; 
    cout << "vTWater[m3/kg] = " << vTWater(TK1) << endl; 
    cout << "vTSteam[m3/kg] = " << vTSteam(TK2) << endl; 
    cout << "HVapH2O[kJ/kg] = " << HVapH2O(Tsat) << endl; 
    cout << "CpWater[kJ/kg*K] = " << CpWater(TK1) << endl;
  } 
}
