#pragma once

/**
 * @brief Specific enthalpy [kJ/kg] of water
 * @param P = Pressure (bar-a)
 * @param A,B,C,D,E,f,G,H,i constants to evaluate the function
 */
double hPWater(double P);

/**
 * @brief Specific enthalpy [kJ/kg] of water
 * @param T = Temperature (deg. C)
 * @param A,B,C,D,E,f,G,H,i,j constants to evaluate the function
 */
double hTWater(double T);

/**
 * @brief Saturation temperature (deg. C) of water
 * @param P = Pressure (bar-a)
 * @param A,B,C,D,E,f,G,H,i,j,k constants to evaluate the function
 */
double TSatWater(double P);

/**
 * @brief Saturation pressure [bar-a] of water
 * @param T = Temperature (deg. C)
 * @param A,B,C,D,E,f,G,H,i,j,k constants to evaluate the function
 */
double PSatWater(double T);

/**
 * @brief Specific entropy [kJ/kg K] of superheated steam
 * @param P = Pressure (bar-a)
 * @param H = Specific enthalpy (kJ/kg)
 * @param A,B,C,D,E,f,G,H,i,j,k constants to evaluate the function
 */
double sPhSupSteam(double P, double H);

/**
 * @brief Temperature [deg.C] of superheated steam
 * @param P = Pressure (bar-a)
 * @param H = Specific enthalpy (kJ/kg)
 * @param A,B,C,D,E,f,G,U,i,j constants to evaluate the function
 */
double TPhSupSteam(double P, double H);

/**
 * @brief Specific entalpy [kJ/kg] of superheated steam
 * @param P = Pressure (bar-a)
 * @param Temp = Temperature (deg.C)
 * @param A,B,C,D,E,f,G,H,i,j,T constants to evaluate the function
 */
double hPTSupSteam(double P, double Temp);

/**
 * @brief Specific entalpy [kJ/kg] of saturated steam
 * @param P = Pressure (bar-a)
 * @param A,B,C,D,E,f,G,H,i,j constants to evaluate the function
 */
double hPSatSteam(double P);

/**
 * @brief Specific entropy [kJ/kg K] of superheated steam
 * @param P = Pressure (bar-a)
 * @param T_in = Temperature (deg.C)
 * @param A,B,C,D,E,f,G,H,i,j constants to evaluate the function
 */
double sPTSupSteam(double P, double T_in);

/**
 * @brief Specific entropy [kJ/kg K] of water
 * @param P = Pressure (bar-a)
 * @param A,B,C,D,E,f,G,H,i,j,k constants to evaluate the function
 */
double sPWater(double P);

/**
 * @brief Specific entropy [kJ/kg K] of saturated steam
 * @param P = Pressure (bar-a)
 * @param A,B,C,D,E,f,G,H,i,j constants to evaluate the function
 */
double sPSatSteam(double P);

/**
 * @brief Specific entropy [kJ/kg K] of water
 * @param T = Temperature (deg.C)
 * @param A,B,C,D,E,f,G,H,i,j,k constants to evaluate the function
 */
double sTWater(double T);

/**
 * @brief Specific enthalpy [kJ/kg] of steam
 * @param T = Temperature (deg.C)
 * @param A,B,C,D,E,f,G,H,i constants to evaluate the function
 */
double HTSteam(double T);

/**
 * @brief Specific volume [m3/kg] of water
 * @param T = Temperature (deg.C)
 * @param A,B,C,D,E constants to evaluate the function
 */
double vTWater(double T);

/**
 * @brief Specific volume [m3/kg] of steam
 * @param T = Temperature (deg.C)
 * @param A,B,C,D,E,f constants to evaluate the function
 */
double vTSteam(double T);

/**
 * @brief Vaporization heat [kJ/kg] of water
 * @param T = Temperature (deg.C)
 */
double HVapH2O(double T);

/**
 * @brief Specific heat [kJ/kg K] of water
 * @param T = Temperature (deg.C)
 */
double CpWater(double T);
