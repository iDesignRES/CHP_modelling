#pragma once

#include <string>

/**
 * @brief Shomate function defined by NIST to calculate thermodynamic properties
 *
 * @param tK = Temperatre (K) / 1000.0
 * @param property, string specifying a property
 *	"cp" = specific molar heat at constant pressure
 *	"h" = specific molar enthalpy
 *	"s" = specific molar entropy
 * @param A,B,C,D,E,F,G constants of the Shomate function
 * @param Hf298 specific molar formation enthalpy
 */
double shomate(std::string property, double tK, double A, double B, double C,
         double D, double E, double F, double G, double Hf298);

/**
 * @brief Function to calculate thermodynamic properties of one species
 *
 * @param species, string specifying the name of the species
 * @param property, string specifying a property
 *	"cp" = specific molar heat at constant pressure
 *	"h" = specific molar enthalpy
 *	"s" = specific molar entropy
 * @param TK Temperature (K)
 * @param unit, string specifying the units of the property
 */
double thermodynamic_property(std::string species, std::string property, double TK,
               std::string unit);
