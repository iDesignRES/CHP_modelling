#include "Cost.h"

#include <cmath>
#include <cstddef>
#include <iostream>
#include "utils.h"

double cecpi(int year_input) {
  if (year_input < 1994) {
    return 268.1;
  }
  if (year_input > 2020) {
    return 668.0;
  }
  switch (year_input) {
    case 1994:
      return 268.1;
    case 1995:
      return 381.1;
    case 1996:
      return 381.7;
    case 1997:
      return 386.5;
    case 1998:
      return 389.5;
    case 1999:
      return 390.6;
    case 2000:
      return 394.1;
    case 2001:
      return 394.3;
    case 2002:
      return 395.6;
    case 2003:
      return 402.0;
    case 2004:
      return 444.2;
    case 2005:
      return 468.2;
    case 2006:
      return 499.6;
    case 2007:
      return 525.4;
    case 2008:
      return 550.8;
    case 2009:
      return 521.9;
    case 2010:
      return 575.4;
    case 2011:
      return 525.4;
    case 2012:
      return 584.6;
    case 2013:
      return 587.3;
    case 2014:
      return 586.77;
    case 2015:
      return 592.0;
    case 2016:
      return 606.0;
    case 2017:
      return 623.5;
    case 2018:
      return 638.1;
    case 2019:
      return 652.9;
    case 2020:
      return 668.0;
  }
  throw std::invalid_argument("CECPI value for year " +
                              std::to_string(year_input) + " not available");
}

double I_cecpi(int year_ref, int year) { return cecpi(year) / cecpi(year_ref); }

void equipment_cost(object &par) {
  double f_inst = par.fp("f_inst");
  double Cpb = par.fp("Cpb");
  double S = par.fp("S");
  double Sb = par.fp("Sb");
  double n = par.fp("n");
  double Cpi = f_inst * Cpb * std::pow(S / Sb, n) *
               I_cecpi(std::stoi(par.sp("base_year")), 2020);
  if (std::isnan(Cpi))
    par.fval_p("Cpi", 0.0);
  else
    par.fval_p("Cpi", Cpi);
}

void material_cost(object &par) {
  double Q_annual = par.fp("Q_annual");
  double price = par.fp("price");
  double C_annual = Q_annual * price;

  if (std::isnan(C_annual))
    par.fval_p("C_annual", 0.0);
  else
    par.fval_p("C_annual", C_annual);
}

void equipment_list(std::vector<equipment> &list, object &par) {
  equipment eq;
  for (std::size_t n = 0; n < par.c.size(); n++) {
    if (par.c[n].sys_type == "equipment") {
      eq.def = par.sys_def + "-" + par.c[n].sys_def;
      eq.Cpi = par.c[n].fp("Cpi");
      double W_el = par.c[n].fp("W_el");
      if (std::isnan(W_el))
        eq.W_el = 0.0;
      else
        eq.W_el = W_el;

      double f_maint = par.c[n].fp("f_maint");
      if (std::isnan(f_maint))
        eq.C_maint = 0.0;
      else
        eq.C_maint = par.c[n].fp("Cpi") * par.c[n].fp("f_maint");

      list.push_back(eq);
      if (std::isnan(W_el))
        par.fval_p("electric_load_" + par.c[n].sys_def, 0.0);
      else
        par.fval_p("electric_load_" + par.c[n].sys_def, W_el);
    }
    if (par.c[n].c.size() > 0) {
      equipment_list(list, par.c[n]);
    }
  }
}

void material_list(std::string type, std::vector<material> &list, object &par) {
  material m;
  for (std::size_t n = 0; n < par.c.size(); n++) {
    if (par.c[n].sys_type == type) {
      m.def = par.sys_def + "-" + par.c[n].sys_def;
      if (par.c[n].bp("Q_annual")) {
        m.Q_annual = par.c[n].fp("Q_annual");
      } else if (!par.c[n].bp("Q_annual")) {
        m.Q_annual = 0.0;
      }

      if (par.c[n].bp("price")) {
        m.price = par.c[n].fp("price");
      } else if (!par.c[n].bp("price")) {
        m.price = 0.0;
      }

      if (par.c[n].bp("C_annual")) {
        m.C_annual = par.c[n].fp("C_annual");
      } else if (!par.c[n].bp("C_annual")) {
        m.C_annual = 0.0;
      }
      list.push_back(m);
    }
    if (par.c[n].c.size() > 0) {
      material_list(type, list, par.c[n]);
    }
  }
}

void print_capex(object &par) {
  print_separation_line();
  std::cout << " Capital costs (M$): " << par.fp("C_inv") * 1e-6 << std::endl;
  print_separation_line();
  std::cout << "C_eq = " << par.fp("C_eq") * 1e-6 << std::endl;
  std::cout << "C_piping = " << par.fp("C_piping") * 1e-6 << std::endl;
  std::cout << "C_el = " << par.fp("C_el") * 1e-6 << std::endl;
  std::cout << "C_I&C = " << par.fp("C_I&C") * 1e-6 << std::endl;
  std::cout << "C_land = " << par.fp("C_land") * 1e-6 << std::endl;
  std::cout << "C_site = " << par.fp("C_site") * 1e-6 << std::endl;
  std::cout << "C_build = " << par.fp("C_build") * 1e-6 << std::endl;
  std::cout << "C_com = " << par.fp("C_com") * 1e-6 << std::endl;
  std::cout << "C_eng = " << par.fp("C_eng") * 1e-6 << std::endl;
  std::cout << "C_dev = " << par.fp("C_dev") * 1e-6 << std::endl;
  std::cout << "C_cont = " << par.fp("C_cont") * 1e-6 << std::endl;
  print_separation_line();
}

