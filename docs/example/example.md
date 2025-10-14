# Example Application: Minimum Viable bioCHP Case

This document describes a **minimum viable application** of the `bioCHP` model.  
It demonstrates how to define and execute a small-scale case for the biomass-based Combined Heat and Power (bioCHP) plant model.

---

## 1. Overview

The `bioCHP` model simulates the **conversion of solid biomass** into combined heat and electricity under **steady-state conditions**.  
It evaluates the **mass and energy balances** of the CHP plant, as well as the **economic performance** in terms of capital (CAPEX) and operational (OPEX) costs.

The plant is represented as an integrated process system including:
- Biomass storage and feeding,
- Combustion and steam generation,
- Flue gas cleaning and residue management,
- A Rankine cycle with electricity production and multiple heat extractions,
- Heat exchange for district heating, and
- Economic evaluation modules for CAPEX and OPEX.

For detailed information about:
- Installation requirements,  
- Compilation instructions, and  
- Execution on Linux or Windows,  

please refer to the **[main README.md page](@ref quickstart)**.  

The example presented here uses the input file located at: `docs/example/bioCHP_input.toml`

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

---

## 2. Example Description

This case defines a mixed feedstock, a target for electricity generation, and two thermal energy demands.

The plant in this example is designed to:
- Deliver **100 MW of electric power**,  
- Provide **70 MW of district heating** (30 MW + 40 MW) at two temperature levels, and  
- Use a **biomass blend** composed of spruce and birch resources with different moisture contents.

---

## 3. Model Inputs

The model requires two main categories of inputs: (a) biomass feedstock definition and (b) plant operating specifications.

### (a) Biomass Feedstock Inputs

| Parameter | Symbol | Description | Example |
|------------|---------|-------------|----------|
| `fuel_def` | – | Names of the biomass feedstocks (must exist in model database) | `["spruce_stem", "birch_stem", "spruce_TandB", "spruce_bark"]` |
| `Yj` | \f$ Y_j^F \f$ | Mass fraction of each biomass in the feed mixture (sums to 1) | `[0.1, 0.3, 0.4, 0.2]` |
| `YH2Oj` | \f$ Y_{H_2O,j}^F \f$ | Moisture content (mass fraction) of each biomass | `[0.4, 0.35, 0.45, 0.5]` |

Each feedstock is characterized internally by:
- Atomic composition (C, H, O, N, S, Cl),
- Ash content,
- Lower Heating Value (LHV),
- Density and thermodynamic correlations.

These are stored in the model database (`src/Database/flows.toml`).

---

### (b) Plant Operation and Demand Inputs

| Parameter | Symbol | Description | Example |
|------------|---------|-------------|----------|
| `W_el` | \f$ \dot{W}_{el} \f$ | Target electric power output [MW] | `100.0` |
| `Qk` | \f$ \dot{Q}_{k} \f$ | Thermal power supplied to each heat user [MW] | `[30.0, 40.0]` |
| `Tk_in` | \f$ T_{in,k} \f$ | Return temperature for each heat user [°C] | `[70.0, 50.0]` |
| `Tk_out` | \f$ T_{out,k} \f$ | Supply temperature for each heat user [°C] | `[90.0, 80.0]` |

Each heat user corresponds to a **district heating loop**, simulated as a heat exchanger where steam from the turbine condenses to provide useful heat.

---

## 4. Model Outputs

The bioCHP model produces both **technical** and **economic** results.  
The results are written to a plain text file `output.txt`:

```txt
plant bioCHP_PLANT Mj num 2.55675 7.67026 10.227 5.1135
plant bioCHP_PLANT Hfj num 47.1976 61.8726 188.947 96.5762
plant bioCHP_PLANT Biomass_mass_input_(t/h) num 92.0431
plant bioCHP_PLANT Biomass_energy_input_(MW) num 394.593
plant bioCHP_PLANT Heat_production_(MW) num 70
plant bioCHP_PLANT Electricity_production_(MW) num 100
plant bioCHP_PLANT C_op_mat num 3.79187e+07
plant bioCHP_PLANT C_op_el num 2.65084e+06
plant bioCHP_PLANT C_op_maint num 2.58165e+06
plant bioCHP_PLANT C_op_ins num 1.29083e+06
plant bioCHP_PLANT C_op_adm num 1.29083e+06
plant bioCHP_PLANT C_op num 4.57329e+07
plant bioCHP_PLANT C_op_var num 4.05695e+07
```

with the following detailed explanation

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

These results allow estimation of:
- **Total biomass required** to sustain the desired power outputs,
- **Energy conversion efficiency** (electrical and overall),
- **Operating cost structure** for the bioCHP system.

---

## 6. Example Interpretation

For the given example:
- The total **biomass energy input** is approximately **395 MW**.  
- The model produces **100 MW of electricity** and **70 MW of useful heat**, corresponding to an **overall efficiency** of about **43%**, assuming nominal conversion losses.  
- The total biomass feed rate is around **92 t/h**, representing a large-scale CHP plant.

The detailed cost breakdown (`C_op_*`) quantifies the impact of fuel, operation, and maintenance on plant economics.  
By adjusting the feedstock mix, moisture contents, or power-to-heat ratio, users can explore the sensitivity of technical and economic performance.

---

## 7. Customization

You can modify the example by changing parameters in the TOML input:
- **Feedstock mix:** alter `fuel_def`, `Yj`, or `YH2Oj` to simulate alternative biomass blends.
- **Plant capacity:** change `W_el` to represent smaller or larger installations.
- **Heat demands:** modify `Qk`, `Tk_in`, and `Tk_out` to reflect district heating requirements.