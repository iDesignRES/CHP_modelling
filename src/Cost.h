#pragma once

#include <string>
#include <vector>

#include "Parameters.h"

struct equipment {
 public:
  std::string def;
  double Cpi, W_el, C_maint;
};

struct material {
 public:
  std::string type, def;
  double Q_annual, price, C_annual;
};

double cecpi(int year_input);

double I_cecpi(int year_ref, int year);

void equipment_cost(object &par);

void material_cost(object &par);

void equipment_list(std::vector<equipment> &list, object &par);

void material_list(std::string type, std::vector<material> &list, object &par);

void print_capex(object &par);

void capex(object &par);

void print_opex(object &par, std::vector<material> &m);

void opex(object &par);

void cost(object &par);
