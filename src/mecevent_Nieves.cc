/*
*		Author : Jakub
* 		Updated by : Hemant Prasad
* 		Date of Modification : 07/01/2024
*
* 		Major Changes:
* 		1. Reads new Hadronic Grid. Grids are stored in New_Grid_2p2h_*.h and New_Grid_3p3h.h.
* 		2. mec_do_cc() is removed from mecevent() and used in Nieves_kinematics() function.
* 		3. 3 New function .
* 			3a. Generate_2p2h_dynamics()
* 			3b. Generate_3p3h_dynamics()
* 			3c. threebodydecay();
* 		4. This file is used for calculated 2p2h and 3p3h cross section.
* 		
* 		
*
*/

#include "mecevent_common.h"
#include "mecevent_Nieves.h"
#include "TMath.h"


double przeskalowanie (double w, double q)
{
  int w_int = floor(w/50);
  int q_int = floor(q/50);
  
  return przeskalowanie_mec [w_int][q_int];
};


void mecevent_Nieves (params & p, event & e, nucleus & t, bool cc)
{

  e.par = p;            // save params in the event
  e.flag.cc = cc;       // set flags for the event
  e.flag.nc = !cc;
  e.flag.dis = false;
  e.flag.qel = false;
  e.flag.coh = false;
  e.flag.mec = true;
  int ile_pb = p.mec_pb_trials;
  double mec_central = p.mec_central_motion;
  double mec_smearing = p.mec_back_to_back_smearing;
  double binding = p.kaskada_w;
  double mc_sampling = p.MEC_cm_direction;
  int mc_strength = p.MEC_cm_strength;
  int mecskalowanie = p.mec_scaling;

  // sadly, only CC events available so far...
  if(e.flag.nc)
  {
    cerr<<" MEC error: Wrong Settings!\n";
    e.weight = 0;
    return;
  }

  particle meclepton;
  ap=(e.in[0].pdg<0);
  //if(e.in[0].pdg>0) meclepton.pdg = e.in[0].pdg-1;  //all leptons available
  //else {meclepton.pdg = e.in[0].pdg+1;}
  meclepton.pdg = e.in[0].pdg-1+2*ap;
  meclepton.set_mass (PDG::mass (meclepton.pdg)); //set mass coresponding to pdg
  ml=meclepton.mass();
  ml2=ml*ml;

  //nucleus and Pauli blocking
  nucl=(t.p>7);//+(t.p>15);
  PB=p.MEC_pauli_blocking;

  //binding:either carbon or oxygen or calcium neutrino/antineutrino
  Bmec=qvalues_Nieves[2*nucl+(e.in[0].pdg<0)];

  // Qvalue threshold...
  //double q0max = e.in[0].energy() - ml - Bmec;
  //width_q0 = q0max;

  particle incoming_mecnucleon_2p2h[2];
  particle outgoing_mecnucleon_2p2h[2];

  particle incoming_mecnucleon_3p3h[3]; //for 2p2h -> 0,1  for 3p3h ->0,1,2
  particle outgoing_mecnucleon_3p3h[3]; //for 2p2h -> 0,1,2 for 3p3h ->0,1,2

  bool flag_2p2h = true;
  double weight=0;
  double individual_weight[4] = {4};

  weight=Nieves_kin_and_weight (e.in[0].E(), individual_weight, meclepton, incoming_mecnucleon_2p2h, outgoing_mecnucleon_2p2h, incoming_mecnucleon_3p3h, outgoing_mecnucleon_3p3h, flag_2p2h, t, mec_central, mec_smearing, binding, ile_pb, mc_sampling, mc_strength, mecskalowanie);
  

  e.weight = weight;
  e.flag_2p2h = flag_2p2h;
  
  if(e.flag_2p2h)
  {
    e.in.push_back(incoming_mecnucleon_2p2h[0]);
    e.in.push_back(incoming_mecnucleon_2p2h[1]);
    e.out.push_back(meclepton);
    e.out.push_back(outgoing_mecnucleon_2p2h[0]);
    e.out.push_back(outgoing_mecnucleon_2p2h[1]);
  }
  else 
  {
    e.in.push_back(incoming_mecnucleon_3p3h[0]);
    e.in.push_back(incoming_mecnucleon_3p3h[1]);
    e.in.push_back(incoming_mecnucleon_3p3h[2]);
    e.out.push_back(meclepton);
    e.out.push_back(outgoing_mecnucleon_3p3h[0]);
    e.out.push_back(outgoing_mecnucleon_3p3h[1]);
    e.out.push_back(outgoing_mecnucleon_3p3h[2]);

  }

  // corrections for A>39;
  // combinatoric factors with respect to hypothetical isosymmetric target here approximated by calcium
  // K.N.: but actually oxygen is used...
  int NN=p.nucleus_n;
  int ZZ=p.nucleus_p;

  if ( NN+ZZ>39 )
  {
  double NZ=(NN+ZZ)/2.0;

    if(e.flag_2p2h)
    {
      // neutrino on neutron-neutron pair
      if (e.in[0].pdg>0  && (incoming_mecnucleon_2p2h[0].pdg +  incoming_mecnucleon_2p2h[1].pdg) ==4224 )
      {
        double renorm = NN*(NN-1)/NZ/(NZ-1);
        weight=weight*renorm;
      }

      //neutrino on neutron-proton pair
      if (e.in[0].pdg>0  && (incoming_mecnucleon_2p2h[0].pdg +  incoming_mecnucleon_2p2h[1].pdg) ==4324 )
      {
        double renorm = NN*ZZ/NZ/NZ;
        weight=weight*renorm;
      }

      //neutrino on proton-proton pair
      if (e.in[0].pdg<0  && (incoming_mecnucleon_2p2h[0].pdg +  incoming_mecnucleon_2p2h[1].pdg) ==4424 )
      {
        double renorm = ZZ*(ZZ-1)/NZ/(NZ-1);
        weight=weight*renorm;
      }

      // neutrino on neutron-proton pair
      if (e.in[0].pdg<0  && (incoming_mecnucleon_2p2h[0].pdg +  incoming_mecnucleon_2p2h[1].pdg) ==4324 )
      {
        double renorm = NN*ZZ/NZ/NZ;
        weight=weight*renorm;
      }
    }
  }

}


