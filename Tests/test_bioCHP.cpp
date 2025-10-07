#include <cstddef>
#include <iomanip>  // for setprecision
#include <iostream>

#include <filesystem>
#include <fstream>
#include "../src/bioCHP.h"
#include "../src/utils.h"
#include "test_utils.h"

bool test_bioCHP(const std::string& input_file) {
  toml::table tbl = get_toml_table(input_file);

  object bioCHP;
  try {
    bioCHP = bioCHP_plant(tbl);
  } catch (const std::exception&) {
    if (tbl.contains("expected")) {
      throw;
    } else {
      std::cout << "Successfully generated the error." << std::endl;
      return true;
    }
  }
  double Q_prod, W_el_prod, C_inv, C_op, C_op_var;
  std::vector<double> Mj;
  extract_outputs(bioCHP, &Q_prod, &W_el_prod, &C_inv, &C_op, &C_op_var, Mj);

  bool all_passed = true;

  std::cout << std::fixed << std::setprecision(6);

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

  for (std::size_t nj = 0; nj < Mj.size(); nj++) {
    if (!approxEqual(Mj[nj], expected_Mj[nj])) {
      all_passed = false;
      std::cout << "Mj[" << nj << "] failed: expected " << expected_Mj[nj]
                << ", got " << Mj[nj] << std::endl;
    }
  }

  if (!approxEqual(Q_prod, expected_Q_prod)) {
    all_passed = false;
    std::cout << "Q_prod failed: expected " << expected_Q_prod << ", got "
              << Q_prod << std::endl;
  }

  if (!approxEqual(W_el_prod, expected_W_el_prod)) {
    all_passed = false;
    std::cout << "W_el_prod failed: expected " << expected_W_el_prod << ", got "
              << W_el_prod << std::endl;
  }

  if (!approxEqual(C_inv, expected_C_inv)) {
    all_passed = false;
    std::cout << "C_inv failed: expected " << expected_C_inv << ", got "
              << C_inv << std::endl;
  }

  if (!approxEqual(C_op, expected_C_op)) {
    all_passed = false;
    std::cout << "C_op failed: expected " << expected_C_op << ", got " << C_op
              << std::endl;
  }

  if (!approxEqual(C_op_var, expected_C_op_var)) {
    all_passed = false;
    std::cout << "C_op_var failed: expected " << expected_C_op_var << ", got "
              << C_op_var << std::endl;
  }

  if (all_passed) {
    std::cout << "Test pass!" << std::endl;
  } else {
    std::cout << "Test failed!" << std::endl;
  }

  return all_passed;
}

int main() {
  namespace fs = std::filesystem;
  bool tests_passed = true;

  for (const auto& entry : fs::directory_iterator("../Tests/input_files")) {
    if (entry.is_regular_file() && entry.path().extension() == ".toml") {
      std::cout << "Running test for: " << entry.path() << std::endl;
      tests_passed = tests_passed && test_bioCHP(entry.path().string());
    }
  }

  return tests_passed ? 0 : 1;
}