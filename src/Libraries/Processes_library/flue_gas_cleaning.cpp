#include "flue_gas_cleaning.h"
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#include "../../Cost.h"

void dry_scrubber_model(flow &in, flow &out, object &par) {
  /*dry scrubber model, described in docs/bioCHP_model_description.md */

  /*define Temperature and pressure conditions
  for the input flue gas based on
  specified inputs T_op (deg.C) and P_op (bar-g) */
  in.F.T = par.fp("T_op");
  in.F.P = par.fp("P_op");
  in.calculate_flow();

  /*Create and specify lime as consumble
   Annual mass consumption = 14.44 kg / ton solid fuel,
   from Stubenvoll et al. 2002  */

  par.c.push_back(object("consumable", "lime"));
  std::size_t lime = par.ic("consumable", "lime");
  par.c[lime].fval_p("Q_annual", par.fp("M_fuel") * 14.44 * 3.6 * 8000);
  material_cost(par.c[lime]);

  /*Create and calculate cake as solid residue
   assumed that cake and lime quantities as equal    */

  par.c.push_back(object("solid_residue", "scrubber_cake"));
  std::size_t cake = par.ic("solid_residue", "scrubber_cake");
  par.c[cake].fval_p("Q_annual", par.c[lime].fp("Q_annual"));
  material_cost(par.c[cake]);

  out = in;

  /* defining and specifying the dry scrubber equipment. */

  par.c.push_back(object("equipment", "dry_scrubber"));
  std::size_t scrubber = par.ic("equipment", "dry_scrubber");
  par.c[scrubber].fval_p("S", in.F.M * 3.6);
  par.c[scrubber].fval_p("W_el",
                         par.fp("M_fuel") * 3.6 * par.c[scrubber].fp("w_el"));

  equipment_cost(par.c[scrubber]);
}
