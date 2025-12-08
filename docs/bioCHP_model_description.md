\page detailed_model_documentation_page bioCHP module description

This pages gives a detailed documentation of the bioCHP module.

\section detailed_model_documentation bioCHP module description

The bioCHP plant model incorporates the following mass/energy streams and unit process operations:

\dot
digraph bioCHP {
    rankdir=LR;
    node [shape=box, style=filled, color=lightgray];

    A1 [label="Biomass"];
    A2 [label="Biomass"];
    B  [label="Storage, preparation\nand feeding"];
    C  [label="Boiler"];
    D  [label="Flue gas cleaning"];
    E  [label="Rankine cycle"];
    F  [label="El. network"];
    G1 [label="HX"];
    H1 [label="District heating"];
    G2 [label="HX"];
    H2 [label="District heating"];
    I  [label="Emissions to air"];
    J  [label="Disposal"];
    K  [label="Disposal"];
    L  [label="Flue gas fan"];
    Air [label="Air", shape=ellipse, color=lightblue];

    A1 -> B;
    A2 -> B;
    B -> C;
    Air -> C;

    C -> E [label="steam"];

    E -> G1 [label="steam"];
    G1 -> H1;

    E -> G2 [label="steam"];
    G2 -> H2;

    C -> D [label="raw flue gas"];
    D -> L;
    L -> I [label="clean flue gas"];

    D -> K [label="solid residue"];
    C -> J [label="bottom ash"];
}
\enddot

The model includes the following main processes:

1. Combustion of the biomass with recovery of the combustion heat for production of superheated steam,
3. Direct utilization of the steam for production of electricity in a steam turbine,
4. Production of heat through extractions from the steam turbine, and
5. Cleaning of the raw flue gas after combustion to remove particulate matter, acid gases and volatile organic components.

The capacity of the bioCHP plant is defined in terms of the total power output from the biomass boiler, calculated from the electric power and heat demands.
The model considers two sold residues, i.e., bottom ash from the boiler and cake residue from flue gas cleaning.

Important features of the model include:
- The feedstock is defined as a mixture of several types of *Biomass* resources.
- Multiple heat demands for **district heating**, specified by thermal power and return/supply temperatures.

\subsection implementation Implementation

