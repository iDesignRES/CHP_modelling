\# Example (minimum viable case)



This example represents a minimum viable case for running the bioCHP function as part of the EMX package, using the following inputs:



\- Solid feedstock defined by a mixture of four types of biomass: fuel\_def = \["spruce\_stem", "birch\_stem", "spruce\_TandB", "spruce\_bark"]

\- The mass fraction of each biomass component in the feedstock: Yj = \[0.1, 0.3, 0.4, 0.2]

\- The moisture content, as mass fraction, of each biomass component in the feedstock: YH2Oj = \[0.4, 0.35, 0.45, 0.5]

\- Electric Power Production (MW) of the bioCHP plant: W\_el = 100.0

\- The bioCHP supply heat to two separate external users specified by thermal power (MW): Qk = \[30.0, 40.0]

\- Return temperature for each heat demand (deg. C): Tk\_in = \[70.0, 50.0]

\- Supply temperature for each heat demand (deg. C): Tk\_out = \[90.0, 80.0]



