#ifndef _mecevent_Nieves_h_
#define _mecevent_Nieves_h_

#include <cmath>

#include "particle.h"
#include "jednostki.h"
#include "params.h"
#include "beam.h"
#include "pdg.h"
#include "nucleus.h"
#include "event1.h"
#include "TFile.h"
#include "TMatrixD.h"

// Global variables and common functions in Jakub's implementation
#include "mecevent_common.h"

// Data file with hadronic tensor elements
#include "Nieves_MEC.h"
#include "New_Grid_2p2h_pp.h"
#include "New_Grid_2p2h_np.h"
#include "New_Grid_2p2h_pn.h"
#include "New_Grid_3p3h.h"

// Cut in momentum transfer for Valencia model
const double qmax_Nieves=1.2*GeV;

// Efffecitive CC binding energies in MeV: Carbon-neutrino, Carbon-antineutrino
// Oxygen-neutrino, Oxygen-antineutrino
static double qvalues_Nieves[6]={16.827*MeV,13.880*MeV,14.906*MeV,10.931*MeV,13.809*MeV,1.822*MeV};
//static double qvalues_Nieves[6]={16.827*MeV,13.880*MeV,14.906*MeV,10.931*MeV,14.906*MeV,10.931*MeV};

// Generate event
void mecevent_Nieves (params & p, event & e, nucleus & t, bool cc);

// Generate lepton kinematics and calculate the cross section
double Nieves_kin_and_weight (double E, double *individual_dsdqdw, particle &meclep, particle *inc_nucleon_2p2h, particle *out_nucleon_2p2h, particle *inc_nucleon_3p3h, particle *out_nucleon_3p3h, bool &flag_2p2h_dynamics, nucleus &t,
                              double mec_central, double mec_smearing, double binding,
                              int ile_pb, double sampling, int strength, int skalowanie);

// Double-differential cross section
double Nieves_dsdEdc (double E, int outgoing_pair, double q0, double Ep, double ct, int skalowanie);

void Generate_nucleon_dynamics_2p2h(particle *inc_nucleon_mec, particle *out_nucleon_mec, nucleus &T, double& xsec_result, vect qqq, double mec_central, double mec_smearing, double binding, int ile_pb, double sampling, int strength);
void Generate_nucleon_dynamics_3p3h(particle *inc_nucleon_mec, particle *out_nucleon_mec, nucleus& T, double& xsec_result, vect qqq, int ile_PB);
void threebodydecay (double W, double m1, double m2, double m3, vect &p1, vect &p2, vect& p3);
#endif