The nonlinear C++ model enables sampling for [EnergyModelsX](https://github.com/EnergyModelsX) through the function `bioCHP_wrapper.cpp` which provides a function that calculates both the **costs** (Investment, total and variable operating expenses) and the required **mass flow rate of each feedstock** as outputs, based on the specified *electric power production*, *heat demands* (thermal power capacity and return/supply temperatures) and **moisture content of each biomass feedstock** as inputs. This sampling routine allows a tight integration of the model within the [EnergyModelsX](https://github.com/EnergyModelsX) framework.

Important features and functionalities of the module include:

- **Flows** are implemented as the so-called `flow` structure, defined in `src/Libraries/Flows_library/flow_definitions.h`, which contain identifying strings, atomic and molecular composition, material and energy flow rates, and physical and thermodynamic properties. Flows are created by importing their data from a database located in the `src/Database/flows.toml` folder.  
- **Atomic and molecular species** are also implemented as the so-called `species` structure, defined in `src/Libraries/Flows_library/flow_definitions.h`, which contains identifying strings and physical or thermodynamic properties. Atomic and molecular species are created by importing their data from `src/Database/atoms.toml` and `src/Database/molecules.toml`, respectively. 
- **Thermodynamic properties** are calculated using the library located in `src/Libraries/Thermodynamic_library/`. Molecular gas species (O2, N2, Ar, CO, CO2, H2, CH4, SO2, SO3, HCl, Cl2, NH3) using the NIST correlaltions [8] in `src/Libraries/Thermodynamic_library/species_thermodynamics.h`. Solid biomass properties are constant and user defined, except for heating values which are evaluated by correlations [9]. Liquid water and steam through specific correlations in `src/Libraries/Thermodynamic_library/Library_Water_Correlations.h` using the IAPWS Formulation 1995 [14]. 
-	**Physical systems** such as equipment, systems or plants are defined as so-called `objects`. Objects are implemented as a structure, defined in `src/Parameters.h`, containing identifying strings and a set (vector) of specifying `parameters` for the object. Each `parameter` is also implemented as a structure, defined in `src/Parameters.h`, containing identifying strings, two vectors with numerical or string values of the parameter, and a integer pointing to a position inside the vectors. Each object can contain also other components as objects.
- **Consumables, residues, effluents and utilities** as also implemented as objects.        
- **Processes** are implemented through functions in `src/Libraries/Processes_library/`. Inputs to each function can be flows and an object (containing input parameters) representing the physical system where the process takes place. Output from the process function can be calculated output flows and calculated output parameters within the representing object. 
-	The **cost module** `src/Cost.h` evaluates CAPEX of a physical system and variable OPEX of a consumable or utility using their representing object as input. 
- Values for all physical properties are defined in SI units. Default units for temperature and pressure are defined in degrees Celsius and bar. 

All **input data** used by the bioCHP plant module is implenented using **TOML format** [7] within the `src/Database` directory. Database is described below, in the section `Module implementation and database`. 

The CHP plant module is implemented through the function (in `src/bioCHP.h`): 

`bioCHP_plant(std::vector<std::string> fuel_def, std::vector<double> Yj,
                    std::vector<double> YH2Oj, double W_el,
                    std::vector<double> Qk, std::vector<double> Tk_in,
                    std::vector<double> Tk_out)`

with the inputs defined as follow:

- none (`fuel_def` in the model as `vector[string]`): Biomass composition in the input soild fuel to the CHP plant.
  The input solid fuel to the CHP plant is a mixture of individual biomass feedstock.
  Each biomass feedstock is defined by one string in the `fuel_def` vector, e.g., `spruce_bark`, linked
  to a biomass database as described in section `Module implementation and database` below.
- \f$ Y_j^F \f$ (`Yj_vec[j]` in the model as `vector[double]`): The mass fraction of biomass resource \f$ j \f$ in the feed.
  The total mass fraction must sum to 1.
- \f$ Y_{H_2O,j}^F \f$ (`YH2Oj_vec[j]` in the model as `vector[double]`): The moisture content of biomass resource \f$ j \f$ as a mass fraction.
  It corresponds to the water content in the total wet mass.
- \f$ Th_{in,k} \f$ (`Tkin_vec[k]` in the model as `vector[double]`): The return temperature level of heat demand \f$ k \f$ in °C.
  As the CHP plant can provide heat at several temperature levels, it is possible to specify them directly.
- \f$ Th_{out,k} \f$ (`Tkout_vec[k]` in the model as `vector[double]`): The supply temperature level of heat demand \f$ k \f$ in °C.
  (Provide the pressure by subtracting the ambient pressure. The combination of temperature and pressure determines whether the heat is supplied as steam or as hot water.)
- \f$ \dot{Q}_{k} \f$ (`Qk_vec[k]` in the model as `vector[double]`): The heat demands at the individual temperature levels \f$ k \f$ in MW.
- \f$ \dot{W}_{el} \f$ (`W_el` in the model as `double`): The electric power output of the CHP plant in MW.

When running the CHP module separately, these inputs are specified by the user through the toml files. Examples in `Tests/input_files`. 

\note Vector positions:
Both the input biomass and heat demands are specified as vectors.
This implies that the ordering of the biomass types, their mass fractions, and the moisture content requires correct indexing.
Similarly, the thermal power and return/supply temperatures of the district heating demands must follow the correct indexing.

The standard outputs from the `bioCHP_plant` functions are:

- \f$ \dot{M}_F \f$ (`M_fuel` in the model) is the input mass flow rate of biomass to the bioCHP plant in kg/s.
- \f$ \dot{H}_F \f$ (`H_fuel` in the model) is the input energy flow rate of biomass to the bioCHP plant in MW.
- \f$ C_{inv} \f$ (`C_inv` in the model) represents the capital expenditures in M\$.
- \f$ C_{op,d} \f$ (`C_op_d` in the model) represents the annual direct variable operating expenses in M\$.  
  It is assumed that the plant operates at XXX h/year at full capacity.
- \f$ C_{op,f} \f$ (`C_op_f` in the model) represents the annual fixed operating expenses in M\$.

\note Format of the outputs:
All outputs are required as `double` types in the model.

The integration of the CHP module with EMX is through the wrapper `C` function, (in `src/bioCHP_wrapper.h`): 

`bioCHP_plant_c(const char** fuel_def, int fuel_count, const double* Yj,
                    int Yj_len, const double* YH2Oj, int YH2Oj_len, double W_el,
                    const double* Qk, int Qk_len, const double* Tk_in,
                    int Tk_in_len, const double* Tk_out, int Tk_out_len,
                    double* Mj, int Mj_len, double* Q_prod, double* W_el_prod,
                    double* C_inv, double* C_op, double* C_op_var),`

with variables defined as follow:

- `fuel_def` (input) as biomass composition in the input soild fuel to the CHP plant 
- `fuel_count` (input) denoting the number of biomass feedstock in `fuel_def`
- `Yj`(input) as the mass fraction of biomass resource \f$ j \f$ in the feed.
- `Yj_len` (input) denoting the size of `Yj`   
- `YH2Oj`(input) as the moisture content of biomass resource \f$ j \f$ as a mass fraction.
- `YH2Oj_len` (input) denoting the size of `YH2Oj`
- `Tk_in` (input) as the return temperature level of heat demand \f$ k \f$ in °C.
- `Tk_in_len` (input) denoting the size of `Tk_in`
- `Tk_out` (input) as the supply temperature level of heat demand \f$ k \f$ in °C.
- `Tk_out_len` (input) denoting the size of `Tk_out`
- `Qk` (input) as the heat demand at the individual temperature levels \f$ k \f$ in MW.
- `Qk_len` (input) denoting the size of `Qk`
- `W_el` (input) as the electric power output of the CHP plant in MW.   
- `Mj` (output) as the input mass flow rate of each biomass feedstock to the bioCHP plant in kg/s.
- `Mj_len` (output) denoting the size of `Mj`
- `Q_prod` (output) as the total heat production from the bioCHP plant in MW.
- `W_el_prod` (output) as the total electric power production from the bioCHP plant in MW.
- `C_inv` (output) as represents the capital expenditures in M\$.
- `C_op_d` (output) as the annual direct variable operating expenses in M\$.  
- `C_op_f` (output) as the annual fixed operating expenses in M\$.

\section back-bio_CHP-par-math Mathematical description of the bioCHP model

\subsection back-bio_CHP-math-fuel Solid fuel characterization

The different biomass types furthermore have a set of internal parameters.
These parameters describe the characteristics of the biomass that are fundamental properties of the respective biomass.
Adding a new type of biomass requires adding the following parameters:

- \f$ Y_{i,j}^F \f$ (`Yi.fuel[j]` in the model): The atomic composition of biomass \f$ j \f$ in kg/kg dry basis, i.e., without any water content.
- \f$ Y_{ash,j}^F \f$ (`Yash.fuel[j]` in the model): The ash content in biomass \f$ j \f$ (kg/kg dry basis).
- \f$ LHV_j^F \f$ (`LHV.fuel[j]` in the model): The lower heating value of biomass \f$ j \f$ in MJ/kg.

\subsection back-bio_CHP-math-boiler Solid fuel boiler model

The solid fuel boiler model is implemented in the function
`solid_fuel_boiler(std::vector<flow> &fuel, std::vector<flow> &comb_air,
                       flow &flue_gas, flow &bottom_ash, flow &fly_ash,
                       object &comb)` located in `src/Libraries/Processes_library/combustion.h`. 

It assumes steady-state mass and energy conservation applied to the entire boiler with the following boundaries:
- Inlets: biomass feed, combustion air
- Outlets: bottom ash, flue gas, steam produced, dissipation heat losses

Calculations of the model include:  
1. the mass and energy flows of combustion air, flue gas, bottom ash and fly ash, and
2. the thermal power of the output steam

as a function of: 

 the input biomass mixture and defined boiler parameters. 
- the biomass mixture defined by the mass fraction of each type of feedstock in the mixture \f$ Y_j^F \f$,
- \f$ \lambda_{air}^{comb} \f$ (`lambda_comb` in the model): The excess air in the combustion process (-).
- \f$ T_{stm}^{boiler} \f$ (`T_stm` in the model): The **Boiler** steam temperature in °C.
- \f$ P_{stm}^{boiler} \f$ (`P_stm` in the model): The **Boiler** steam pressure in bar gauge.

Mass and energy flows of combustion air, bottom ash, fly ash and flue gas are calculated as linear superposition of the mass and energy balance for the combustion process of each individual biomass feedstock in the fuel mix. 

Combustion of each biomass feedstock is modelled assuming steady-state through the global reaction [12]

\f$ CH_xO_y + \nu_{O2} (O_2 + 3.76N_2) \rightarrow \nu_{CO2} CO_2 + \nu_{H2O} H_2O \f$

where CH_xO_y represents the biomass feedstock atomic composition per unit mol of carbon.
Here \f$ x \f$ and \f$ y \f$ denoting, respectively, the hydrogen-to-carbon and oxygen-to-carbon atomic molar ratio calculated in terms of the atomic composition from:
For each biomass feedstock, stoichiometric combustion parameters are calculated through the function 
`calculate_fuel_combustion_properties(flow fuel, object &prop)`, including:

- \f$ x \f$ (`prop.n_H` in the model) from \f$ x = (Y_H / MW_H) / (Y_C / MW_C) \f$
- \f$ y \f$ (`prop.n_O` in the model) from \f$ y = (Y_O / MW_O) / (Y_C / MW_C) \f$
- \f$ \nu_{O2} \f$ (`prop.nu_O2` in the model) from \f$ \nu_{O2} = 1 - y/2 + x/4 \f$
- \f$ \nu_{CO2} \f$ (`prop.nu_CO2` in the model) from \f$ \nu_{CO2} = 1 \f$
- \f$ \nu_{H2O} \f$ (`prop.nu_O2` in the model) from \f$ \nu_{H2O} = y/2 \f$
- \f$ \dot{V}_{stoich} \f$ (`prop.V_stoich` in the model) is the stoichiometric combustion air flow rate (Nm3/s) calculated from 
   
     \f$ \dot{V}_{stoich} = \dot{M}_F Y_j^F (1 - Y_{H2O,j}^F) (1-Y_{ash,j}^F) (\nu_{O2} / 0.21 ) (0.02241 / W_{CHxOy})  \f$

- \f$ vn_{CO2} \f$ (`prop.vn_CO2_m` in the model) is the stoichiometric normal flow rate of CO2 (Nm3) per unit mass (kg) produced from combustion calculated from 

     \f$ vn_{CO2} = \dot{M}_F Y_j^F (1 - Y_{H2O,j}^F) (1-Y_{ash,j}^F) \nu_{CO2} (0.02241 / W_{CHxOy})  \f$

- \f$ vn_{H2O} \f$ (`prop.vn_H2O_m` in the model) is the stoichiometric normal flow rate of H2O (Nm3) per unit mass (kg)  produced from combustion  calculated from 

     \f$ vn_{H2O} = \dot{M}_F Y_j^F (1 - Y_{H2O,j}^F) (1-Y_{ash,j}^F) \nu_{H2O} (0.02241 / W_{CHxOy})  \f$

Here, \f$ MW_C \f$, \f$ MW_H \f$, and \f$ MW_O \f$ are atomic weights for C, H and O. 

Calculated flows for the solid fuel boiler model includes:

- Mass and energy flow rate of the biomass mixture and of each feedstock \f$ j \f$:
 \f[
  \begin{aligned}
    \dot{M}_F & = \dot{H}_F/h_{fuel} \\
    \dot{M}_j^F & = \dot{M}_F \sum_{j=1}^{N_j} Y_j^F \\
    \dot{H}_F & = \dot{M}_F h_{fuel} \\
  \end{aligned}
  \f]
with specific enthalpy of the fuel is calculated through the lower heating values of the individual biomass resources (indexed through \f$ j \f$), excluding the moisture in the resources (indexed through \f$ H_2O \f$):
\f[
h_{fuel} = \sum_j Y_j^F\Big[(1-Y_{H_2O,j})\, LHV_j - Y_{H_2O,j}\, h_{v,H_2O}\Big]
\f]

- Volumetric flow rate (Nm3/s)and mass flow rate (kg/s) of combustion air required by the boiler:
 \f[
  \begin{aligned}
    \dot{V}_{ca}^{boiler} & = \sum_j \Big[ \dot{V}_{stoich,j}\, (1+\lambda_{air}^{comb}) \Big] \\
    \dot{M}_{ca}^{boiler} & = \dot{V}_{ca}^{boiler} (W_{air} / 0.02241) \\
  \end{aligned}
  \f]
Here, \f$ W_{air} \f$ is the molecular weight of air (29 g/mol).

- Mass flow rate and energy flow rate of bottom ash (\f$ ba \f$) from the boiler:
 \f[
  \begin{aligned}
    \dot{M}_{ba}^{boiler} & = \dot{M}_F \,(1+Y_{C,ba})\, f_{ba}\, \sum_j \Big[ Y_{ash,j}^F\, \dot{Y}_j^F\,(1-Y_{H2O,j}) \Big] \\
    \dot{H}_{ba}^{boiler} & = \dot{M}_F\, h_{ba}^{boiler} \\
  \end{aligned}
  \f]
with the specific enthalpy of the bottom ash is given by
\f[
h_{ba}^{boiler}  = \Big[c_{ba}\,(T_{ba}^{boiler}-T_0) + Y_{C,ba}\, h_C\Big]\, f_{ba}\,\sum_j \Big[ \dot{Y}_j^F\,(1-Y_{H2O,j})\, Y_{ash,j}^F \Big]
\f]

- Mass and energy flow rates of flue gas from the CHP plant:
 \f[
  \begin{aligned}
    \dot{M}_{g}^{FGC} & = \dot{M}_F\, \sum_j \dot{Y}_j^F\Big[ Y_{H2O,j} + (1-Y_{H2O,j})\Big( 1+\lambda_{air}^{comb}\, Y_{C,j}\Big(\frac{W_{air}}{W_C}\Big)(1+n_H/4-n_O/2)\Big) \Big] \\
    \dot{H}_{g}^{FGC} & = \dot{M}_F\, h_g^{FGC} \\
  \end{aligned}
  \f]
with the specific enthalpy of the flue gas at the boiler outlet is calculated through the difference between its temperature and the reference temperature \f$ T_0 \f$, the heat capacities \f$ c_{p,g,j} \f$ and
\f[
h_g^{FGC}  = (T_g^{FGC}-T_0) \sum_j \Big\{ Y_j^F\, c_{p,g,j} \Big( Y_{H2O,j} + (1-Y_{H2O,j})\Big[ 1+\lambda_{air}^{comb}\, Y_{C,j}\Big(\frac{W_{air}}{W_C}\Big)(1+n_H/4-n_O/2)\Big]\Big) \Big\}
\f]

\note Specific enthalpies:
All specific enthalpies described above are relative to the mass flow of the biomass feedstock \f$ \dot{M}_F \f$.

The total thermal power produced from the boiler is calculated by applying energy conservation as the difference in mass enthalpy between the biomass fuel and the output from the system (i.e. the flue gas, the fly ash, and the bottom ash).  

 \f[
  \begin{aligned}
    \dot{Q}_{stm} & = \frac{\dot{H}_{fuel} - \dot{H}_g^{FGC} - \dot{H}_{ba}^{boiler} - \dot{H}_{s}^{FGC}}{(1-q_{loss})} \\
  \end{aligned}
  \f]

where the \f$ q_{loss} \f$ corresponds is the fraction of dissipation heat losses relative to thermal power production, assumed to be constant and equal to 0.1.

\subsection back-bio_CHP-math-rankine_cycle Rankine cycle model

The rankine cycle model is implemented in the function `rankine_cycle(object &par)` (located in `src/Libraries/Processes_library/Rankine_cycle.h`)

with the following input parameters:

- \f$ \dot{Q}_{stm}^{boiler} \f$ (`par.Q_stm` in the model): The **Boiler** steam thermal energy production in MW.
- \f$ T_{stm}^{boiler} \f$ (`par.T_stm` in the model): The **Boiler** steam temperature in °C.
- \f$ P_{stm}^{boiler} \f$ (`par.P_stm` in the model): The **Boiler** steam pressure in bar gauge.
- \f$ \dot{Q}_{k} \f$ (`par.Qk[k]` in the model as `vector[double]`): The heat demands at the individual temperature levels \f$ k \f$ in MW.
- \f$ Th_{in,k} \f$ (`par.Tk_in[k]` in the model as `vector[double]`): The return temperature level of heat demand \f$ k \f$ in °C.
- \f$ Th_{out,k} \f$ (`par.Tk_out[k]` in the model as `vector[double]`): The supply temperature level of heat demand \f$ k \f$ in °C.
- \f$ \eta_S \f$ (`par.mu_isent` in the model): The isentropic efficiency of the **steam turbine**, assumed to be constant.
- \f$ T_{cond} \f$ (`par.T_cond` in the model): The **Condenser** temperature in °C.
- \f$ P_{cond} \f$ (`par.P_cond` in the model): The **Condenser** pressure in bar gauge.

The rankine cycle performs the following calculations:

1. **Steam flow** produced from the boiler, from:
 \f[
  \begin{aligned}
    \dot{M}_{stm}^{boiler} & = \frac{\dot{Q}_{stm}^{boiler}}{(h_{stm}^{boiler}-h_{bfw}^{boiler})} \\
  \end{aligned}
  \f]

2. **District heating model**, implemented in the function `district_heating(object &par)` (located in `src/Libraries/Processes_library/Rankine_cycle.h`)
It calculates **Steam extractions** (bleeds) required from steam turbine to meet the specified heat demands for distric heating
Calculated bleeds, exported to the rankine_cycle `par` object, are defined as: 
    - \f$ Pb_{n} \f$ (`par.Pb[nb]` in the model as `vector[double]`): pressure for each steam extraction in descendent order.
    - \f$ \dot{M}b_{n} \f$ (`par.Mb[nb]` in the model as `vector[double]`): steam mass flow rates related to each steam extraction pressure.
Bleeds are calculated for each heat demand as follows:
  \f$ Pb_{k} \f$ (`P_bleed[k]` in the model as `vector[double]`) is calculated as the saturation pressure of water at \f$ Th_{out,k} + 25°C \f$
  \f$ \dot{M}b_{k} \f$ (`P_bleed[k]` in the model as `vector[double]`)
\f[ 
Mb_{k} = \frac{\dot{Q}_{k}}{h_{vap}^{sat}(Pb_{k}) - h_{cond}^{sat}(Pb_{k})} 
\f]
with \f$ h_{vap}^{sat}(Pb_{k}) \f$ and \f$ h_{cond}^{sat}(Pb_{k}) \f$ are the vapor and condensate specific enthalpies of saturated water evaluated at \f$ Pb_{k} \f$. 
The pair \f$ [Pb_{n}, \dot{M}_{n} ]\f$ is obtained by sorting the pressure in descendent order and merging bleeds with pressure difference lower than 5 bar. 
When two bleeds merge, the resulting one keeps the higher pressure and the sum the steam flow rates.     

3. **steam turbine model**, implemented in the function `steam_turbine_model(object &par)`(located in `src/Libraries/Processes_library/Rankine_cycle.h`)
It calculates the steam turbine with steam extractions. 
The overall steam turbine process is divided in stages: inlet to first extraction, between extractions, and last extraction to outlet. 
Each stage is calculated assuming steady state, adiabatic expansion using a constant isentropic efficiency [13]   
The output is the calculated total electric power production, denoted by \f$ \dot{W}_{el} \f$ (`par.W_el` in the model) in MW, from
\f[
\dot{W}_{el} = \sum_{n=0}^{N_q} \left(\dot{H}_F \frac{(q_{stm}^{boiler}/h_{fuel})}{(h_{stm}^{boiler}-h_{bfw}^{boiler})} - \sum_{k=0}^n \frac{\dot{Q}_{k}}{(h_{stm}^{k}-h_{o}^{k})} \right) (h_{in}^{ST_n}-h_{out,s}^{ST_n})\eta_S^{ST_n}
\f]

4. **Steam condenser**, implemented in the function `steam_condenser(flow &steam, flow &cond, object &par)` (located in `src/Libraries/Processes_library/Rankine_cycle.h`)
The model assumes steady-state and isobaric process [13].
Inputs: 
    - `steam`, **flow** denoting the steam outlet from the steam turbine model
    - \f$ T_{cond} \f$ (`par.T_cond` in the model): The condenser temperature in °C.
    - \f$ P_{cond} \f$ (`par.P_cond` in the model): The condenser pressure in bar gauge.

Outputs:
    - `cond`, **flow** denoting the condenstae outlet
    -  Thermal power output \f$ \dot{Q}_{cond} \f$ (`par.Q_cond` **parameter** in the model) 
 \f[
  \begin{aligned}
    \dot{Q}_{cond} & = \frac{\dot{M}_{stm}^{turbine}}{h_{stm}^{turbine}-h_{cond}} \\
  \end{aligned}
  \f]
where \f$ \dot{M}_{stm}^{turbine} \f$ and \f$ h_{stm}^{turbine} \f$ is the mass flow and specific enthalpy of steam from the turbine, and \f$ h_{cond} \f$ is the specific ethalpy of condensated water evaluated at \f$ P_{cond}\f$ and \f$ T_{cond} \f$.

\subsection back-bio_CHP-math-dry-scrubber Dry scrubber model

Implemented in the function `dry_scrubber_model(flow &in, flow &out, object &par)` (located in`src/Libraries/Processes_library/flue_gas_cleaning.h`)

Inputs: 
- `in`, flow denoting the input flue gas
- \f$ T_{cond} \f$ (`par.T_cond` in the model): The condenser temperature in °C.
- \f$ P_{cond} \f$ (`par.P_cond` in the model): The condenser pressure in bar gauge.

Outputs:
- `out`, **flow** denoting the output flue gas
- `par.lime`, **object** representing lime as **consumable** 
- `par.cake`, **object** representing cake as **solid residue** 
-  Annual mass of lime \f$ \dot{M}_{lime} \f$ (`par.lime.Q_annual` in the model), calculated from
 \f[
  \dot{M}_{lime} = \dot{M}_F\ \cdot m_{lime,b}\ \cdot t_{op} \cdot 3.6 
  \f]
with \f$ m_{lime,b} = 14.4 kg/t \f$ a constant specific lime consumption per unit mass of solid fuel entering the boiler [5] and \f$ t_{op} = 8000 h \f$ is the annual operational time.

-  Annual mass of cake \f$ \dot{M}_{s} \f$ (`par.cake.Q_annual` in the model), calculated from   
 \f[
  \dot{M}_{s} = \dot{M}_{lime}
  \f]


\subsection back-bio_CHP-math-CAPEX CAPEX models

The installed cost of equipment \f$ k \f$ is calculated from [1]

\f[
C_{eq,k} = C_{P,k}^B \left(\frac{S_k}{S_k^B}\right)^{n_k} \left(\frac{I}{I_B}\right) f_{inst,k},
\f]

where \f$ C_{P,k}^B \f$ is the purchase cost for a base-case equipment size \f$ S_k^B \f$ in the reference year, \f$ S_k \f$ is the actual equipment size, \f$ n_k \f$ is the equipment scale factor, and \f$ f_{inst,k} \f$ is the installation factor. The ratio \f$ I/I_B \f$ is the price index ratio between the actual year and the reference year (e.g. using the Chemical Engineering Plant Cost Index).

The total equipment cost is then given by

\f[
C_{eq} = \sum_k C_{eq,k}
\f]

The total capital expenditures (CAPEX) is evaluated in terms of the total permanent investment \f$ C_{inv} \f$ adapting the factoral Method by Peters et al. [2] from :

\f[
C_{inv} = \sum_k C_{eq,k}\,(1+f_{pip}+f_{el}+f_{I\&C})\Big[(1+f_{site}+f_{building})+f_{com}\Big](1+f_{cont,k}+f_{eng,k})(1+f_{dev})
\f]

where \f$ C_{eq,k} \f$ denotes the installed cost of equipment and the \f$ f_i \f$ are cost parameters defined [3] as follows:
- \f$ f_{pip} \f$ for interconnecting piping between equipment,
- \f$ f_{el} \f$ for the plant electric system,
- \f$ f_{I\&C} \f$ for the instrumentation and control system,
- \f$ f_{site} \f$ for land and site preparation,
- \f$ f_{building} \f$ for construction of buildings,
- \f$ f_{com} \f$ for commissioning,
- \f$ f_{dev} \f$ for project development and licenses,
- \f$ f_{eng,k} \f$ for engineering, and
- \f$ f_{cont,k} \f$ for contingency.

\subsection back-bio_CHP-math-OPEX OPEX models

The OPEX is defined as the total annual operating costs calculated from

\f[
C_{op} = C_{op,d} + C_{op,f},
\f]

where:

- \f$ C_{op,d} \f$ denotes the variable operating cost calculated from \f$ C_{op,d} = \sum_j Q_j p_j \f$, with the subscript \f$ j \f$ denoting feedstock, consumables and residues, \f$ Q_j \f$ is the annual production quantity proportional to the annual operating time \f$ t_{op} \f$, and \f$ p_j \f$ is the unit price.  including the

- \f$ C_{op,f} \f$ denotes the fixed operating costs required for keeping the bioCHP plant in operation, including:
    - Maintenance cost:
    \f[
    C_{maint} = 0.05\, C_{eq}
    \f]

    - Insurance:
    \f[
    C_{ins} = 0.01\, C_{inv}
    \f]

    - Administration and site services:
    \f[
    C_{adm} = 0.03\, C_{inv}
    \f]

    - Labor cost:
    \f[
    C_{labor} = \sum_k N_k^{labor}\, c_{b,k}^{labor}\,\Big[1+f_{oh,k}^{labor}\Big]
    \f]
  Here, the subscript \f$ k \f$ denotes the personnel categories and the parameters are defined as follows:

    - Plant manager:
    \f[
    \begin{aligned}
    c_{b,k}^{labor} & = 162~k\$/year \\
    N_k^{labor} & = 1 \\
    f_{oh,k}^{labor} & = 0.0 \\
    \end{aligned}
    \f]

    - O\&M manager:
    \f[
    \begin{aligned}
    c_{b,k}^{labor} & = 96~k\$/year \\
    N_k^{labor} & =
    \begin{cases}
      1 ,& \text{if} ~ \dot{M}_F < 10~\text{t/h} \\
      2 ,& \text{if} ~ \dot{M}_F > 10~\text{t/h} \\
    \end{cases} \\
    f_{oh,k}^{labor} & = 1.2 \\
    \end{aligned}
    \f]

    - O\&M engineer:
    \f[
    \begin{aligned}
      c_{b,k}^{labor} & = 88~k\$/year \\
      N_k^{labor} & =
      \begin{cases}
        1 ,& \text{if} ~ \dot{M}_F < 10~\text{t/h} \\
        2 ,& \text{if} ~ \dot{M}_F > 10~\text{t/h} \\
      \end{cases} \\
      f_{oh,k}^{labor} & = 1.2 \\
    \end{aligned}
    \f]

    - Shift operator:
    \f[
    \begin{aligned}
      c_{b,k}^{labor} & = 37~k\$/year \\
      N_k^{labor} & =
      \begin{cases}
        3 ,& \text{if} ~ \dot{M}_F < 10~\text{t/h} \\
        6 ,& \text{if} ~ \dot{M}_F > 10~\text{t/h} \\
      \end{cases} \\
      f_{oh,k}^{labor} & = 1.3 \\
    \end{aligned}
    \f]
\note Number of employees:
The number of employees depends on the size of the plant. The chosen distinction is based on the mass flow of biomass into the plant \f$ \dot{M}_F \f$, with a change in staffing when the flow exceeds 10 t/h.

\section module_implementation_and_database Module implementation and database


- Input parameters for the bioCHP plant, in `src/Database/bioCHP_inputs.toml`:
| Parameter                | symbol | value | unit | ref. |
|-------------------------:|-------:|------:|-----:|-----:|
| Boiler steam temperature | T_stm  | 450   | deg. C  | assumed  |
| Boiler steam pressure    | P_stm  | 100.0 | bar-g | assumed |
| Electricity price       | price_electricity | 0.1 | euro/kwh | assumed |
| equipment cost factor, piping | f_piping | 0.065 | - | [3] |
| equipment cost factor, electrical system | f_el | 0.05 | - | [3] |
| equipment cost factor, instrumentation&control | f_I&C | 0.05 | - | [3] |
| capital cost factor, land | f_land | 0.12 | - | [3] |
| capital cost factor, site | f_site | 0.05 | - | [3] |
| capital cost factor, building | f_build | 0.2 | - | [3] |
| capital cost factor, engineering | f_eng | 0.1 | - | [3] |
| capital cost factor, development | f_dev | 0.02 | - | [3] |
| capital cost factor, commissioning | f_com | 0.1 | - | [3] |
| capital cost factor, contigency | f_cont | 0.15 | - | [3] |
| capital cost factor, insurance | f_ins | 0.01 | - | [3] |
| capital cost factor, administration | f_adm | 0.01 | - | [3] |

- Input parameters for the biomass boiler, in `src/Database/bioCHP_inputs.toml`:
| Parameter                | symbol | value | unit | ref. |
|-------------------------:|-------:|------:|-----:|-----:|
| Excess combustion air  | lambda  | 1.2   | -  | assumed  |
| Combustion chamber pressure  | P_bar  | 1.01325 | bar-g | assumed |
| Combustion air temperature  | T_ox | 25.0 | deg. C | assumed |
| Flue gas temperature, boiler outlet | T_g | 140 | deg. C | assumed |
| Heat loss to powe output ratio | q_loss | 0.05 | - | assumed |
| Carbon content in bottom ash | yC_ba | 0.03 | - | assumed |
| Temperature bottom ash | T_ba | 1000.0 | - | assumed |
| Temperature fly ash | T_fa | 1000.0 | - | assumed |
| Fraction of ash as bottom ash | f_ba | 0.1 | - | assumed |

- Input parameters for the flue gas cleaning, in `src/Database/bioCHP_inputs.toml`:
| Parameter                | symbol | value | unit | ref. |
|-------------------------:|-------:|------:|-----:|-----:|
| SO2 emission limit  | SO2  | 50   | mg/Nm3  | assumed  |
| HCl emission limit  | HCl  | 10   | mg/Nm3  | assumed  |
| PM emission limit  | PM  | 50   | mg/Nm3  | assumed  |
| Operatonal temperature | T_op | 140 | deg. C | assumed |
| Operatonal temperature | P_op | 1.01325 | bar-a | assumed |

- Input parameters for the Rankine cycle, in `src/Database/bioCHP_inputs.toml`:
| Parameter                | symbol | value | unit | ref. |
|-------------------------:|-------:|------:|-----:|-----:|
| Outlet pressure steam turbines | Po  | 0.032   | bar-a  | assumed  |
| Isentropic efficiency steam turbines  | mu_isent  | 0.65   | -  | assumed  |
| Condenser temperature | T_cond | 25.0 | deg. C | assumed |
| Condenser pressure | P_cond | 1.01325 | bar-a | assumed |

- Biomass feedstock data, atomic and proximate composition, in `src/Database/flows.toml`
| biomass feedstock                | ref. |
|------------------------------|-----:|
| spruce chips supply          | [9]  |
| spruce stem supply           | [9]  |
| spruce TandB supply          | [9]  |
| spruce bark supply           | [9]  |
| birch stem supply            | [9]  |

- Molecules data, including composition (chemical formula) and molecular weight (kg/mol) from NIST database [8], in `src/Database/molecules.toml`   

- Atoms data, including atomic weight (kg/mol) and valences, in `src/Database/atoms.toml`

- Specific equipment cost data, in `src/Database/cost.toml`:
| Equipment                         | `S_k^B` | `C_{P,k}^B` (M\$) | `f_{inst,k}` | `n_k` | Base year | ref. |
|-----------------------------------|--------:|------------------:|-------------:|------:|----------:|-----:|
| Biomass storage and preparation   | 25 t/h  | 5.4               | 2.1          | 0.5   | 2007      | [4]  |
| Biomass boiler                    | 25 t/h  | 7.9               | 2.1          | 0.7   | 2007      | [4]  |
| Flue gas cleaning                 | 67 t/h  | 0.18              | 2.7          | 0.7   | 2007      | [5]  |
| Steam turbines and condenser      | 1500 MW | 40.5              | 1.3          | 0.7   | 2006      | [6]  |
| Heat Exchanger (heat export)      | 100 m²  | 0.086             | 2.8          | 0.71  | 2008      | [1]  |

- Unit prices used to calculate direct operating costs, in `src/Database/Cost.toml`:
| Variable cost                | Unit price `p_j` | ref. |
|------------------------------|-----------------:|-----:|
| spruce chips supply          | 50 \$/t          | assumed  |
| spruce stem supply           | 70 \$/t          | assumed  |
| spruce TandB supply          | 30 \$/t          | assumed  |
| spruce bark supply           | 30 \$/t          | assumed  |
| birch stem supply            | 70 \$/t          | assumed  |
| Lime supply                  | 0.31 \$/kg       | [11]  |
| Scrubber cake disposal       | 40 \$/kg         | [11]  |
| Bottom ash disposal          | 20 \$/kg         | [11]  |


\section references References

[1] Woods, D. R., Rules of Thumb. Rules of Thumb in Engineering Practice. Wiley-VCH: 2008. https://doi.org/10.1002/9783527611119

[2] Peters, M. S.; Timmerhaus, K. D.; West, R. E.; Timmerhaus, K.; West, R., Plant design and economics for chemical engineers, Vol. 4.. 5th ed., McGraw-Hill New York: 2003.

[3] Ereev, S..; Patel, M. Recommended methodology and tool for cost estimates at micro level for new technologies. Utrecht, October 2011.
https://dspace.library.uu.nl/bitstream/handle/1874/279466/Issue02-2012_150.pdf?sequence=1&isAllowed=y 

[4] Tomberlin, G. (2014). Wood Pellet-Fired Biomass Boiler Project at the Ketchikan Federal Building. National Renewable Energy Laboratory (NREL). https://doi.org/10.2172/1171779

[5] J. Stubenvoll, S. Böhmer and I. Szednyj, "State of the Art for Waste Incineration Plants," Federal Ministry of Agriculture and Forestry, Environment and Water Management, Vienna 2002. ISBN 3-902 338-13-X 

[6] M. C. Woods, P. J. Capicotto, J. L. Haslbeck, N. J. Kuehn, M. Matuszewski, L. L. Pinkerton, M. D. Rutkowski, R. L.
Schoff, V. Vaysman, Cost and Performance Baseline for Fossil Energy Plants. Volume 1: Bituminous coal and natural
gas to electricity. Report DOE/NETL-2007/1281, 2007. URL: https://www.nrc.gov/docs/ML1217/ML12170A423.pdf.

[7] T. Preston-Werner, P. Gedam, et al., “TOML: Tom’s Obvious, Minimal Language, Version 1.0.0,” Jan. 11, 2021. [Online]. Available: https://toml.io/en/v1.0.0

[8] National Institute of Standards and Technology. (2025). NIST Chemistry WebBook, NIST Standard Reference Database Number 69. U.S. Department of Commerce. https://doi.org/10.18434/T4D303 

[9] Phyllis2, database for (treated) biomass, algae, feedstocks for biogas production and biochar. https://phyllis.nl/

[10] Changdong Sheng, J.L.T. Azevedo, "Estimating the higher heating value of biomass fuels from basic analysis data, Biomass and Bioenergy," Vol. 28, Issue 5, 2005, pp. 499-507. https://doi.org/10.1016/j.biombioe.2004.11.008.

[11] G. Del Alamo, Rajesh S. Kempegowda, Øyvind Skreiberg, and Roger Khalil, Energy & Fuels 2017 31 (6), 6089-6108. https://doi.org/10.1021/acs.energyfuels.7b00273

[12] Koppejan, J., & van Loo, S. (Eds.). (2008). The Handbook of Biomass Combustion and Co-firing (1st ed.). Routledge. https://doi.org/10.4324/9781849773041

[13] Moran, M.J., Shapiro, H.N., Boettner, D.D., & Bailey, M.B. Fundamentals of Engineering Thermodynamics. 8th Edition. Wiley, 2014. ISBN: 978-1118412930

[14] W. Wagner and A. Pruß, "The IAPWS Formulation 1995 for the Thermodynamic Properties of Ordinary Water Substance for General and Scientific Use," J. Phys. Chem. Ref. Data, 31, 387-535 (2002). https://doi.org/10.1063/1.1461829