# Nanalysis

Nanalysis a tool for analysing root files from Nuwro Monte Carlo neutrino event generator, created at the University of Wroclaw. Nuwro simulates neutrino-nucleon and neutrino-nucleus reactions for energies from threshold to TeV. The generator has a detector geometry module and can handle realistic neutrino beams, which make it suitable to use in neutrino experiments. [ROOT](https://root.cern.ch/) framework is used to store generated events. Nanalysis can be loaded with nuwro event file to produce exclusive inclusive phase-space diagrams as well as creating TNtuples out of tree from the *.root files generated by nuwro

More information can be found in [NuWro User Guide](https://nuwro.github.io/user-guide/).

# Installation

* Install [ROOT](https://root.cern.ch/) with [PYTHIA6](https://pythia6.hepforge.org/)
* Set *ROOTSYS* to the folder where ROOT is installed
* In NuWro directory type:

  ```
  make
  ```

For more details see [user-guide/getting-started/installation](https://nuwro.github.io/user-guide/getting-started/installation/).

# Usage

* Add *nuwro/bin* to your *PATH*
* Copy *data/params.txt* to your working directory and modify as needed (see [user-guide/getting-started/parameters](https://nuwro.github.io/user-guide/getting-started/parameters/))
* Type

  ```
  nuwro
  ```

# Credits

Currently involved in the project:

* Hemant Prasad
(Advisor Jan T. Sobczyk)

