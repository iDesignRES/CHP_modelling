
void dry_scrubber_model(flow &in, flow &out, object &par) {
  // -------------------------
  // WET SCRUBBER MODEL
  // -------------------------

  in.F.T = par.fp("T_op");
  in.F.P = par.fp("P_op");
  in.calculate_flow("PT");

  par.c.push_back(object("consumable", "lime"));
  int lime = par.ic("consumable", "lime");
  par.c[lime].fval_p(
      "Q_annual",
      par.fp("M_fuel") * 14.44 * 3.6 * 8000);  // 14.44 kg / ton solid fuel
  material_cost(par.c[lime]);

  par.c.push_back(object("solid_residue", "scrubber_cake"));
  size_t cake = par.ic("solid_residue", "scrubber_cake");
  par.c[cake].fval_p("Q_annual", par.c[lime].fp("Q_annual"));
  material_cost(par.c[cake]);

  out = in;

  par.c.push_back(object("equipment", "dry_scrubber"));
  size_t scrubber = par.ic("equipment", "dry_scrubber");
  par.c[scrubber].fval_p("S", in.F.M * 3.6);
  par.c[scrubber].fval_p("W_el",
                         par.fp("M_fuel") * 3.6 * par.c[scrubber].fp("w_el"));

  equipment_cost(par.c[scrubber]);

  /*
  cout << "scrubber inlet M (kg/s): " << in.F.M << endl;
  cout << "scrubber cost (M$): " << par.c[scrubber].fp("Cpi") * 1e-6 << endl;
  cout << "lime cost (M$ annual): " << par.c[lime].fp("C_annual") * 1e-6 <<
  endl; cout << "cake cost (M$ annual): " << par.c[cake].fp("C_annual") * 1e-6
  << endl;
  */
}
