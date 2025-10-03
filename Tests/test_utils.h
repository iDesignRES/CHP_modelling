#pragma once

#include <cmath>  // for fabs

bool approxEqual(double a, double b, double tol = 1e-4) {
  return std::fabs(a - b) < tol;
}