#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#include "../src/Libraries/Flows_library/flow_definitions.h"

int main() {
  flow f1("f1", "spruce_chips");
  flow f2("f2", "birch_TandB");
  flow f3("CO2");
  flow f4("dry_air");
  f4.molec_def = "X";
  flow f5("dry_air");
  f5.molec_def = "X";
  flow f6("dry_air");
  f6.molec_def = "X";
  std::vector<flow> f = {f1, f2, f3, f4, f5, f6};
  for (std::size_t nf = 0; nf < f.size(); nf++) {
    if (nf < 4) {
      f[nf].F.M = 1.0;
    } else if (nf == 4)
      f[nf].F.N = 1.0;
    else if (nf == 5)
      f[nf].F.VN = 1.0;

    f[nf].F.T = 25.0;
    f[nf].F.P = 1.01325;
    f[nf].calculate_flow();
    f[nf].print_flow();
  }

  return 0;
}