void capex(object &par) {
  std::vector<equipment> eq;
  equipment_list(eq, par);

  double C_eq = 0.0, W_el = 0.0, C_eq_maint = 0.0;
  for (std::size_t n = 0; n < eq.size(); n++) {
    C_eq += eq[n].Cpi;
    W_el += eq[n].W_el;
    C_eq_maint += eq[n].C_maint;
  }

  par.fval_p("W_el(kW)", W_el);

  par.fval_p("electric_load", W_el);
  par.fval_p("C_eq", C_eq);
  par.fval_p("C_piping", C_eq * par.fp("f_piping"));
  par.fval_p("C_el", C_eq * par.fp("f_el"));
  par.fval_p("C_I&C", C_eq * par.fp("f_I&C"));
  par.fval_p("C_pi", par.fp("C_eq") + par.fp("C_piping") + par.fp("C_el") +
                         par.fp("C_I&C"));
  par.fval_p("C_land", par.fp("C_pi") * par.fp("f_land"));
  par.fval_p("C_site", par.fp("C_pi") * par.fp("f_site"));
  par.fval_p("C_build", par.fp("C_pi") * par.fp("f_build"));
  par.fval_p("C_com", par.fp("C_pi") * par.fp("f_com"));
  par.fval_p("C_eng", (par.fp("C_pi") + par.fp("C_site") + par.fp("C_build") +
                       par.fp("C_com")) *
                          par.fp("f_eng"));
  par.fval_p("C_dev", (par.fp("C_pi") + par.fp("C_land") + par.fp("C_site") +
                       par.fp("C_build") + par.fp("C_com") + par.fp("C_eng")) *
                          par.fp("f_dev"));
  par.fval_p("C_cont", (par.fp("C_pi") + par.fp("C_land") + par.fp("C_site") +
                        par.fp("C_build") + par.fp("C_com") + par.fp("C_eng") +
                        par.fp("C_dev")) *
                           par.fp("f_cont"));
  par.fval_p("C_inv", par.fp("C_pi") + par.fp("C_land") + par.fp("C_site") +
                          par.fp("C_build") + par.fp("C_com") +
                          par.fp("C_eng") + par.fp("C_dev") + par.fp("C_cont"));
  par.fval_p("C_eq_maint", par.fp("C_eq") * 0.02);
  par.fval_p("C_piping_maint", par.fp("C_piping") * 0.02);
  par.fval_p("C_el_maint", par.fp("C_el") * 0.02);
  par.fval_p("C_I&C_maint", par.fp("C_I&C") * 0.02);
  print_capex(par);
}

void print_opex(object &par, std::vector<material> &m) {
  print_separation_line();
  std::cout << " Operational costs (M$ / year): " << par.fp("C_op") * 1e-6
            << std::endl;
  print_separation_line();
  std::cout << "Materials = " << par.fp("C_op_mat") * 1e-6 << std::endl;
  for (std::size_t n = 0; n < m.size(); n++) {
    std::cout << '\t' << m[n].def << ": " << m[n].C_annual * 1e-6 << std::endl;
  }
  std::cout << "Electricity = " << par.fp("C_op_el") * 1e-6 << std::endl;
  std::cout << "Equipment maintenance = " << par.fp("C_eq_maint") * 1e-6
            << std::endl;
  std::cout << "Piping maintenance = " << par.fp("C_piping_maint") * 1e-6
            << std::endl;
  std::cout << "Electric system maintenance = " << par.fp("C_el_maint") * 1e-6
            << std::endl;
  std::cout << "I&C_maintenance = " << par.fp("C_I&C_maint") * 1e-6
            << std::endl;
  std::cout << "Total maintenance = " << par.fp("C_op_maint") * 1e-6
            << std::endl;
  std::cout << "Insurance and taxes = " << par.fp("C_op_ins") * 1e-6
            << std::endl;
  std::cout << "Administration = " << par.fp("C_op_adm") * 1e-6 << std::endl;
  print_separation_line();
}

void opex(object &par) {
  std::vector<material> m;
  material_list("consumable", m, par);
  material_list("solid_residue", m, par);

  double C_op_mat = 0.0;
  for (std::size_t n = 0; n < m.size(); n++) {
    C_op_mat = C_op_mat + m[n].C_annual;
  }

  par.fval_p("C_op_mat", C_op_mat, "output");

  par.fval_p("C_op_el", par.fp("W_el(kW)") * par.fp("price_electricity") * 8000,
             "output");

  par.fval_p("C_op_maint",
             par.fp("C_eq_maint") + par.fp("C_piping_maint") +
                 par.fp("C_el_maint") + par.fp("C_I&C_maint"),
             "output");

  par.fval_p("C_op_ins", par.fp("C_pi") * par.fp("f_ins"), "output");

  par.fval_p("C_op_adm", par.fp("C_pi") * par.fp("f_adm"), "output");

  par.fval_p("C_op",
             par.fp("C_op_mat") + par.fp("C_op_el") + par.fp("C_op_maint") +
                 par.fp("C_op_ins") + par.fp("C_op_adm"),
             "output");

  par.fval_p("C_op_var", par.fp("C_op_mat") + par.fp("C_op_el"), "output");
  print_opex(par, m);
}

void cost(object &par) {
  capex(par);
  opex(par);
}
