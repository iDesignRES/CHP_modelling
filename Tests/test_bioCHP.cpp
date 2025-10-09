#include <cassert>
#include <cstddef>
#include <iomanip>  // for setprecision (for printing with cout)
#include <iostream>

#include <filesystem>
#include <fstream>
#include "../src/bioCHP.h"
#include "../src/utils.h"
#include "test_utils.h"

void test_bioCHP(const std::string& input_file) {
  toml::table tbl = get_toml_table(input_file);

  object bioCHP;
  try {
    bioCHP = bioCHP_plant(tbl);
  } catch (const std::exception&) {
    if (tbl.contains("expected")) {
      throw;
    } else {
      assert(true);
      return;
    }
  }
  double Q_prod, W_el_prod, C_inv, C_op, C_op_var;
  std::vector<double> Mj;
  extract_outputs(bioCHP, &Q_prod, &W_el_prod, &C_inv, &C_op, &C_op_var, Mj);

  std::cout << std::fixed << std::setprecision(12);

  const auto& expected = *tbl["expected"].as_table();
  std::vector<double> expected_Mj;
  for (const auto& val : *expected["Mj"].as_array())
    expected_Mj.push_back(val.value<double>().value_or(0.0));

  double expected_Q_prod = expected["Q_prod"].value<double>().value_or(0.0);
  double expected_W_el_prod =
      expected["W_el_prod"].value<double>().value_or(0.0);
  double expected_C_inv = expected["C_inv"].value<double>().value_or(0.0);
  double expected_C_op = expected["C_op"].value<double>().value_or(0.0);
  double expected_C_op_var = expected["C_op_var"].value<double>().value_or(0.0);

  for (std::size_t nj = 0; nj < Mj.size(); nj++)
    assert(approxEqual(Mj[nj], expected_Mj[nj]) ||
           mismatch("Mj[" + std::to_string(nj) + "]", Mj[nj]));

  assert(approxEqual(Q_prod, expected_Q_prod) || mismatch("Q_prod", Q_prod));
  assert(approxEqual(W_el_prod, expected_W_el_prod) ||
         mismatch("W_el_prod", W_el_prod));
  assert(approxEqual(C_inv, expected_C_inv) || mismatch("C_inv", C_inv));
  assert(approxEqual(C_op, expected_C_op) || mismatch("C_op", C_op));
  assert(approxEqual(C_op_var, expected_C_op_var) ||
         mismatch("C_op_var", C_op_var));
}

int main() {
  namespace fs = std::filesystem;

  for (const auto& entry : fs::directory_iterator("../Tests/input_files")) {
    if (entry.is_regular_file() && entry.path().extension() == ".toml") {
      std::cout << "Running test for: " << entry.path() << std::endl;
      test_bioCHP(entry.path().string());
    }
  }

  return 0;
}