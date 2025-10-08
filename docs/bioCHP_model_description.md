\page detailed_model_documentation Detailed Model Documentation

\section back-bio_CHP BioCHP plant module

The BioCHP plant module incorporates the following mass/energy streams and unit process operations:

\dot
digraph BioCHP {
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
    E -> F [label="electricity"];

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

1. Supply, storage and handling of the solid biomass,
2. Combustion of the biomass with recovery of the combustion heat for production of superheated steam,
3. Direct utilization of the steam for production of electricity in a steam turbine,
4. Production of heat through extractions from the steam turbine, and
5. Cleaning of the raw flue gas after combustion to remove particulate matter, acid gases and volatile organic components.

The capacity of the bioCHP plant is defined in terms of the total power output from the biomass boiler, calculated from the electric power and heat demands.
The model considers two sold residues, i.e., bottom ash from the boiler and residue from flue gas cleaning containing fly ash and consumed lime.

Important features of the model include:
- The feedstock is defined as a mixture of several types of *Biomass* resources.
- Multiple heat demands are specified by thermal power, temperature, and pressure for **district heating** (each through a heat exchanger) or as **direct steam export**.

The module is implemented as a nonlinear C++ model linked to `EnergyModelsX` through a function that calculates both the **costs** (Investment, total and variable operating expenses) and the required **mass flow rate of each feedstock** as outputs, based on the specified *electric power production*, *heat demands* (thermal power capacity and return/supply temperatures) and **moisture content of each biomass feedstock** as inputs. 

This sampling routine allows a tight integration of the model within the `EnergyModelsX` framework.

Important functionalities of the module include:

- **Flows** are implemented as the so-called `flow` structure, defined in `src/Flows/Flow_definitions.h`, which contain identifying strings, atomic and molecular composition, material and energy flow rates, and physical and thermodynamic properties. 
- **Atomic and molecular species** are also implemented as the so-called `species` structure, defined in `src/Flows/Flow_definitions.h`, which contains identifying strings and physical or thermodynamic properties.       
-	**Physical systems** such as equipment, systems or plants are defined as so-called `objects`. Objects are implemented as a structure, defined in `src/Parameters.h`, containing identifying strings and a set (vector) of specifying `parameters` for the object. Each `parameter` is also implemented as a structure, defined in `src/Parameters.h`, containing identifying strings, two vectors with numerical or string values of the parameter, and a integer pointing to a position inside the vectors. Each object can contain also other components as objects.
- **Consumables and utilities** as also implemented as objects.        
- **Processes** as implemented through functions. Inputs to each function can be flows and an object (with input parameters) representing the physical system where the process takes place. Output from the process function can be calculated output flows and calculated output parameters within the representing object. 
-	The **cost module** `src/Cost.cpp` evaluates CAPEX of a physical system and variable OPEX of a consumable or utility using their representing object as input. 

\section back-bio_CHP-par Parameters

The parameters of the BioCHP plant model can be differentiated into external inputs and internal parameters as defined below.

\subsection back-bio_CHP-par-inp Inputs

The first relevant input category is related to the biomass used in the CHP plant.
The biomass is included in the module through a combination of strings.
It is hence necessary to use the strings specified within the module.
The following input is required for the biomass feed:

- none (`fuel_def` in the model as `vector[string]`): The type of biomass.
  Multiple biomass types can be provided to the module, e.g., `spruce_bark`.  
  The currently implemented feedstock strings are `spruce_stem`, `spruce_bark`, `spruce_T&B`, and `birch_stem`.
- \f$ Y_j^F \f$ (`Y_fuel[j]` in the model as `vector[double]`): The mass fraction of biomass resource \f$ j \f$ in the feed.
  The total mass fraction must sum to 1.
- \f$ Y_{H_2O,j}^F \f$ (`YH2O_fuel[j]` in the model as `vector[double]`): The moisture content of biomass resource \f$ j \f$ as a mass fraction.
  It corresponds to the water content in the total wet mass.

In addition, the input for the desired plant characteristics must be provided.
This input can be specified by the user through a text file, although upper bounds may exist for some variables.
The following input is required for the plant characteristics:

- \f$ T_{h,k} \f$ (`T_h[k]` in the model as `vector[double]`): The temperature level of heat demand \f$ k \f$ in °C.
  As the CHP plant can provide heat at several temperature levels, it is possible to specify them directly.
- \f$ P_{h,k} \f$ (`P_h[k]` in the model as `vector[double]`): The outlet pressure of heat demand \f$ k \f$ in bar gauge.
  (Provide the pressure by subtracting the ambient pressure. The combination of temperature and pressure determines whether the heat is supplied as steam or as hot water.)
- \f$ \dot{Q}_{k} \f$ (`Q_h[k]` in the model as `vector[double]`): The heat demand at the individual temperature and pressure levels \f$ k \f$ in MW.
- \f$ \dot{W}_{el} \f$ (`W_el` in the model as `double`): The electric power output of the CHP plant in MW.

\note Vector positions:
Both the input biomass and heat demands are specified as vectors.
This implies that the ordering of the biomass types, their mass fractions, and the moisture content requires correct indexing.
Similarly, the thermal power, temperature, and pressure conditions of the heat demands must follow the correct indexing.

\section back-bio_CHP-par-out_stand Outputs (standard)

The standard output of the model is given as

- \f$ \dot{M}_F \f$ (`M_fuel` in the model) is the input mass flow rate of biomass to the BioCHP plant in kg/s.
- \f$ \dot{H}_F \f$ (`H_fuel` in the model) is the input energy flow rate of biomass to the BioCHP plant in MW.
- \f$ C_{inv} \f$ (`C_inv` in the model) represents the capital expenditures in M\$.
- \f$ C_{op,d} \f$ (`C_op_d` in the model) represents the annual direct variable operating expenses in M\$.  
  It is assumed that the plant operates at XXX h/year at full capacity.
- \f$ C_{op,f} \f$ (`C_op_f` in the model) represents the annual fixed operating expenses in M\$.

\note Format of the outputs:
All outputs are required as `double` types in the model.

\subsection back-bio_CHP-par-int Internal

The different biomass types furthermore have a set of internal parameters.
These parameters describe the characteristics of the biomass that are fundamental properties of the respective biomass.
Adding a new type of biomass requires adding the following parameters:

- \f$ Y_{i,j}^F \f$ (`Yi.fuel[j]` in the model): The atomic composition of biomass \f$ j \f$ in kg/kg dry basis, i.e., without any water content.
- \f$ Y_{ash,j}^F \f$ (`Yash.fuel[j]` in the model): The ash content in biomass \f$ j \f$ (kg/kg dry basis).
- \f$ LHV_j^F \f$ (`LHV.fuel[j]` in the model): The lower heating value of biomass \f$ j \f$ in MJ/kg.
- \f$ x \f$: The hydrogen-to-carbon atomic molar ratio.
- \f$ y \f$: The oxygen-to-carbon atomic molar ratio.

Furthermore, process characteristics can be specified internally:

- \f$ \lambda_{air}^{comb} \f$ (`lambda_comb` in the model): The excess air in the combustion process (-).
- \f$ T_{stm}^{boiler} \f$ (`T_stm` in the model): The **Boiler** steam temperature in °C.
- \f$ P_{stm}^{boiler} \f$ (`P_stm` in the model): The **Boiler** steam pressure in bar gauge.
- \f$ \eta_S^{ST_n} \f$ (`eta_s[n]` in the model): The isentropic efficiency for steam turbine stage \f$ n \f$.
  (Individual steam turbines of the Rankine cycle can have different isentropic efficiencies.)

\note Format of the parameters:
All internal parameters are required as `double` types in the model.

\section back-bio_CHP-math Mathematical formulation

\subsection back-bio_CHP-math-balances Mass and energy flows (nominal steady state operation)

Considering

1. a biomass mixture defined by the mass fraction of each type of feedstock in the mixture \f$ Y_j^F \f$,
2. the net power production from the Rankine cycle \f$ \dot{W}_{el}^{RK} \f$, and
3. the external heat demands \f$ \dot{Q}_{k} \f$

as inputs, the total energy rate of the input biomass to the CHP plant \f$ \dot{H}_F \f$ with \f$ N_q \f$ steam extractions to cover heat demands is calculated as

\f[
\dot{W}_{el}^{RK} = \sum_{n=0}^{N_q} \left(\dot{H}_F \frac{(q_{stm}^{boiler}/h_{fuel})}{(h_{stm}^{boiler}-h_{bfw}^{boiler})} - \sum_{k=0}^n \frac{\dot{Q}_{k}}{(h_{stm}^{k}-h_{o}^{k})} \right) (h_{in}^{ST_n}-h_{out,s}^{ST_n})\eta_S^{ST_n}
\f]

with

\f[
q_{stm}^{boiler} = \frac{h_{fuel} - h_g^{FGC} - h_{ba}^{boiler} - h_{s}^{FGC}}{(1-q_{loss})},
\f]

that is, the difference in mass enthalpy between the biomass fuel and the output from the system (i.e. the flue gas, the fly ash, and the bottom ash).  
The value \f$ q_{loss} \f$ corresponds in this situation to the fraction of heat loss.

The specific enthalpy of the fuel is calculated through the lower heating values of the individual biomass resources (indexed through \f$ j \f$), excluding the moisture in the resources (indexed through \f$ H_2O \f$):

\f[
h_{fuel} = \sum_j Y_j^F\Big[(1-Y_{H_2O,j})\, LHV_j - Y_{H_2O,j}\, h_{v,H_2O}\Big]
\f]

The specific enthalpy of the combustion gas is calculated through the difference between its temperature and the reference temperature \f$ T_0 \f$, the heat capacities \f$ c_{p,g,j} \f$ and

\f[
h_g^{FGC}  = (T_g^{FGC}-T_0) \sum_j \Big\{ Y_j^F\, c_{p,g,j} \Big( Y_{H2O,j} + (1-Y_{H2O,j})\Big[ 1+\lambda_{air}^{comb}\, Y_{C,j}\Big(\frac{W_{air}}{W_C}\Big)(1+x/4-y/2)\Big]\Big) \Big\}
\f]

Here, \f$ W_{air} \f$ and \f$ W_C \f$ are the molecular weight of air (28 g/mol) and the atomic weight of carbon (12 g/mol).

The specific enthalpy of the bottom ash is given by

\f[
h_{ba}^{boiler}  = \Big[c_{ba}\,(T_{ba}^{boiler}-T_0) + Y_{C,ba}\, h_C\Big]\, f_{ba}\,\sum_j \Big[ \dot{Y}_j^F\,(1-Y_{H2O,j})\, Y_{ash,j}^F \Big]
\f]

and the specific enthalpy of the flue gas per unit mass feedstock is given by

\f[
h_{s}^{FGC}  = (T_{s}^{FGC}-T_0) \Big[ m_{lime}\, c_{lime} + f_{fa}\, c_{fa}\,\sum_j \dot{Y}_j^F\,(1-Y_{H2O,j})\, Y_{ash,j}^F \Big]
\f]

\note Specific enthalpies:
All specific enthalpies described above are relative to the mass flow of the biomass feedstock \f$ \dot{M}_F \f$.

Using the calculated value for \f$ \dot{H}_F \f$, other material and energy flows for the BioCHP plant are calculated:

- Mass flow rate of the biomass mixture and of each feedstock \f$ j \f$:

 \f[
  \begin{aligned}
    \dot{M}_F & = \dot{H}_F/h_{fuel} \\
    \dot{M}_j^F & = \dot{M}_F \sum_{j=1}^{N_j} Y_j^F \\
  \end{aligned}
  \f]

- Mass flow rate and energy flow rate of bottom ash (\f$ ba \f$) from the boiler:

 \f[
  \begin{aligned}
    \dot{M}_{ba}^{boiler} & = \dot{M}_F \,(1+Y_{C,ba})\, f_{ba}\, \sum_j \Big[ Y_{ash,j}^F\, \dot{Y}_j^F\,(1-Y_{H2O,j}) \Big] \\
    \dot{H}_{ba}^{boiler} & = \dot{M}_F\, h_{ba}^{boiler} \\
  \end{aligned}
  \f]

- Inlet mass flow rate of lime to flue gas cleaning:

 \f[
  \dot{M}_{lime} = \dot{M}_F\, m_{lime,b}\, \sum_j \frac{Y_{S,j} + Y_{Cl,j}}{Y_{S,b}+Y_{Cl,b}}
  \f]

- Mass flow rate and energy flow rate of solid residue (\f$ s \f$) from the flue gas cleaning:

 \f[
  \begin{aligned}
    \dot{M}_{s}^{FGC} & = \dot{M}_F \Big[ m_{lime} + f_{fa}\, \sum_j \dot{Y}_j^F\,(1-Y_{H2O,j})\, Y_{ash,j}^F \Big] \\
    \dot{H}_{s}^{FGC} & = \dot{M}_F\, h_{s}^{FGC} \\
  \end{aligned}
  \f]

- Mass and energy flow rates of flue gas from the CHP plant:

 \f[
  \begin{aligned}
    \dot{M}_{g}^{FGC} & = \dot{M}_F\, \sum_j \dot{Y}_j^F\Big[ Y_{H2O,j} + (1-Y_{H2O,j})\Big( 1+\lambda_{air}^{comb}\, Y_{C,j}\Big(\frac{W_{air}}{W_C}\Big)(1+x/4-y/2)\Big) \Big] \\
    \dot{H}_{g}^{FGC} & = \dot{M}_F\, h_g^{FGC} \\
  \end{aligned}
  \f]

- Thermal energy and mass flow rates of steam (\f$ stm \f$) produced from the boiler:

 \f[
  \begin{aligned}
    \dot{Q}_{stm}^{boiler} & = \frac{\dot{H}_{fuel} - \dot{H}_g^{FGC} - \dot{H}_{ba}^{boiler} - \dot{H}_{s}^{FGC}}{(1-q_{loss})} \\
    \dot{M}_{stm}^{boiler} & = \frac{\dot{Q}_{stm}^{boiler}}{(h_{stm}^{boiler}-h_{bfw}^{boiler})} \\
  \end{aligned}
  \f]

\subsection back-bio_CHP-math-CAPEX CAPEX

The installed cost of equipment \f$ k \f$ is calculated from [1]

\f[
C_{eq,k} = C_{P,k}^B \left(\frac{S_k}{S_k^B}\right)^{n_k} \left(\frac{I}{I_B}\right) f_{inst,k},
\f]

where \f$ C_{P,k}^B \f$ is the purchase cost for a base-case equipment size \f$ S_k^B \f$ in the reference year, \f$ S_k \f$ is the actual equipment size, \f$ n_k \f$ is the equipment scale factor, and \f$ f_{inst,k} \f$ is the installation factor. The ratio \f$ I/I_B \f$ is the price index ratio between the actual year and the reference year (e.g. using the Chemical Engineering Plant Cost Index).

The following equipment cost parameters are used:

| Equipment                         | `S_k^B` | `C_{P,k}^B` (M\$) | `f_{inst,k}` | `n_k` | Base year | ref. |
|-----------------------------------|--------:|------------------:|-------------:|------:|----------:|-----:|
| Biomass storage and preparation   | 25 t/h  | 5.4               | 2.1          | 0.5   | 2007      | [2]  |
| Biomass boiler                    | 25 t/h  | 7.9               | 2.1          | 0.7   | 2007      | [2]  |
| Flue gas cleaning                 | 67 t/h  | 0.18              | 2.7          | 0.7   | 2007      | [3]  |
| Steam turbines and condenser      | 1500 MW | 40.5              | 1.3          | 0.7   | 2006      | [4]  |
| Heat Exchanger (heat export)      | 100 m²  | 0.086             | 2.8          | 0.71  | 2008      | [1]  |

The total equipment cost is then given by

\f[
C_{eq} = \sum_k C_{eq,k}
\f]

The total capital expenditures (CAPEX) is evaluated in terms of the total permanent investment \f$ C_{inv} \f$ adapting the factoral Method by Peters et al. [5] from :

\f[
C_{inv} = \sum_k C_{eq,k}\,(1+f_{pip}+f_{el}+f_{I\&C})\Big[(1+f_{site}+f_{building})+f_{com}\Big](1+f_{cont,k}+f_{eng,k})(1+f_{dev})
\f]

where \f$ C_{eq,k} \f$ denotes the installed cost of equipment and the \f$ f_i \f$ are cost parameters defined [6] as follows:
- \f$ f_{pip} = 0.065 \f$ for interconnecting piping between equipment,
- \f$ f_{el} = 0.05 \f$ for the plant electric system,
- \f$ f_{I\&C} = 0.05 \f$ for the instrumentation and control system,
- \f$ f_{site} = 0.17 \f$ for land and site preparation,
- \f$ f_{building} = 0.20 \f$ for construction of buildings,
- \f$ f_{com} = 0.10 \f$ for commissioning,
- \f$ f_{dev} = 0.02 \f$ for project development and licenses,
- \f$ f_{eng,k} = 0.15 \f$ for engineering, and
- \f$ f_{cont,k} = 0.20 \f$ for contingency.

\subsection back-bio_CHP-math-OPEX OPEX

The OPEX is defined as the total annual operating costs calculated from

\f[
C_{op} = C_{op,d} + C_{op,f},
\f]

where:

- \f$ C_{op,d} \f$ denotes the variable operating cost, proportional to the annual operating time \f$ t_{op} \f$, including the
  - supply of biomass (unit price: 100 \$/t),
  - purchase of lime (unit cost: 0.25 \$/kg),
  - disposal of flue gas cleaning solid residue (unit cost: 40.0 \$/t), and
  - disposal of bottom ash (unit cost: 20.0 \$/t).
- \f$ C_{op,f} \f$ denotes the fixed operating costs required for keeping the BioCHP plant in operation, including:
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

1) Plant manager:

