#pragma once

#include <cmath>    // for fabs
#include <iomanip>  // for setprecision
#include <string>

bool approxEqual(double a, double b, double tol = 1e-4) {
  return std::fabs(a - b) < tol;
}

bool mismatch(const std::string& name, double val) {
  std::cout << std::fixed << std::setprecision(12);  // Set higher precision
  std::cout << name << " = " << val << " does not match expected value."
            << std::endl;
  return false;
}
