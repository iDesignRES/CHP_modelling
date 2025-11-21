#pragma once

#include <string>
#include <vector>

#include "Parameters.h"

/**
 * @struct equipment
 * @brief Structure to equipment parameters
 * @var equipment::def
 * Defining name in the database
 * @var equipment::Cpi
 * Installed cost
 * @var equipment::W_el
 * Electric load
 * @var equipment::C_maint
 * Maintenance cost
 */
struct equipment {
 public:
  std::string def;
  double Cpi, W_el, C_maint;
};

/**
 * @struct material
 * @brief Structure to define material consumables parameters
 * @var material::type
 * Type of material (consumable, product)
 * @var material::def
 * Defining name in the database
 * @var material::Q_annual
 * Annual quantity
 * @var material::price
 * Unit price
 * @var material::C_annual
 * Annual cost
 */
struct material {
 public:
  std::string type, def;
  double Q_annual, price, C_annual;
};

/**
 * @brief Chemical Engineering Price index
 *
 * @param year_input
 */
double cecpi(int year_input);

/**
 * @brief Function to calculate price index ratio relative to a reference year
 *
 * @param year_ref integer specifying a referece year
 * @param year integer specifying the actual year
 */
double I_cecpi(int year_ref, int year);

/**
 * @brief Function to calculate equipment cost
 *
 * @param par object including the input parameters of the equipment
 */
void equipment_cost(object &par);

/**
 * @brief Function to calculate material cost
 *
 * @param par object including the input parameters of the material
 */
void material_cost(object &par);

/**
 * @brief Function to generate the equipment list of a system
 *
 * @param par object representing a system, including the equipment as
 * sub-objects
 * @param list vector of equipment within the system
 */
void equipment_list(std::vector<equipment> &list, object &par);

/**
 * @brief Function to generate the material flow list of a system
 *
 * @param par object representing a system, including the equipment as
 * sub-objects
 * @param list vector of materials within the system
 */
void material_list(std::string type, std::vector<material> &list, object &par);

/**
 * @brief Function to print capital cost information
 *
 * @param par object representing a system or equipment
 */
void print_capex(object &par);

/**
 * @brief Function to calculate capital cost
 *
 * @param par object representing a system or equipment
 */
void capex(object &par);

/**
 * @brief Function to print operating cost
 *
 * @param par object representing a system or equipment
 */
void print_opex(object &par, std::vector<material> &m);

/**
 * @brief Function to calculate operating cost
 *
 * @param par object representing a system or equipment
 */
void opex(object &par);

/**
 * @brief Function to calculate capital and operating cost
 *
 * @param par object representing a system or equipment
 */
void cost(object &par);
