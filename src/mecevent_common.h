#ifndef _mecevent_common_h_
#define _mecevent_common_h_

#include "pdg.h"
#include "particle.h"

// Size of the randomization region in energy transfer
extern double width_q0;

// Lepton mass
extern double ml;
extern double ml2;

// Reaction threshold or binding energy
extern double Bmec;

// Antinetrino
extern int ap;

// Nucleus heavier than nitrogen
extern int nucl;

// Pauli Blocking?
extern bool PB;

// How many repetitions in kinematic sampling
static int calls_max=50;

// Nucleon mass
const double MN = (PDG::mass_proton + PDG::mass_neutron) / 2.0;
const double MN2 = MN * MN;

const double MN_proton = PDG::mass_proton;
const double MN2_proton = MN_proton * MN_proton;

////////////////////////////////////////

// This sets all final nucleon isospin behaviour (T.G.)
void mec_do_cc (particle *p, double ratio_pp, double ratio_np);
void mec_do_cc_Nieves (particle *in_p, particle *out_p, double ratio_pp, double ratio_np);
void mec_do_cc_Nieves_3p3h (particle *in_p, particle *out_p);
void mec_decide_3p3h_2p2h(double ratio_2p2h, bool& flag_2p2h);
#endif
