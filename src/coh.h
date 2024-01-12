#ifndef _coh_h_
#define _coh_h_


#include "params.h"
#include "nucleus.h"
#include "event1.h"

////////////////////////////////////////////////////////////////////////
void cohevent_cj(params &p, event &e, nucleus &t, bool prad, bool fast=1);
////////////////////////////////////////////////////////////////////////
void cohevent_bs(params &p, event &e, nucleus &t, bool prad, bool fast=1);
////////////////////////////////////////////////////////////////////////

#endif