\f[
\begin{aligned}
  c_{b,k}^{labor} & = 162~k\$/year \\
  N_k^{labor} & = 1 \\
  f_{oh,k}^{labor} & = 0.0 \\
\end{aligned}
\f]

2) O\&M manager:

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

3) O\&M engineer:

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

4) Shift operator:

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

\section input_data Input data

All input data used by the bioCHP plant modules is implenented using TOML format [ref] within the 'src/Database' Directory. Values for all physical properties are defined in SI units.      

The model includes the following input data:

1. Input parameters for the bioCHP plant and subcomponents
2. Flows data, including flow class and prop_data
3. Molecules data, including composition (chemical formula) and molecular weight (kg/mol)   
4. Atoms data, including atomic weight (kg/mol)
4. Capital cost data, including parameters


\section references References

[] https://toml.io/en/

[1] Woods, D. R., Rules of Thumb. Rules of Thumb in Engineering Practice. Wiley-VCH: 2008

[2] Tomberlin, G. (2014). Wood Pellet-Fired Biomass Boiler Project at the Ketchikan Federal Building. National Renewable Energy Laboratory (NREL). https://doi.org/10.2172/1171779

[3] J. Stubenvoll, S. Böhmer and I. Szednyj, "State of the Art for Waste Incineration Plants," Federal Ministry of Agriculture and Forestry, Environment and Water Management, Vienna 2002. ISBN 3-902 338-13-X 

[4] M. C. Woods, P. J. Capicotto, J. L. Haslbeck, N. J. Kuehn, M. Matuszewski, L. L. Pinkerton, M. D. Rutkowski, R. L.
Schoff, V. Vaysman, Cost and Performance Baseline for Fossil Energy Plants. Volume 1: Bituminous coal and natural
gas to electricity. Report DOE/NETL-2007/1281, 2007. URL: https://www.nrc.gov/docs/ML1217/ML12170A423.pdf.

[5] Peters, M. S.; Timmerhaus, K. D.; West, R. E.; Timmerhaus, K.; West, R., Plant design and economics for chemical engineers, Vol. 4.. 5th ed., McGraw-Hill New York: 2003,

[6] Ereev, S..; Patel, M. Recommended methodology and tool for cost estimates at micro level for new technologies. Utrecht, October 2011.
http://www.prosuite.org/c/document_library/get_file?uuid=0efb1401-9854-4b92-8741-b0955c387cfa&groupId=12772


