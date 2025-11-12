#include "bioCHP.h"
#include <cstddef>
#include <iostream>

#include "Libraries/Processes_library/bioCHP_plant.h"
#include "utils.h"

object bioCHP_plant(std::vector<std::string> fuel_def, std::vector<double> Yj,
                    std::vector<double> YH2Oj, double W_el,
                    std::vector<double> Qk, std::vector<double> Tk_in,
                    std::vector<double> Tk_out) {
  print_separation_line();
  std::cout << "Input parameters:" << std::endl;
  std::cout << "fuel_def: ";
  for (const auto& fuel : fuel_def) std::cout << fuel << " ";
  std::cout << std::endl;

  std::cout << "Yj: ";
  for (const auto& y : Yj) std::cout << y << " ";
  std::cout << std::endl;

  std::cout << "YH2Oj: ";
  for (const auto& y : YH2Oj) std::cout << y << " ";
  std::cout << std::endl;

  std::cout << "W_el: " << W_el << std::endl;

  std::cout << "Qk: ";
  for (const auto& q : Qk) std::cout << q << " ";
  std::cout << std::endl;

  std::cout << "Tk_in: ";
  for (const auto& t : Tk_in) std::cout << t << " ";
  std::cout << std::endl;

  std::cout << "Tk_out: ";
  for (const auto& t : Tk_out) std::cout << t << " ";
  std::cout << std::endl;

  // Check specificatins of feedstock
  if (fuel_def.size() != Yj.size() || fuel_def.size() != YH2Oj.size())
    throw std::runtime_error(
        "Number of specifications for fuel_def, Yj and YH2Oj are different");

  // Check specificatins of heat demands
  if (Qk.size() != Tk_in.size() || Qk.size() != Tk_out.size())
    throw std::runtime_error(
        "Number of specifications for Tk_in, Tk_out and Qk are different");

  // Check that Tk_in < Tk_out
  for (std::size_t nk = 0; nk < Tk_in.size(); nk++) {
    if (Tk_in[nk] > Tk_out[nk])
      throw std::runtime_error("return temperature of heat demand no. " +
                               std::to_string(nk) +
                               " is higher than supply temperature");
  }

  // Check that Yj sums to 1
  double sum_Yj = 0.0;
  for (const auto& y : Yj) sum_Yj += y;
  if (std::abs(sum_Yj - 1.0) > 10 * std::numeric_limits<double>::epsilon())
    throw std::runtime_error("Yj must sum to 1");

  // Check that there is sufficient heat available from Rankine cycle
  double sum_Qk = 0.0;
  for (std::size_t nk = 0; nk < Qk.size(); nk++) sum_Qk += Qk[nk];
  if (sum_Qk > 0.5 * (W_el / 0.2)) {
    throw std::runtime_error(
        "There is not sufficient heat available from Rankine cycle to "
        "supply the specified heat demand");
  }

  object bioCHP("plant", "bioCHP_PLANT",
                get_database_path("bioCHP_inputs.toml"));
  bioCHP.vct_sp("fuel_def", fuel_def);
  bioCHP.vct_fp("Yj", Yj);
  bioCHP.vct_fp("YH2Oj", YH2Oj);
  bioCHP.fval_p("W_el", W_el);
  bioCHP.vct_fp("Qk", Qk);
  bioCHP.vct_fp("Tk_in", Tk_in);
  bioCHP.vct_fp("Tk_out", Tk_out);

  bioCHP_plant_model(bioCHP);

  return bioCHP;
}

object bioCHP_plant(const toml::table& tbl) {
  // Parse fuel_def
  std::vector<std::string> fuel_def;
  for (const auto& v : *tbl["inputs"]["fuel_def"].as_array()) {
    fuel_def.push_back(v.value<std::string>().value());
  }

  // Parse Yj
  std::vector<double> Yj;
  for (const auto& v : *tbl["inputs"]["Yj"].as_array()) {
    Yj.push_back(v.value<double>().value());
  }

  // Parse YH2Oj
  std::vector<double> YH2Oj;
  for (const auto& v : *tbl["inputs"]["YH2Oj"].as_array()) {
    YH2Oj.push_back(v.value<double>().value());
  }

  // Parse W_el
  double W_el = tbl["inputs"]["W_el"].value<double>().value();

  // Parse Qk
  std::vector<double> Qk;
  for (const auto& v : *tbl["inputs"]["Qk"].as_array()) {
    Qk.push_back(v.value<double>().value());
  }

  // Parse Tk_in
  std::vector<double> Tk_in;
  for (const auto& v : *tbl["inputs"]["Tk_in"].as_array()) {
    Tk_in.push_back(v.value<double>().value());
  }

  // Parse Tk_out
  std::vector<double> Tk_out;
  for (const auto& v : *tbl["inputs"]["Tk_out"].as_array()) {
    Tk_out.push_back(v.value<double>().value());
  }

  return bioCHP_plant(fuel_def, Yj, YH2Oj, W_el, Qk, Tk_in, Tk_out);
}

object bioCHP_plant(const std::string& input_file) {
  return bioCHP_plant(get_toml_table(input_file));
}

void extract_outputs(object bioCHP, double* Q_prod, double* W_el_prod,
                     double* C_inv, double* C_op, double* C_op_var,
                     std::vector<double>& Mj) {
  if (Q_prod) *Q_prod = bioCHP.fp("Heat_production_(MW)");
  if (W_el_prod) *W_el_prod = bioCHP.fp("Electricity_production_(MW)");
  if (C_inv) *C_inv = bioCHP.fp("C_inv") * 1e-6;
  if (C_op) *C_op = bioCHP.fp("C_op") * 1e-6;
  if (C_op_var) *C_op_var = bioCHP.fp("C_op_var") * 1e-6;
  Mj = bioCHP.vctp("Mj");
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file.toml> [output_file.txt]"
              << std::endl;
    return 1;
  }

  std::string output_file = "output.txt";
  if (argc >= 3) output_file = argv[2];

  object bioCHP = bioCHP_plant(argv[1]);
  export_output_parameters(bioCHP, output_file);
  std::cout << "Results exported to " << output_file << std::endl;

  return 0;
}
