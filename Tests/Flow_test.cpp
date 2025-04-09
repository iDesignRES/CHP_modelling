#include "../src/bioCHP.cpp"
#include <iomanip>  // for setprecision

// using namespace std;

int main() {

  flow fg("flue_gas", "flue_gas");

    size_t O2g = index_species(fg.j, "O2");
    size_t N2g = index_species(fg.j, "N2");
    size_t CO2g = index_species(fg.j, "CO2");
    size_t H2Og = index_species(fg.j, "H2O");

    fg.F.T = 500.0;
    fg.F.P = 1.01325;
    fg.F.VN = 100.0;

    fg.j[O2g].X = 0.1;
    fg.j[N2g].X = 0.7;
    fg.j[CO2g].X = 0.1 ;
    fg.j[H2Og].X = 0.1;

    fg.molec_def = "X";
    fg.calculate_flow("PT");

    fg.print_flow();	

}
