#pragma once

/**
 * @brief Specific enthalpy [kJ/kg] of water
 * @param P = Pressure (bar-a)
 */
double hPWater(double P);

/**
 * @brief Specific enthalpy [kJ/kg] of water
 * @param T = Temperature (deg. C)
 */
double hTWater(double T);

/**
 * @brief Saturation temperature (deg. C) of water
 * @param P = Pressure (bar-a)
 */
double TSatWater(double P);

/**
 * @brief Saturation pressure [bar-a] of water
 * @param T = Temperature (deg. C)
 */
double PSatWater(double T);

/**
 * @brief Specific entropy [kJ/kg K] of superheated steam
 * @param P = Pressure (bar-a)
 * @param H = Specific enthalpy (kJ/kg)
 */
double sPhSupSteam(double P, double H);

/**
 * @brief Temperature [deg.C] of superheated steam
 * @param P = Pressure (bar-a)
 * @param H = Specific enthalpy (kJ/kg)
 */
double TPhSupSteam(double P, double H);

/**
 * @brief Specific entalpy [kJ/kg] of superheated steam
 * @param P = Pressure (bar-a)
 * @param Temp = Temperature (deg.C)
 */
double hPTSupSteam(double P, double Temp);

/**
 * @brief Specific entalpy [kJ/kg] of saturated steam
 * @param P = Pressure (bar-a)
 */
double hPSatSteam(double P);

/**
 * @brief Specific entropy [kJ/kg K] of superheated steam
 * @param P = Pressure (bar-a)
 * @param T_in = Temperature (deg.C)
 */
double sPTSupSteam(double P, double T_in);

/**
 * @brief Specific entropy [kJ/kg K] of water
 * @param P = Pressure (bar-a)
 */
double sPWater(double P);

/**
 * @brief Specific entropy [kJ/kg K] of saturated steam
 * @param P = Pressure (bar-a)
 */
double sPSatSteam(double P);

/**
 * @brief Specific entropy [kJ/kg K] of water
 * @param T = Temperature (deg.C)
 */
double sTWater(double T);

/**
 * @brief Specific enthalpy [kJ/kg] of steam
 * @param T = Temperature (deg.C)
 */
double HTSteam(double T);

/**
 * @brief Specific volume [m3/kg] of water
 * @param T = Temerature (deg.C)
 */
double vTWater(double T);

/**
 * @brief Specific volume [m3/kg] of steam
 * @param T = Temerature (deg.C)
 */
double vTSteam(double T);

/**
 * @brief Vaporization heat [kJ/kg] of water
 * @param T = Temerature (deg.C)
 */
double HVapH2O(double T);

/**
 * @brief Specific heat [kJ/kg K] of water
 * @param T = Temerature (deg.C)
 */
double CpWater(double T);
