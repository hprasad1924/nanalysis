#include "Fillhistograms.h"
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <string.h>
#include "TFile.h"
#include "TNtupleD.h"
#include "TH1.h"
#include "TH2.h"
#include "TLatex.h"
#include "TVectorD.h"
#include "TStyle.h"
#include "TCanvas.h"

#ifndef _fittings_h_
#define _fittings_h_

void Load_Valencia_Samples(TH2D *real_hist, double &Normalization_1, Int_t &Normalization_2, int choice);
void Load_Nuwro_Samples(TH2D *real_hist, int choice, std::vector<double> *q, std::vector<double> *q0, std::vector<double> *p1out, std::vector<int> *pdg1, std::vector<double> *p2out, std::vector<int> *pdg2, std::vector<double> *kos, std::vector<double> *cosboundary, std::vector<double> *e_weight);

void Restrict_Phase_space( TH2D *hist, TH2D *sample_hist, TH2D *restrictor, double totalxsec, Int_t nentries, int choice);
void Fit_2D_histogram(TH2D *hist, TH2D *reweight_hist, double &chisquare, int &dof, TH2D *org_hist, std::vector<double> *q, std::vector<double> *q0, std::vector<double> *p1out, std::vector<int> *pdg1, std::vector<double> *p2out, std::vector<int> *pdg2, std::vector<double> *kos, std::vector<double> *cosboundary, std::vector<double> *e_weight, double par, int n);




#endif // !_fittings_h_