double Nieves_kin_and_weight(double E, double *individual_dsdqdw, particle &meclep, particle *inc_nucleon_2p2h, particle *out_nucleon_2p2h, particle *inc_nucleon_3p3h, particle *out_nucleon_3p3h, bool &flag_2p2h_dynamics, nucleus &t, double mec_central, double mec_smearing, double binding, int ile_PB, double sampling, int strength, int skalowanie)
{
  //It is assumed that neutrino direction is (0, 0, 1): but transition to other direction in nuwro.cc !
  // here we extrapolate the cross section from Nives's grid file.
  double result = 0; 
  double result_2p2h = 0;
  double result_3p3h = 0;
  //Kinematic variables which will be assigned differently for 2p2h and 3p3h
  double q0max = 0;
  double Eout_lepton = 0, q0 = 0, lp2 = 0, lp = 0, cos_min = 0, ct = 0;

  double ratio_2p2h = 0;
  double ratio_2p2h_pp = 0;
  double ratio_2p2h_np = 0;

  double phi = 0;
  vec qq_2p2h(0,0,0), qq_3p3h(0,0,0);
  vec kprim_2p2h(0,0,0), kprim_3p3h(0,0,0);
  vect qqq_2p2h (qq_2p2h, 0), qqq_3p3h(qq_3p3h, 0);

  ////////////////////////////////////////////////////////////////////////////////////////////
  ///                                   block for 2p2h                                     ///
  ////////////////////////////////////////////////////////////////////////////////////////////

  {
    // Qvalue threshold ...
    q0max = E - ml - Bmec;
    width_q0 = q0max;
    // Energy of the outgoing lepton momentum as defined in PhysRev C.102.024641 (Exclusive model code)
    Eout_lepton = ml + width_q0*frandom();
    q0 = E - Eout_lepton - Bmec;
    // square of final lepton momentum
    lp2 = Eout_lepton * Eout_lepton - ml2;
    // final lepton momentum
    lp = sqrt(lp2);

    // minimum cosine boundary from momentm transfer
    cos_min = 0.5 * (E * E + lp2 - qmax_Nieves * qmax_Nieves)/E/lp;
    if(cos_min < -1.0) 
    {
      cos_min = -1.0;
    }
    if(cos_min < 1)
    {
      double width_cos = 1.0 - cos_min;
      // we randomize scattering angle
      ct = cos_min + width_cos * frandom();

      //squared momentum transfer
      double q2 = E * E + lp2 - 2 * E * lp * ct;
      if( q2 > (q0*q0) )
      {

              
              //	    Outgoing pairs:
              //	    1 -> pp
              //	    2 -> np
              //	    3 -> pn
              //	    4 -> 3p3h
        
        for(int outgoing_pair = 0; outgoing_pair < 3; outgoing_pair++)
        {
          individual_dsdqdw[outgoing_pair] = Nieves_dsdEdc(E, outgoing_pair+1, q0, Eout_lepton, ct, skalowanie) * width_cos * width_q0;
          result_2p2h += individual_dsdqdw[outgoing_pair];
        }
        q0 += Bmec;

        if(result_2p2h)
        {
          phi = 2.0 * Pi * frandom();

          qq_2p2h = vec(cos(phi) * lp * sqrt(1.0 - ct * ct), sin(phi) * lp * sqrt(1.0 - ct * ct), E - lp * ct);

          //lepton momentum
          kprim_2p2h  = vec( -cos(phi) * lp * sqrt(1.0 - ct * ct), -sin(phi) * lp * sqrt(1.0 - ct * ct), lp * ct);

          //4- momentum transfer
          qqq_2p2h = vect(qq_2p2h, q0);

          ratio_2p2h_pp = individual_dsdqdw[0] / result_2p2h;
          ratio_2p2h_np = individual_dsdqdw[1] / (individual_dsdqdw[1] + individual_dsdqdw[2]);
          Generate_nucleon_dynamics_2p2h(inc_nucleon_2p2h, out_nucleon_2p2h, t, result_2p2h, qqq_2p2h, mec_central, mec_smearing, binding, ile_PB, sampling, strength);
          mec_do_cc_Nieves (inc_nucleon_2p2h, out_nucleon_2p2h, ratio_2p2h_pp, ratio_2p2h_np);

        }
      }
    }

  }

  
  ////////////////////////////////////////////////////////////////////////////////////////////
  ///                                   block for 3p3h                                     ///
  ////////////////////////////////////////////////////////////////////////////////////////////
  {
    //Qvalue threshold
    q0max = E - ml;
    width_q0 = q0max;
    q0 = frandom() * width_q0;
    // energy of the outgoing lepton
    Eout_lepton =  E - q0;
    //square of the final lepton momentum
    lp2 = Eout_lepton*Eout_lepton - ml2;
    //final lepton momentum
    lp = sqrt(lp2);

    //minimum cosine boundary from momentum transfer cut 
    cos_min = 0.5 * (E * E + lp2 - qmax_Nieves * qmax_Nieves )/E/lp;
    if(cos_min < -1.0)
    {
      cos_min  = -1.0;
    }
    if( cos_min < 1 )
    {
      double width_cos = 1.0 - cos_min;
      //we randomize scattering angle
      ct = cos_min + width_cos * frandom();
      
      //squared momentum transfer
      double q2 = E * E + lp2 - 2 * E * lp * ct;
      if( q2 > (q0*q0) )
      {
        result_3p3h = Nieves_dsdEdc(E, 4, q0, Eout_lepton, ct, skalowanie) * width_cos * width_q0;
        individual_dsdqdw[3] = result_3p3h;
      }

      if(result_3p3h)
      {
        phi = 2.0 * Pi * frandom();

        qq_3p3h = vec(cos(phi) * lp * sqrt(1.0 - ct * ct), sin(phi) * lp * sqrt(1.0 - ct * ct), E - lp * ct);

        //lepton momentum
        kprim_3p3h  = vec( -cos(phi) * lp * sqrt(1.0 - ct * ct), -sin(phi) * lp * sqrt(1.0 - ct * ct), lp * ct);

        //4- momentum transfer
        qqq_3p3h = vect(qq_3p3h, q0);

        mec_do_cc_Nieves_3p3h(inc_nucleon_3p3h, out_nucleon_3p3h);
        Generate_nucleon_dynamics_3p3h(inc_nucleon_3p3h, out_nucleon_3p3h, t, result_3p3h, qqq_3p3h, ile_PB);

      }
    }
  }


  ////////////////////////////////////////////////////////////////////////////////////////////
  ///                         Generate nucleon kinematics                                  ///
  ////////////////////////////////////////////////////////////////////////////////////////////

  if ( (result_2p2h > 0) || (result_3p3h > 0) )
  {
    result = result_2p2h + result_3p3h;
    ratio_2p2h = result_2p2h / result;
    mec_decide_3p3h_2p2h(ratio_2p2h, flag_2p2h_dynamics);

    if (flag_2p2h_dynamics)
    {     
      //Set lepton momentum
      meclep.set_momentum(kprim_2p2h);
    }
    else 
    {
      //Set Lepton momentum
      meclep.set_momentum(kprim_3p3h);
    }

  }


  return result;
}

