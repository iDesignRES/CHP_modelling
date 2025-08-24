#include <iomanip>  // for setprecision

#include "../src/bioCHP.cpp"

// using namespace std;

int main() {
  flow f1("f1","spruce_chips");
  flow f2("f2","spruce_T&B");
  flow f3("dry_air");
  vector<flow> f = {f1,f2,f3};
  for(size_t nf = 0; nf < f.size(); nf++){
    f[nf].F.M = 1.0;
    f[nf].F.T = 25.0;
    f[nf].F.P = 1.01325;
    f[nf].calculate_flow("PT");
    f[nf].print_flow();
  }   	
  f1.initialize_species(f1.i);

  /*flow f4, f5;
  f4.mix_flows(f[0],f[1]);
  f4.print_flow();
  f5.mix_flows(f[0],f[2]);
  f5.print_flow();*/

}
