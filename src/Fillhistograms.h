#ifndef _Fillhistograms_h_
#define _Fillhistograms_h_

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "event1.h"
#include "vect.h"
#include <functional>
#include <iostream>
#include <stdio.h>


double coshad(vect p1out, vect p2out);
double coshad(vect p1out, vect p2out, double &beta, double &gamma, double &E);
double reweighting_function(double kos, double Par, int n, double kappa);

void Fillhist_ct(TH1D *hist, event *ev, bool reweight, double P, int n);
void Fillhist_inclusive(TH2D *hist, event *ev, bool reweight, double P, int n);
void Fillhist_exclusive(TH2D *hist, TH2D *hist2, event *ev, bool reweight, double P, int n);


#endif


