# Example Application: Minimum Viable bioCHP Case

This document describes a **minimum viable example** that demonstrates how to define and execute a small-scale case for the biomass-based Combined Heat and Power (bioCHP) plant model.

The `bioCHP` model simulates the **conversion of solid biomass** into combined heat and electricity under **steady-state conditions**. It evaluates the **mass and energy balances** of the CHP plant, as well as the **economic performance** in terms of capital (CAPEX) and operational (OPEX) costs.

For detailed information on the bioCHP model and implementation, please refer to `docs/bioCHP_model_description.md`

For detailed information about:
- Installation requirements,  
- Compilation instructions, and  
- Execution on Linux or Windows,  

please refer to the **[main README.md page](@ref quickstart)**.  

---

## 1. Example Description

In this example, the bioCHP plant is defined by:
- The biomass mixture feeding the plant, composed of four different type of wood feedstock quantified in terms of their mass fraction.
- The target for electricity generation, quentified in terms of power output
- Two independent district heating demands, defined by their thermal power (MW) and their return/supply temperatures. 

### (a) Model Inputs


| Parameter | Symbol | Description | Example |
|------------|---------|-------------|----------|
| `fuel_def` | – | Names of the biomass feedstocks (must exist in model database) | `["spruce_stem", "birch_stem", "spruce_TandB", "spruce_bark"]` |
| `Yj` | \f$ Y_j^F \f$ | Mass fraction of each biomass in the feed mixture (sums to 1) | `[0.1, 0.3, 0.4, 0.2]` |
| `YH2Oj` | \f$ Y_{H_2O,j}^F \f$ | Moisture content (mass fraction) of each biomass | `[0.4, 0.35, 0.45, 0.5]` |
| `W_el` | \f$ \dot{W}_{el} \f$ | Target electric power output [MW] | `100.0` |
| `Qk` | \f$ \dot{Q}_{k} \f$ | Thermal power supplied to each heat user [MW] | `[30.0, 40.0]` |
| `Tk_in` | \f$ T_{in,k} \f$ | Return temperature for each heat user [°C] | `[70.0, 50.0]` |
| `Tk_out` | \f$ T_{out,k} \f$ | Supply temperature for each heat user [°C] | `[90.0, 80.0]` |


The input in this example are implented using toml format in the file located at: `docs/example/bioCHP_input.toml`

```toml
# Example input for bioCHP_plant function
[inputs]
    fuel_def = ["spruce_stem", "birch_stem", "spruce_TandB", "spruce_bark"]
    Yj = [0.1, 0.3, 0.4, 0.2]
    YH2Oj = [0.4, 0.35, 0.45, 0.5]
    W_el = 100.0
    Qk = [30.0, 40.0]
    Tk_in = [70.0, 50.0]
    Tk_out = [90.0, 80.0]
```

### (b) Model Outputs

The main outputs from the bioCHP model are the quantified **required input feedstock**, **capex** interms of capital investment, and **opex**. 

A detailed description of the outputs from the bioCHP model is shown in the tanle below:

| Output | Symbol | Description | Units |
|---------|---------|-------------|--------|
| `Mj` | \f$ \dot{M}_j^F \f$ | Mass flow rate of each biomass component in feed mixture | t/h |
| `Hfj` | \f$ \dot{H}_j^F \f$ | Energy contribution from each biomass component | MW |
| `Biomass_mass_input_(t/h)` | \f$ \dot{M}_F \f$ | Total biomass feed to the plant | t/h |
| `Biomass_energy_input_(MW)` | \f$ \dot{H}_F \f$ | Total thermal input from biomass | MW |
| `Heat_production_(MW)` | \f$ \sum_k \dot{Q}_k \f$ | Total heat supplied to district heating users | MW |
| `Electricity_production_(MW)` | \f$ \dot{W}_{el} \f$ | Net electrical power output | MW |
| `C_op_mat` | – | Cost of consumables (e.g. biomass, lime) | USD/year |
| `C_op_el` | – | Cost of electricity for auxiliaries | USD/year |
| `C_op_maint` | \f$ C_{maint} \f$ | Maintenance cost (≈5% of equipment cost) | USD/year |
| `C_op_ins` | \f$ C_{ins} \f$ | Insurance cost (≈1% of investment) | USD/year |
| `C_op_adm` | \f$ C_{adm} \f$ | Administrative cost (≈3% of investment) | USD/year |
| `C_op_var` | \f$ C_{op,d} \f$ | Direct variable operating costs | USD/year |
| `C_op` | \f$ C_{op} \f$ | Total operating costs (variable + fixed) | USD/year |
| `C_inv` | \f$ C_{inv} \f$ | Total investment | USD |

These output results are written to a plain text file `output.txt`:

```txt
plant bioCHP_PLANT Mj num 2.55675 7.67026 10.227 5.1135
plant bioCHP_PLANT Hfj num 47.1976 61.8726 188.947 96.5762
plant bioCHP_PLANT Biomass_mass_input_(t/h) num 92.0431
plant bioCHP_PLANT Biomass_energy_input_(MW) num 394.593
plant bioCHP_PLANT Heat_production_(MW) num 70
plant bioCHP_PLANT Electricity_production_(MW) num 100
plant bioCHP_PLANT C_inv num 2.4302e+08
plant bioCHP_PLANT C_op_mat num 3.79187e+07
plant bioCHP_PLANT C_op_el num 2.65084e+06
plant bioCHP_PLANT C_op_maint num 2.58165e+06
plant bioCHP_PLANT C_op_ins num 1.29083e+06
plant bioCHP_PLANT C_op_adm num 1.29083e+06
plant bioCHP_PLANT C_op num 4.57329e+07
plant bioCHP_PLANT C_op_var num 4.05695e+07
```

## 2. Example Interpretation

These output results allow estimation of:
- **Total biomass required** to sustain the desired power outputs,
- **Energy conversion efficiency** (electrical and overall),
- **Operating cost structure** for the bioCHP system.

For the given example:
- The total **biomass energy input** is approximately **395 MW**.  
- The model produces **100 MW of electricity** and **70 MW of useful heat**, corresponding to an **overall efficiency** of about **43%**, assuming nominal conversion losses.  
- The total biomass feed rate is around **92 t/h**, representing a large-scale CHP plant.
- The required total investment is around **243 M USD**
- The total annual operating cost is approximately **45.7 M USD**, where around **40.6 M USD** are variable operating cost due to consumables including the supply cost of biomass.

## 3. Customization

The values of the parameters defined in the TOML input for this example can be modified:
- **Feedstock mix:** alter `fuel_def`, `Yj`, or `YH2Oj` to simulate alternative biomass blends.
- **Plant capacity:** change `W_el` to represent smaller or larger capacity of the CHP plant.
- **Heat demands:** modify `Qk`, `Tk_in`, and `Tk_out` to analyze the impact of district heating demands on the process and economic performance.

Additional input data can also be modified:
- Introducing new types of wood feedstock can be reated within the flows database (`src/Database/flows.toml`) by replicating existing feedstock using the same characterization
- Process parameters of the bioCHP plant, within the file (`src/Database/bioCHP_input.toml`)
- Prices and equipment cost aparemeters, within the file (`src/Database/costs.toml`)