double Nieves_dsdEdc(double E, int outgoing_pair, double q0, double Ep, double ct, int skalowanie)
{
  double result=0;
  double lp=sqrt(Ep*Ep-ml2);
  double vq2=E*E+lp*lp-2*E*lp*ct;
  if((vq2>q0*q0)&&(vq2>0)&&(fabs(ct)<=1))
  {
    double q=sqrt(vq2);

    // interpolating the nuclear tensor elements
    double w00=0;
    double w03=0;
    double w11=0;
    double w12=0;
    double w33=0;
    //q0-=Bmec;
    if((q0<=q)and(q>=0.01*GeV)and(q0>=0)and(q<=qmax_Nieves))
    {
      double spacing=0.01*GeV;
      int m=int((q-spacing)/spacing);
      int n=int(q0/spacing);
      // q0-B<0.1 GeV extrapolation with 0 at q0-B=0
        //------------------------------------ New Hadronic Tables----------------------------------//
        int y2 = (n+1)*spacing;
        int y1 = n*spacing;
        int x2 = (m+1)*spacing;
        int x1 = m*spacing;
        
        switch(outgoing_pair)
        {
            case 1:
            if( q < spacing)
            {
              w00 = ((y2 - q0)*(q - x1)*Nw00pp[n][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw00pp[n+1][m])/(spacing*spacing);
              w03 = ((y2 - q0)*(q - x1)*Nw03pp[n][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw03pp[n+1][m])/(spacing*spacing);
              w11 = ((y2 - q0)*(q - x1)*Nw11pp[n][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw11pp[n+1][m])/(spacing*spacing);
              w12 = ((y2 - q0)*(q - x1)*Nw12pp[n][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw12pp[n+1][m])/(spacing*spacing);
              w33 = ((y2 - q0)*(q - x1)*Nw33pp[n][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw33pp[n+1][m])/(spacing*spacing);
            }
            w00 = ((y2 - q0)*(x2 - q)*Nw00pp[n][m])/(spacing*spacing) +  ((y2 - q0)*(q - x1)*Nw00pp[n][m+1])/(spacing*spacing) + ((q0 - y1)*(x2 - q)*Nw00pp[n+1][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw00pp[n+1][m+1])/(spacing*spacing);

            w03 = ((y2 - q0)*(x2 - q)*Nw03pp[n][m])/(spacing*spacing) +  ((y2 - q0)*(q - x1)*Nw03pp[n][m+1])/(spacing*spacing) + ((q0 - y1)*(x2 - q)*Nw03pp[n+1][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw03pp[n+1][m+1])/(spacing*spacing);

            w11 = ((y2 - q0)*(x2 - q)*Nw11pp[n][m])/(spacing*spacing) +  ((y2 - q0)*(q - x1)*Nw11pp[n][m+1])/(spacing*spacing) + ((q0 - y1)*(x2 - q)*Nw11pp[n+1][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw11pp[n+1][m+1])/(spacing*spacing);

            w12 = ((y2 - q0)*(x2 - q)*Nw12pp[n][m])/(spacing*spacing) +  ((y2 - q0)*(q - x1)*Nw12pp[n][m+1])/(spacing*spacing) + ((q0 - y1)*(x2 - q)*Nw12pp[n+1][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw12pp[n+1][m+1])/(spacing*spacing);

            w33 = ((y2 - q0)*(x2 - q)*Nw33pp[n][m])/(spacing*spacing) +  ((y2 - q0)*(q - x1)*Nw33pp[n][m+1])/(spacing*spacing) + ((q0 - y1)*(x2 - q)*Nw33pp[n+1][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw33pp[n+1][m+1])/(spacing*spacing);
            
            q0 += Bmec;
            break;

            case 2:
            if( q < spacing)
            {
              w00 = ((y2 - q0)*(q - x1)*Nw00np[n][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw00np[n+1][m])/(spacing*spacing);
              w03 = ((y2 - q0)*(q - x1)*Nw03np[n][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw03np[n+1][m])/(spacing*spacing);
              w11 = ((y2 - q0)*(q - x1)*Nw11np[n][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw11np[n+1][m])/(spacing*spacing);
              w12 = ((y2 - q0)*(q - x1)*Nw12np[n][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw12np[n+1][m])/(spacing*spacing);
              w33 = ((y2 - q0)*(q - x1)*Nw33np[n][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw33np[n+1][m])/(spacing*spacing);
            }

            w00 = ((y2 - q0)*(x2 - q)*Nw00np[n][m])/(spacing*spacing) +  ((y2 - q0)*(q - x1)*Nw00np[n][m+1])/(spacing*spacing) + ((q0 - y1)*(x2 - q)*Nw00np[n+1][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw00np[n+1][m+1])/(spacing*spacing);

            w03 = ((y2 - q0)*(x2 - q)*Nw03np[n][m])/(spacing*spacing) +  ((y2 - q0)*(q - x1)*Nw03np[n][m+1])/(spacing*spacing) + ((q0 - y1)*(x2 - q)*Nw03np[n+1][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw03np[n+1][m+1])/(spacing*spacing);

            w11 = ((y2 - q0)*(x2 - q)*Nw11np[n][m])/(spacing*spacing) +  ((y2 - q0)*(q - x1)*Nw11np[n][m+1])/(spacing*spacing) + ((q0 - y1)*(x2 - q)*Nw11np[n+1][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw11np[n+1][m+1])/(spacing*spacing);

            w12 = ((y2 - q0)*(x2 - q)*Nw12np[n][m])/(spacing*spacing) +  ((y2 - q0)*(q - x1)*Nw12np[n][m+1])/(spacing*spacing) + ((q0 - y1)*(x2 - q)*Nw12np[n+1][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw12np[n+1][m+1])/(spacing*spacing);

            w33 = ((y2 - q0)*(x2 - q)*Nw33np[n][m])/(spacing*spacing) +  ((y2 - q0)*(q - x1)*Nw33np[n][m+1])/(spacing*spacing) + ((q0 - y1)*(x2 - q)*Nw33np[n+1][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw33np[n+1][m+1])/(spacing*spacing);
            
            q0+= Bmec;
            break;

            case 3:
            if( q < spacing)
            {
              w00 = ((y2 - q0)*(q - x1)*Nw00pn[n][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw00pn[n+1][m])/(spacing*spacing);
              w03 = ((y2 - q0)*(q - x1)*Nw03pn[n][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw03pn[n+1][m])/(spacing*spacing);
              w11 = ((y2 - q0)*(q - x1)*Nw11pn[n][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw11pn[n+1][m])/(spacing*spacing);
              w12 = ((y2 - q0)*(q - x1)*Nw12pn[n][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw12pn[n+1][m])/(spacing*spacing);
              w33 = ((y2 - q0)*(q - x1)*Nw33pn[n][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw33pn[n+1][m])/(spacing*spacing);
            }

            w00 = ((y2 - q0)*(x2 - q)*Nw00pn[n][m])/(spacing*spacing) +  ((y2 - q0)*(q - x1)*Nw00pn[n][m+1] )/(spacing*spacing) + ((q0 - y1)*(x2 - q)*Nw00pn[n+1][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw00pn[n+1][m+1] )/(spacing*spacing);

            w03 = ((y2 - q0)*(x2 - q)*Nw03pn[n][m])/(spacing*spacing) +  ((y2 - q0)*(q - x1)*Nw03pn[n][m+1] )/(spacing*spacing) + ((q0 - y1)*(x2 - q)*Nw03pn[n+1][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw03pn[n+1][m+1] )/(spacing*spacing);
            
            w11 = ((y2 - q0)*(x2 - q)*Nw11pn[n][m])/(spacing*spacing) +  ((y2 - q0)*(q - x1)*Nw11pn[n][m+1] )/(spacing*spacing) + ((q0 - y1)*(x2 - q)*Nw11pn[n+1][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw11pn[n+1][m+1] )/(spacing*spacing);
            
            w12 = ((y2 - q0)*(x2 - q)*Nw12pn[n][m])/(spacing*spacing) +  ((y2 - q0)*(q - x1)*Nw12pn[n][m+1] )/(spacing*spacing) + ((q0 - y1)*(x2 - q)*Nw12pn[n+1][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw12pn[n+1][m+1] )/(spacing*spacing);
            
            w33 = ((y2 - q0)*(x2 - q)*Nw33pn[n][m])/(spacing*spacing) +  ((y2 - q0)*(q - x1)*Nw33pn[n][m+1] )/(spacing*spacing) + ((q0 - y1)*(x2 - q)*Nw33pn[n+1][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw33pn[n+1][m+1] )/(spacing*spacing);
            
            q0 += Bmec;
            break;

            case 4:
            
            if( q < spacing)
            {
              w00 = ((y2 - q0)*(q - x1)*Nw00_3p3h[n][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw00_3p3h[n+1][m])/(spacing*spacing);
              w03 = ((y2 - q0)*(q - x1)*Nw03_3p3h[n][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw03_3p3h[n+1][m])/(spacing*spacing);
              w11 = ((y2 - q0)*(q - x1)*Nw11_3p3h[n][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw11_3p3h[n+1][m])/(spacing*spacing);
              w12 = ((y2 - q0)*(q - x1)*Nw12_3p3h[n][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw12_3p3h[n+1][m])/(spacing*spacing);
              w33 = ((y2 - q0)*(q - x1)*Nw33_3p3h[n][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw33_3p3h[n+1][m])/(spacing*spacing);
            }
            w00 = ((y2 - q0)*(x2 - q)*Nw00_3p3h[n][m])/(spacing*spacing) +  ((y2 - q0)*(q - x1)*Nw00_3p3h[n][m+1])/(spacing*spacing) + ((q0 - y1)*(x2 - q)*Nw00_3p3h[n+1][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw00_3p3h[n+1][m+1])/(spacing*spacing);

            w03 = ((y2 - q0)*(x2 - q)*Nw03_3p3h[n][m])/(spacing*spacing) +  ((y2 - q0)*(q - x1)*Nw03_3p3h[n][m+1])/(spacing*spacing) + ((q0 - y1)*(x2 - q)*Nw03_3p3h[n+1][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw03_3p3h[n+1][m+1])/(spacing*spacing);

            w11 = ((y2 - q0)*(x2 - q)*Nw11_3p3h[n][m])/(spacing*spacing) +  ((y2 - q0)*(q - x1)*Nw11_3p3h[n][m+1])/(spacing*spacing) + ((q0 - y1)*(x2 - q)*Nw11_3p3h[n+1][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw11_3p3h[n+1][m+1])/(spacing*spacing);

            w12 = ((y2 - q0)*(x2 - q)*Nw12_3p3h[n][m])/(spacing*spacing) +  ((y2 - q0)*(q - x1)*Nw12_3p3h[n][m+1])/(spacing*spacing) + ((q0 - y1)*(x2 - q)*Nw12_3p3h[n+1][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw12_3p3h[n+1][m+1])/(spacing*spacing);

            w33 = ((y2 - q0)*(x2 - q)*Nw33_3p3h[n][m])/(spacing*spacing) +  ((y2 - q0)*(q - x1)*Nw33_3p3h[n][m+1])/(spacing*spacing) + ((q0 - y1)*(x2 - q)*Nw33_3p3h[n+1][m])/(spacing*spacing) + ((q0 - y1)*(q - x1)*Nw33_3p3h[n+1][m+1])/(spacing*spacing);
            break;

            default: std:cerr << "Wrong pair !! see mecevent_Nieves.cc Nieves_dsdEdc() function ... \n";
                     std::exit(1);

        }


            //------------------------------------ New Hadronic Tables----------------------------------// 
    }
    double w1=0.5*w11;
    double q02=q0*q0;
    double w2=0.5*(w00+w11+q02/vq2*(w33-w11)-2*q0/q*w03);
    double w3=(1-2*ap)*w12/q;
    double w4=0.5*(w33-w11)/vq2;
    double w5=(w03-q0/q*(w33-w11))/q;
    double sts=1.0-ct*ct;
    double st2=0.5*(1.0-ct);
    double ct2=0.5*(1.0+ct);
    result=(2*w1*st2+w2*ct2-w3*(E+Ep)*st2);
    result+=ml2/((Ep+lp)*Ep)*(w1*ct-0.5*w2*ct+0.5*w3*(Ep*(1.0-ct)+lp-E*ct)+0.5*w4*(ml2*ct+2*Ep*(Ep+lp)*sts)-0.5*w5*(Ep+lp));
    result*=2*G*G*Ep*lp/Pi/cm2;
    /*
    if (skalowanie == 1 && nucl == 0)//only for carbon phenomenological rescaling
    {
      result*=przeskalowanie(q0,q);
    }
    
    if (skalowanie == 2 && nucl <3)//only nuclei phenomenological rescaling
    {
      result*=przeskalowanie(q0,q);
    }
    */
    if (result<0) { result=0;}
  }
  return result;
}

void Generate_nucleon_dynamics_2p2h(particle *inc_nucleon_mec, particle *out_nucleon_mec, nucleus &T, double &xsec_result, vect qqq,double mec_central, double mec_smearing, double binding, int ile_PB, double sampling, int strength)
{
        vect suma(vec(0,0,0), 0);
        unsigned int licz = 0;
        //Pauli-NoPauli Blocking Version  <- Hemant Prasad 24/09/2023
        //-------------------------------------------------------------------------------------------------
        int pb_trials = 0;
        do 
        {
            ++pb_trials;
            do
            {
                inc_nucleon_mec[0] = T.get_nucleon();
                //vec N1 = spectral_choice(6, 6);
                //nucleon[0].set_momentum(N1);s

                vec pos (inc_nucleon_mec[0].r.x, inc_nucleon_mec[0].r.y, inc_nucleon_mec[0].r.z);
                inc_nucleon_mec[1] = T.get_nucleon(pos);
                //vec N2 = spectral_choice(6, 6);
                //nucleon[1].set_momentum(N2);

                inc_nucleon_mec[0].set_fermi( T.Ef(inc_nucleon_mec[0])  );

                suma = inc_nucleon_mec[0].p4() + inc_nucleon_mec[1].p4() + qqq;
                
                double bilans = inc_nucleon_mec[0].Ek() + inc_nucleon_mec[1].Ek()  - 2*(inc_nucleon_mec[0].his_fermi + 0.5*binding);

                if (bilans > 0)     //to avoid perpetuum mobile
                {
                  vect roznica (bilans, 0, 0, 0);
                  suma -= roznica;
                }

                licz++;
            }while ( ((suma.t < suma.length()) || (suma*suma < 4.0 * MN2_proton)) && licz < calls_max ); // Loop active till Hadronic Condition and Hadronic trials
            if((suma*suma > 4.0 *MN2_proton) && (suma.t > suma.length()) && (licz < calls_max)) // <-Hadronic Condition is checked here 
            {
              vec dir_cm;
              vec trans = suma.v();
              out_nucleon_mec[0].set_proton();
              out_nucleon_mec[1].set_proton();
                                          
              suma.boost2(trans);
              double Ecm = suma.t / 2.0;
              if(!PB)
              {
                if ((sampling > -0.01) && (sampling < 0.01))
                {
                  dir_cm = rand_dir() * sqrt(Ecm * Ecm - MN2_proton);
                }
                else
                {
                  dir_cm = rand_direc(trans, sampling, strength) * sqrt(Ecm * Ecm - MN2_proton);
                }
                out_nucleon_mec[0].set_momentum(dir_cm);
                out_nucleon_mec[1].set_momentum(-dir_cm);

                out_nucleon_mec[0].boost2(-trans);
                out_nucleon_mec[1].boost2(-trans);

                
              }
              else
              {
                double beta = trans.length();
                double gamma = 1.0/sqrt( 1 - beta*beta );
                double kappa = (gamma*Ecm - MN - inc_nucleon_mec[0].his_fermi)/beta/gamma/sqrt(Ecm * Ecm - MN2_proton);

                if(!( gamma*Ecm <= (MN_proton+inc_nucleon_mec[0].his_fermi) ))
                {
                  PB = false;
                  if ((sampling > -0.01) && (sampling < 0.01))
                  {
                    dir_cm = rand_dir3(trans, TMath::Min(kappa, 1.0) ) * sqrt(Ecm * Ecm - MN2_proton);
                  }
                  else
                  {
                    dir_cm = rand_direc3(trans, sampling, strength, TMath::Min(kappa, 1.0)) * sqrt(Ecm * Ecm - MN2_proton);
                  }

                  out_nucleon_mec[0].set_momentum(dir_cm);
                  out_nucleon_mec[1].set_momentum(-dir_cm);

                  out_nucleon_mec[0].boost2(-trans);
                  out_nucleon_mec[1].boost2(-trans);


                  
                }
                else
                {
                  //result = 0;
                  //vec zero = vec(0,0,0);
                  //nucleon[2].set_momentum(zero);
                  //nucleon[3].set_momentum(zero);
                  //nucleon[2].set_energy(MN);
                  //nucleon[3].set_energy(MN);
                  PB = true;
                }
              }
            }
            else
            {
              PB = false;
              xsec_result = 0;
              vec zero(0,0,0);
              out_nucleon_mec[0].set_momentum(zero);
              out_nucleon_mec[1].set_momentum(zero);
              out_nucleon_mec[0].set_energy(MN_proton);
              out_nucleon_mec[1].set_energy(MN_proton);
            }
        }while(PB && (pb_trials<=ile_PB));
        if(PB)                            //<- If the event checks all hadronic condition but no outgoing pair found within specific no. of pb_trials
        {
          xsec_result = 0;
          vec zero(0,0,0);
          out_nucleon_mec[0].set_momentum(zero);
          out_nucleon_mec[1].set_momentum(zero);
          out_nucleon_mec[0].set_energy(MN_proton);
          out_nucleon_mec[1].set_energy(MN_proton);
        }
        //-------------------------------------------------------------------------------------------------

}
void Generate_nucleon_dynamics_3p3h(particle *inc_nucleon_mec, particle *out_nucleon_mec, nucleus &T, double &xsec_result, vect qqq, int ile_PB)
{
  vect suma(vec(0,0,0), 0);
  unsigned int licz = 0;
  unsigned int pb_trials = 0;
  vec N1, N2, N3;
  vect N11, N22, N33;
  vec maksio_vec;

  double length1, length2, length3;
  double localfermi;
  double Winv;

  vec trans;

  //inc_nucleon_mec[0].set_neutron();
  //inc_nucleon_mec[1].set_proton();
  //inc_nucleon_mec[2].set_proton();
  
  double mass[3];
  double totalrestmass = 0; // total rest mass of the system -> mass[0] + mass[1] + mass[2];
  for(int mass_index = 0; mass_index < 3; mass_index++)
  {
    mass[mass_index] = inc_nucleon_mec[mass_index].mass();
    totalrestmass += mass[mass_index];
  }

  do 
  {
    particle probe1 = T.get_nucleon();
    vec pos (probe1.x, probe1.y, probe1.z);

    localfermi = T.localkf( probe1);

    for(int nucleon_index = 0; nucleon_index < 3; nucleon_index++)
    {
      out_nucleon_mec[nucleon_index].r.x = probe1.r.x;
      out_nucleon_mec[nucleon_index].r.y = probe1.r.y;
      out_nucleon_mec[nucleon_index].r.z = probe1.r.z;
    }
    
    N1 = rand_from_ball (localfermi);
    N2 = rand_from_ball (localfermi);
    N3 = rand_from_ball (localfermi);

    inc_nucleon_mec[0].set_momentum(N1);
    inc_nucleon_mec[1].set_momentum(N2);
    inc_nucleon_mec[2].set_momentum(N3);
    
    length1 = sqrt(MN2 + N1.norm2());
    length2 = sqrt(MN2 + N2.norm2());
    length3 = sqrt(MN2 + N3.norm2());

    N11 = vect(N1, length1);
    N22 = vect(N2, length2);
    N33 = vect(N3, length3);

    suma = N11 + N22 + N33 + qqq;

    licz++;
  }while(  ( (suma.t < suma.length()) || (suma * suma < totalrestmass*totalrestmass ) ) && (licz < calls_max) ) ;

  if(  ( (suma.t > suma.length()) && (suma * suma >= totalrestmass*totalrestmass ) )  && (licz < calls_max))
  {
    //std::cerr << suma * suma << "\t" << totalrestmass << "\n";
    //out_nucleon_mec[0].set_proton();
    //out_nucleon_mec[1].set_proton();
    //out_nucleon_mec[2].set_proton();

    trans = suma.v();
    suma.boost2(trans);
    Winv = suma.t;

    vect nuc11, nuc22, nuc33;
    //threebodydecay(Winv, mass[0], mass[1], mass[2], nuc11, nuc22, nuc33);

    if(PB)
    {
      while(PB && pb_trials < ile_PB)
      {
        threebodydecay(Winv, mass[0], mass[1], mass[2], nuc11, nuc22, nuc33);
        if( (nuc11.length() >= localfermi) && (nuc22.length() >= localfermi ) && ( nuc33.length() >= localfermi ) )
        {
          PB = false;
          break;
        }
        pb_trials++;
      }
    }
    else 
    {
      threebodydecay(Winv, mass[0], mass[1], mass[2], nuc11, nuc22, nuc33);
    }

    nuc11.boost2(-trans);
    nuc22.boost2(-trans);
    nuc33.boost2(-trans);

    out_nucleon_mec[0].set_momentum(vec(nuc11));
    out_nucleon_mec[1].set_momentum(vec(nuc22));
    out_nucleon_mec[2].set_momentum(vec(nuc33));
    
    
  }
  else if ((licz >= calls_max) || (pb_trials >= ile_PB))
  {
    xsec_result = 0;
    vec zero(0,0,0);
    for(int nucleon_index = 0; nucleon_index < 3; nucleon_index++)
    {
      out_nucleon_mec[nucleon_index].set_momentum(zero);
    }
  }
  
}

void threebodydecay (double W, double m1, double m2, double m3, vect &p1, vect &p2, vect &p3)
{
	if (W < m1+m2+m3)
	{
          std::cerr << "impossible kinematics in 3-body decay" << "\n";

	}

	double W_2 = W*W;
	double m3_2 = m3*m3;
	double m1_2 = m1*m1;
	double m2_2 = m2*m2;

	double m23_2_los, m23_2_min, m23_2_max, E3;

	do
	{
		double m12_2_los = (m1+m2)*(m1+m2) + frandom()* ( (W-m3)*(W-m3) - (m1+m2)*(m1+m2) );
		double m23_test = (m2+m3)*(m2+m3) + frandom()* ( (W-m1)*(W-m1) - (m2+m3)*(m2+m3) );
		m23_2_los = m23_test;

		E3 = (W_2 + m3_2 - m12_2_los)/2.0/W;

		double E2star = (m12_2_los - m1_2 + m2_2)/2.0/sqrt(m12_2_los);
		double E3star = (W_2 - m12_2_los - m3_2)/2.0/sqrt(m12_2_los);

		m23_2_min = (E2star+E3star)*(E2star+E3star)
		- ( sqrt(E2star*E2star - m2_2) + sqrt (E3star*E3star - m3_2) )*
		( sqrt(E2star*E2star - m2_2) + sqrt (E3star*E3star - m3_2) );

		m23_2_max = (E2star+E3star)*(E2star+E3star)
		- ( sqrt(E2star*E2star - m2_2) - sqrt (E3star*E3star - m3_2) )*
		( sqrt(E2star*E2star - m2_2) - sqrt (E3star*E3star - m3_2) );

	}
	while ( (m23_2_los < m23_2_min) || (m23_2_los > m23_2_max) );

	//double m23_2_los = m23_2_min + los()* ( m23_2_max - m23_2_min);

	double E1 = (W_2 + m1_2 - m23_2_los)/2.0/W;
	double E2 = W - E1 - E3;

	double mom1 = sqrt (E1*E1 - m1_2);
	double mom2 = sqrt (E2*E2 - m2_2);
	double mom3 = sqrt (E3*E3 - m3_2);

	vec axis = rand_dir (); // new -- Y axis
	vec proba = rand_dir ();

	vec kier1 = vecprod (axis, proba);
	kier1 = kier1/kier1.length();//unit vector -- new Z axis!

	// unit vetor -- new X axis
	vec kier2 = vecprod (axis, kier1);

	double kosalpha = (mom1*mom1 + mom2*mom2 - mom3*mom3)/2.0/mom1/mom2;
	double sinalpha = sqrt(1-kosalpha*kosalpha);
	double kosbeta = ( mom1 - mom2*kosalpha )/mom3;
	double sinbeta = sqrt(1 - kosbeta*kosbeta);

	vec pp1 = kier1*mom1;
	vec pp2 = mom2*(-kosalpha*kier1 + sinalpha*kier2);
	vec pp3 = mom3*(-kosbeta*kier1 - sinbeta*kier2);

	p1 = vect(pp1,E1);
	p2 = vect(pp2,E2);
	p3 = vect(pp3,E3);

}
