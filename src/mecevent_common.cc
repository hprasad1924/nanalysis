#include "mecevent_common.h"

//int noev=0;
double ml=0;
double ml2=0;
double width_q0=0;
double Bmec=0;
int ap=0;
int nucl=0;
bool PB=0;

////////////////////////////////////////

void mec_do_cc (particle *p, double ratio_pp, double ratio_np)
{
  // here is the isospin model; I assume that 3/5 times a pair is p-p and 2/5 times it is p-n
  // KM: I don't understand the comment above
  if (frandom () < ratio_pp)
  {
    if(ap)
    {
      p[0].set_proton();
      p[1].set_neutron();
      p[2].set_neutron();
      p[3].set_neutron();
    }
    else
    {
      p[0].set_neutron();
      p[1].set_proton();
      p[2].set_proton();
      p[3].set_proton();
    }
  }
  else
  {
    if(ap)
    {
      p[0].set_proton();
      p[1].set_proton();
      p[2].set_neutron();
      p[3].set_proton ();
    }
    else
    {
      if (frandom () < ratio_np)
      {
            p[0].set_neutron();
            p[1].set_neutron();
            p[2].set_neutron();
            p[3].set_proton();

            //Swaping the higher momentum to neutron in np events
            vec mom = p[2].p();
            if( !(p[2].momentum() >= p[3].momentum()) )
            {
                p[2].set_momentum(p[3].p());
                p[3].set_momentum(mom);
            }
      }
      else
      {
            p[0].set_neutron();
            p[1].set_neutron();
            p[2].set_proton();
            p[3].set_neutron();

            //Swaping the higher momentum to proton in pn events
            vec mom = p[2].p();
            if( !(p[2].momentum() >= p[3].momentum()) )
            {
                p[2].set_momentum(p[3].p());
                p[3].set_momentum(mom);
            }

      }
    }
  }
}

void mec_do_cc_Nieves (particle *in_p, particle *out_p, double ratio_pp, double ratio_np)
{
  // here is the isospin model; I assume that 3/5 times a pair is p-p and 2/5 times it is p-n
  // KM: I don't understand the comment above
  if (frandom () < ratio_pp)
  {
    if(ap)
    {
      in_p[0].set_proton();
      in_p[1].set_neutron();
      out_p[0].set_neutron();
      out_p[1].set_neutron();
    }
    else
    {
      in_p[0].set_neutron();
      in_p[1].set_proton();
      out_p[0].set_proton();
      out_p[1].set_proton();
    }
  }
  else
  {
    if(ap)
    {
      in_p[0].set_proton();
      in_p[1].set_proton();
      out_p[0].set_neutron();
      out_p[1].set_proton();
    }
    else
    {
      if (frandom () < ratio_np)
      {
            in_p[0].set_neutron();
            in_p[1].set_neutron();
            out_p[0].set_neutron();
            out_p[1].set_proton();

            //Swaping the higher momentum to neutron in np events
            vec mom = out_p[0].p();
            if( !(out_p[0].momentum() >= out_p[1].momentum()) )
            {
                out_p[0].set_momentum(out_p[1].p());
                out_p[1].set_momentum(mom);
            }
      }
      else
      {
            in_p[0].set_neutron();
            in_p[1].set_neutron();
            out_p[0].set_proton();
            out_p[1].set_neutron();
            //Swaping the higher momentum to proton in pn events
            vec mom = out_p[0].p();
            if( !(out_p[0].momentum() >= out_p[1].momentum()) )
            {
                out_p[0].set_momentum(out_p[1].p());
                out_p[1].set_momentum(mom);
            }

      }
    }
  }
}

void mec_do_cc_Nieves_3p3h(particle *in_p, particle *out_p)
{
  /* When grid files are present for multiple nuclei then this can be uncommented
  switch(nucl)
  {
    case 0: Z = 6;
            A = 12;
            break;
    case 1: Z = 8;
            A = 16;
            break;
  }
  */
  
  double ratio_npp = 0.409, ratio_nnp = 0.409, ratio_nnn = 0.0901, ratio_ppp = 0.0901;
  ratio_npp = ratio_npp / (ratio_npp + ratio_nnp + ratio_nnn);
  ratio_nnp = ratio_nnp / (ratio_nnp + ratio_nnn);

  if(frandom() < ratio_npp)
  {
    in_p[0].set_neutron();
    in_p[1].set_proton();
    in_p[2].set_proton();

    for(int i = 0; i < 3; i++)
    {
      out_p[i].set_proton();
    }
  }
  else {
    if( frandom() < ratio_nnp )
    {
      in_p[1].set_neutron();
      in_p[2].set_neutron();
      in_p[3].set_proton();

      out_p[0].set_proton();
      out_p[1].set_proton();
      out_p[1].set_proton();
    }
    else 
    {
      in_p[1].set_neutron();
      in_p[2].set_neutron();
      in_p[3].set_neutron();

      out_p[1].set_proton();
      out_p[1].set_neutron();
      out_p[1].set_neutron();
    }
  }
  

}

void mec_decide_3p3h_2p2h(double ratio_2p2h, bool& flag_2p2h)
{
  if(frandom() < ratio_2p2h)
  {
    flag_2p2h = true;
  }
  else 
  {
    flag_2p2h = false;
  }

}
