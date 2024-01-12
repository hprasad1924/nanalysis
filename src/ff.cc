#include "ff.h"
#include <cmath>
#include <iostream>
#include <utility>
#include "jednostki.h"
#include "params.h"
#include "pdg.h"
#include "rew/rewparams.h"

using namespace std;

double inline pow2(double x) { return x * x; }

static const double M2 = pow2(0.5 * (PDG::mass_proton + PDG::mass_neutron));

//________________________________________________________
/// Form Factor Parameters
static const double MV2 = 0.71 * GeV2;
static double MA_cc = 1030 * MeV;
static double MA_nc = 1030 * MeV;
static const double MA_cc_mec =
    1014 * MeV;  // axial mass for MEC is fixed in TEM model
static const double MA_nc_mec = 1014 * MeV;
static double MA_s = 1030 * MeV;
static double MA_hyp = 1030 * MeV; // C Thorpe Added Hyperon axial mass parameter

static const double Dipole_Lambda = 5.6; //C Thorpe: Lambda parameter used in dipole form factors
                                   // default 1030 MeV // March 2019
static double delta_s = 0;
static const double mu_p = 2.793;   // moment dipolowy protonu
static const double mu_n = -1.913;  // moment dipolowy neutronu
static const double piMass2 = pow2(PDG::mass_pi);
static const double gA = -1.2673;

static int axialFFset = 0;
static const int strangeFFset = 0;
static int strange = 0;

//Added by C Thorpe Dec 2018
//Constants associated with SU(3) representation of axial currents
static const double Axial_F = 0.463;
static const double Axial_D = 0.804;
static const double Axial_x = Axial_F/(Axial_F+Axial_D);
//Second class current setup
//real and imaginary components at Q2=0
static double Rg20 = 0;
static double Ig20 = 0;
//symmetry breaking setup
static bool sym_break = false;

static double p_AEp[7] = {1., 0.9927, 0.9898, 0.9975, 0.9812, 0.9340, 1.};
static double p_AMp[7] = {1., 1.0011, 0.9992, 0.9974, 1.0010, 1.0003, 1.};
static double p_AEn[7] = {1., 1.1011, 1.1392, 1.0203, 1.1093, 1.5429, 0.9706};
static double p_AMn[7] = {1., 0.9958, 0.9877, 1.0193, 1.0350, 0.9164, 0.7300};
static double p_AAx[7] = {1., 0.9958, 0.9877, 1.0193, 1.0350, 0.9164, 0.7300};

// 2 and 3 Component Form Factor parameters
static double axial_ff_beta = 0.0;
static double axial_ff_theta = 0.0;
static double axial_ff_gamma = 0.0;
static double axial_ff_alpha = 0.0;

// strange =0 nie strange FF
// strange =1 old implementation (recover old bahaviour)
// strange =2 new implementation (uses strange axial mass != nc axial mass)

static int strangeEM = 0;

struct FF {
  double Q2;
  double GEp, GEn;
  double GMp, GMn;

  FF() : Q2(0), GEp(0), GEn(0), GMp(0), GMn(0) {}

  inline pair<double, double> f12(int kind);
};

//_________________________________________________________
/// Pointer to current vector form factors model
static FF (*FFfromq2)(const double) = 0;


//_________________________________________________________
/// Pointer to current axial form factors model
static double (*Axialfromq2)(const double, const double) = 0;


// _________________________________________________________
double axialcorr(int axialFF, double q2);

/*
/// strange form factors /??
struct FFs
{
        double F1s;
        double F2s;
        double FAs;
};

typedef FFs (*FFspointer)(const double);


/// strange correction

FFs strange_cor(double q2)//,double & f1,double & f2,double & fa)
{
        double F1s_0 = 0.53;
        double F2s_0 = -0.40;
//? double delta_s = -0.21;
        double Ma2=MA_s*MA_s;
    double tau=-q2/M2;
    double mian1=pow2(1-q2/MV2);
    FFs f;
        f.F1s= F1s_0/(1+tau)/mian1;
        f.F2s= F2s_0/(1+tau)/mian1;
        f.FAs= delta_s/mian1;
        return f;
}
*/

/// Functions calculating form factors
FF DipoleFF(const double q2);   // 1. dipole electric form factor G_E^V
FF bba03_FF(const double q2);   // 2. hep-ex/0308005 BBA-2003 for Q2<6 GeV
FF bbba05_FF(const double q2);  // 3. arXiv:hep-ex/0602017 BBBA05 for Q2<18 GeV
FF bbba07_FF(const double q2);
double GetA(const double q2, const double* x);
FF JLab_FF(const double q2);  // 4. PHYSICAL REVIEW C, VOLUME 65, 051001(R)
                              // PHYSICAL REVIEW C, VOLUME 51, 409 (1995)
FF kg_FF(const double q2);    // 5. K. Graczyk ...
FF npar_FF(const double q2);  // 6. nowa (1990:) parametryzacja JS z qelcc

/// FA Functions
double dipole_FA(const double q2, const double ma); // Standard dipole
double bbba07_FA(const double q2, const double ma); // BBBA07 Form Factors
double comp2_FA(const double q2, const double ma); // 2 Component Model
double comp3_FA(const double q2, const double ma); // 3 Component Model
double zexp_FA(const double q2, const double ma); // Z-expansion Model   

// double axialcorr(int axialFF,double q2);
////////////////////////////////////////////////////////////////////////
//                          IMPLEMENTATION
////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
/// Calculate F1,F2
pair<double, double> FF::f12(int kind) {

  double Ge = 0, Gm = 0, f1 = 0, f2 = 0, F1s = 0, F2s = 0;
  const double tau = Q2 / (4 * M2);

  //C Thorpe added Dec 2018
  //needed by hyperon production channels
  double f1p=0,f1n=0,f2p=0,f2n=0;

  switch (kind) {
    case 0:
    case 6:  // cc and mec qel part
      Ge = GEp - GEn;
      Gm = GMp - GMn;
      break;
    case 1:
    case 7:  // nc proton and mec qel part
      Ge = 0.5 * (GEp - GEn) - 2 * sin2thetaW * GEp;  //-0.5*GEs;
      Gm = 0.5 * (GMp - GMn) - 2 * sin2thetaW * GMp;  //-0.5*GMs;
      break;
    case 2:
    case 8:  // nc neutron and mec qel part
      Ge = 0.5 * (GEn - GEp) - 2 * sin2thetaW * GEn;  //-0.5*GEs;
      Gm = 0.5 * (GMn - GMp) - 2 * sin2thetaW * GMn;  //-0.5*GMs;
      break;
    case 3:  // cc mec
      Ge = GEp - GEn;
      Gm = sqrt(1 + 6.0 * Q2 / 1e6 * exp(-Q2 / 0.35 / 1e6)) * (GMp - GMn);
      break;
    case 4:                                           // nc proton mec
      Ge = 0.5 * (GEp - GEn) - 2 * sin2thetaW * GEp;  //-0.5*GEs;
      Gm =
          sqrt(1 + 6.0 * Q2 / 1e6 * exp(-Q2 / 0.35 / 1e6)) * 0.5 * (GMp - GMn) -
          2 * sin2thetaW * GMp;  //-0.5*GMs;
      break;
    case 5:                                           // nc neutron mec
      Ge = 0.5 * (GEn - GEp) - 2 * sin2thetaW * GEn;  //-0.5*GEs;
      Gm = sqrt(1 + 6.0 * Q2 / 1e6 * exp(-Q2 / 0.35 / 1e6)) *
           (0.5 * (GMn - GMp) - 2 * sin2thetaW * GMn);  //-0.5*GMs;
      break;
    case 10: //elastic ep scattering
        Ge=GEp;
        Gm=GMp;
        break;
    case 11: //elastic en scattering
        Ge=GEn;
        Gm=GMn;
        break;
    //C Thorpe added Dec 2018
    //hyperon channels: 12,13,14
    case 12: 
    case 13:
    case 14: 
      f1p = (1/(1+tau))*(GEp + tau*GMp);
      f1n = (1/(1+tau))*(GEn + tau*GMn);
      f2p = (1/(1+tau))*(GMp - GEp);   
      f2n = (1/(1+tau))*(GMn - GEn);
      break;
  }

  f1 = (Ge + tau * Gm) / (1 + tau);
  f2 = (Gm - Ge) / (1 + tau);

  if ((kind == 1 or kind == 2) and
      strangeEM)  // strangeness in F1, F2 (only for kind!=0 i.e. nc)
    switch (strangeEM) {
      case 1: {
        double mian = (1 + tau) * pow2(1 + Q2 / MV2);
        f1 -= 0.5 * (0.53 / mian);
        f2 -= 0.5 * (-0.40 / mian);
        break;
      }
      case 2:  // more complex version from KG
      {
        double mian = (1 + tau) * pow2(1 + Q2 / MV2);
        ;
        double mus = -0.39;
        double f1s = 0.49;

        f1 -= 0.5 * mus / mian;
        f2 -= 0.5 * f1s * Q2 / GeV2 / mian;
        break;
      }
      default:
        break;  // no strange correction
    }

  //added c thorpe dec 2018
  //hyperon channels
  //Lambda zero
  if(kind == 12){
    f1 = (-1)*pow(1.5,0.5)*f1p;
    f2 = (-1)*pow(1.5,0.5)*f2p;
    //SU(3) symmetry breaking correction
    if(sym_break == true)
    {
      f1 *= 0.976;
    }
  }
  //Sigma zero
  if(kind == 13){
    f1 = (-1)*(f1p+2*f1n)/(pow(2,0.5));
    f2 = (-1)*(f2p+2*f2n)/(pow(2,0.5));
    //SU(3) symmetry breaking correction
    if(sym_break == true)
    {
      f1 *= 0.975;
    }
  }
  //Sigma minus
  if(kind==14){
    f1 = (-1)*(f1p+2*f1n);
    f2 = (-1)*(f2p+2*f2n);
    if(sym_break == true)
    {
      f1 *= 0.975;
    }
  }

  return pair<double, double>(f1, f2);
}

/////////////////////////////////////////////////////////////
FF DipoleFF(const double q2)  // dipole electric form factor G_E^V
{
  double a = 1.0 - q2 / MV2;
  double a2 = a * a;
  double tau = -q2 / (4 * M2);

  FF ff;
  ff.Q2 = -q2;
  ff.GEp = 1.0 / a2;
  //  ff.GEn = 0;
  //C Thorpe: Updated dipole FFs
  ff.GEn = (-1)*mu_n*tau/(1+Dipole_Lambda*tau)/a2;
  ff.GMp = mu_p / a2;
  ff.GMn = mu_n / a2;

  return ff;
};

/////////////////////////////////////////////////////////////
FF bba03_FF(const double q2) {
  const double Q2 = -q2 / GeV2;       // Q2 in GeV2 
  const double tau = -q2 / (4 * M2);  // must be dimensionless

  FF ff;
  ff.Q2 = -q2;
  // hep-ex/0308005 BBA-2003 for Q2<6 GeV2
  ff.GEp =
      1.0 /
      (1.0 +
       Q2 * (3.253 +
             Q2 * (1.422 +
                   Q2 * (0.08582 +
                         Q2 * (0.3318 + Q2 * (-0.09371 + Q2 * 0.01076))))));
  ff.GEn = -mu_n * 0.942 * tau / (1 + 4.61 * tau) / pow2(1 - q2 / MV2);
  ff.GMp =
      mu_p / (1.0 +
              Q2 * (3.104 +
                    Q2 * (1.428 +
                          Q2 * (0.1112 +
                                Q2 * (-0.006981 +
                                      Q2 * (0.0003705 + Q2 * -0.7063e-5))))));
  ff.GMn =
      mu_n /
      (1.0 +
       Q2 * (3.043 +
             Q2 * (0.8548 + Q2 * (0.6806 + Q2 * (-0.1287 + Q2 * 0.008912)))));

  return ff;
}

///////////////////////////////////////////////////////////////
FF bbba05_FForig(const double q2) {
  const double tau = -q2 / (4 * M2);

  FF ff;
  ff.Q2 = -q2;

  // arXiv:hep-ex/0602017 BBBA05 for Q2<18 GeV
  ff.GEp =
      (1.0 - tau * 0.0578) / (1.0 + tau * (11.1 + tau * (13.6 + tau * 33.0)));
  ff.GEn = tau * (1.25 + tau * 1.30) /
           (1.0 + tau * (-9.86 + tau * (305.0 + tau * (-758.0 + tau * 802.0))));
  ff.GMp = mu_p * (1.0 + tau * 0.15) /
           (1.0 + tau * (11.1 + tau * (19.6 + tau * 7.54)));
  ff.GMn = mu_n * (1.0 + tau * 1.81) /
           (1.0 + tau * (14.1 + tau * (20.7 + tau * 68.7)));
  /*
  double Gep=(1-0.0577*tau)/(1+11.2*tau+13.6*tau2+33*tau3);
  double Gen=(1.38*tau-0.214*tau2)/(1+8.51*tau+59.9*tau2+13.6*tau3+2.57*tau4);
  double Gmp=2.79*(1+0.15*tau)/(1+11.1*tau+19.6*tau2+7.54*tau3);
  double Gmn=-1.91*(1+1.82*tau)/(1+14.1*tau+20.7*tau2+69.7*tau3);
          */
  return ff;
}

FF bbba05_FF(const double q2) {


  FF ff;
  ff.Q2 = -q2;
  double Q2 = -q2;
  double tau = -q2 / 4.0 / M2;
  ff.GEp = (1.0 - tau * 0.0578) /
           (1.0 + tau * (11.1 + tau * (13.6 + tau * 33.0)));  // wersja Artura
  ff.GEn =
      tau * (1.25 + tau * 1.30) /
      (1.0 +
       tau * (-9.86 +
              tau * (305.0 + tau * (-758.0 + tau * 802.0))));  // wersja Artura
  ff.GMp = 2.792847351 * (1.0 + tau * 0.15) /
           (1.0 + tau * (11.1 + tau * (19.6 + tau * 7.54)));
  ff.GMn = -1.91304273 * (1.0 + tau * 1.81) /
           (1.0 + tau * (14.1 + tau * (20.7 + tau * 68.7)));
  return ff;
}

double GetA(const double q2, const double* x) {
  double eps_int[7] = {0., 1. / 6., 1. / 3., 1. / 2., 2. / 3., 5. / 6., 1.};
  double Q2 = -q2;
  double tau = -q2 / 4.0 / M2;
  double eps = 1.0 / (1 + sqrt(1 + (1. / tau)));
  double A = 0.0;

  // Sum up A
  for (int j = 0; j < 7; j++) {
    double mod = x[j];
    for (int k = 0; k < 7; k++) {
      if (k == j) continue;
      mod *= (eps - eps_int[k]) / (eps_int[j] - eps_int[k]);
    }
    A += mod;
  }

  return A;
};

FF bbba07_FF(const double q2) {
  FF ff;
  ff.Q2 = -q2;

  // Set Variables
  double Q2 = -q2;
  double tau = -q2 / 4.0 / M2;
  double eps = 2.0 / (1 + sqrt(1 + (1. / tau)));

  // Calculate Lagrange
  double AEp = GetA(q2, p_AEp);
  double GK_Ep =
      (1. - 0.24 * tau) / (1.0 + tau * (10.98 + tau * (12.82 + tau * (21.97))));

  double AMp = GetA(q2, p_AMp);
  double GK_Mp = (1. + 0.1717 * tau) /
                 (1.0 + tau * (11.26 + tau * (19.32 + tau * (8.33))));

  double AEn = GetA(q2, p_AEn);
  //  double Ep = 0.0;

  double AMn = GetA(q2, p_AMn);
  //  double Mp = 0.0;

  // Define p form factors
  ff.GEp = AEp * GK_Ep;
  ff.GMp = AMp * GK_Mp * mu_p;

  // Define n form factors
  // a = 1.7
  // b = 3.3
  ff.GEn = AEn * ff.GEp * ((1.7 * tau) / (1 + 3.3 * tau));
  ff.GMn = AMn * ff.GMp * mu_n / mu_p;

  return ff;
};

/////////////////////////////////////////////////////////////
FF JLab_FF(const double q2) {
  const double Q2 = -q2 / GeV2;
  const double Q = sqrt(Q2);
  const double tau = -q2 / 4 / M2;

  FF ff;
  ff.Q2 = -q2;
  // PHYSICAL REVIEW C, VOLUME 65, 051001(R)
  ff.GEp = (1.0 - 0.13 * (Q2 - 0.04)) /
           (1.0 + (0.116 + (0.241 + 0.345 * Q2) * Q2) * Q +
            (2.874 + 1.006 * Q2) * Q2);
  ff.GMp = mu_p / (1.0 + (0.116 + (0.241 + 0.345 * Q2) * Q2) * Q +
                   (2.874 + 1.006 * Q2) * Q2);

  // PHYSICAL REVIEW C, VOLUME 51, 409 (1995)
  ff.GEn = -1.25 * mu_n * tau / (1 + 18.3 * tau) / (1 + Q2 * 1.0e+6 / MV2) /
           (1 + Q2 * 1.0e+6 / MV2);
  ff.GMn = mu_n / (1.0 - (1.74 + 7.63 * Q2) * Q + (9.29 + 4.63 * Q2) * Q2);

  return ff;
};

/////////////////////////////////////////////////////////////
FF npar_FF(const double q2) {
  const double Q2 = -q2 / GeV2;
  const double Q = sqrt(Q2);
  const double tau = -q2 / 4 / M2;

  FF ff;
  ff.Q2 = -q2;

  double mu_p = 2.793;   // moment dipolowy protonu
  double mu_n = -1.913;  // moment dipolowy neutronu

  ff.GEp = 1 / (1 + Q * (0.62 + Q * (0.68 + Q * (2.80 + Q * 0.83))));
  ff.GMp =
      mu_p / (1 + Q * (0.35 + Q * (2.44 + Q * (0.50 + Q * (1.04 + Q * 0.34)))));
  ff.GMn = mu_n / (1 + Q * (-0.74 + Q * (9.29 + Q * (-7.63 + Q * 4.63))));

  const double a = 1.25;
  const double b = 10.4;
  double GD = 1 / pow2(1 - q2 / (0.71 * GeV2));

  ff.GEn = -a * mu_n * tau * GD / (1 + b * tau);

  bool strange = false;  // comment out next 2 lines
  if (strange) ff.GEp = ff.GMp / mu_p * (1 - 0.14 * (-q2 / GeV2 - 0.3));

  return ff;
};

/*
/////////////////////////////////////////////////////////////
double newFA(const double q2, double MA) //axial form factor
{
        //double MA=p.sf_axial_mass;
        double MA2=MA*MA;
        double a = 1.0/(1.0 - q2/MA2);
        return gA*a*a;
}

/////////////////////////////////////////////////////////////
double newFP(const double q2,double MA) //pseudoscalar form factor
{
        return 2.0*M2*newFA(q2,MA)/(piMass2 - q2);
}
*/

/////////////////////////////////////////////////////////////
static double funkcja1(double Q2, double* w) {
  double q2 = Q2 / GeV / GeV;

  return w[4] / (1 + exp(-(q2 * w[0] + w[1]))) +
         w[5] / (1 + exp(-(q2 * w[2] + w[3]))) + w[6];
}

/////////////////////////////////////////////////////////////
static double funkcja2(double Q2, double* w) {
  double q2 = Q2 / GeV / GeV;

  return w[6] / (1 + exp(-(q2 * w[0] + w[1]))) +
         w[7] / (1 + exp(-(q2 * w[2] + w[3]))) +
         w[8] / (1 + exp(-(q2 * w[4] + w[5]))) + w[9];
}

/////////////////////////////////////////////////////////////
FF kg_FF(const double q2) {
  static double tab_gen[7] = {10.19704,  2.36812,  -1.144266, -4.274101,
                              0.8149924, 2.985524, -0.7864434};
  static double tab_gmn[10] = {3.19646,    2.565681, 6.441526,  -2.004055,
                               -0.2972361, 3.606737, -3.135199, 0.299523,
                               1.261638,   2.64747};
  static double tab_gep[10] = {3.930227,   0.1108384, -5.325479, -2.846154,
                               -0.2071328, 0.8742101, 0.4283194, 2.568322,
                               2.577635,   -1.185632};
  static double tab_gmp[10] = {-2.862682,  -1.560675, 2.321148, 0.1283189,
                               -0.2803566, 2.794296,  1.726774, 0.861083,
                               0.4184286,  -0.1526676};
  static double tab_axial[10] = {-26.10885, 1.823041,  -8.391283, -7.737312,
                                 15.27646,  0.3992788, -1.350184, -0.2021121,
                                 -2.870517, 3.879841};

  static const double magneton_N_nnff = 1;  // tego nie jestem pewien !!!!
  static const double magneton_proton_nnff = 2.793 * magneton_N_nnff;
  static const double magneton_neutron_nnff = -1.913 * magneton_N_nnff;

  static double gA_nnff = -1.267;
  static double MA_nnff = 1.015 * GeV;  // 1.03*GeV;

  double Q2 = -q2;
  double y = pow2(1.0 + Q2 / 0.71 / GeV / GeV);

  FF ff;
  ff.Q2 = Q2;
  ff.GEp = funkcja2(Q2, tab_gep) / y;
  ff.GEn = funkcja1(Q2, tab_gen);
  ff.GMp = funkcja2(Q2, tab_gmp) * magneton_proton_nnff / y;
  ff.GMn = funkcja2(Q2, tab_gmn) * magneton_neutron_nnff / y;
  return ff;
}

///////////////////////////////////////////////////////////////
// Calculate vector form factors
pair<double, double> f12(double q2, int kind) {
  //  std::cout<<"F12"<<std::endl;
  FF ff = FFfromq2(q2);
  ff.Q2 = -q2;
  //  cout<<ff.GEp<< ' '<<ff.GMp<<' '<<ff.GEn<<' '<<ff.GMn<<endl;
  return ff.f12(kind);
}


///////////////////////////////////////////////////////////////
// AXIAL FORM FACTOR FUNCTIONS

//____________________________________
/// Standard Dipole
double dipole_FA(double q2, double ma){
  return -1.267 / pow2(1 - q2 / ma / ma);
}

//____________________________________
/// BBBA07 Dipole
double bbba07_FA(double q2, double ma){
  
  double FA = dipole_FA(q2, ma);
  double AAx = GetA(q2, p_AAx);
  return FA* AAx;
  
}


//____________________________________
/// 2 Component Model
double comp2_FA(double q2, double ma){
  (void) ma; // MA Ignored for this function

  double ma_axl = 1.230;
  double gterm = 1.0 / pow2(1.0 + axial_ff_gamma * q2);
  double aterm = (1.0 - axial_ff_alpha + \
		  (axial_ff_alpha * (ma_axl * ma_axl) / (ma_axl*ma_axl + q2)));
  
  return -1.267 * gterm * aterm;
}


//____________________________________
/// 3 Component model
double comp3_FA(double q2, double ma){
  (void) ma; // MA Ignored for this function

  double comp2_term = comp2_FA(q2, ma);
  double exp_term = - 1.267 * sqrt( axial_ff_theta) * axial_ff_beta * \
    exp(axial_ff_theta + axial_ff_beta * q2);
  
  return comp2_term + exp_term;
}



//____________________________________
/// Z Expansion Model

static const int kmax = 15;
static double zexp_aterms[15];
static int zexp_nterms;
static bool zexp_q4limit;
static double zexp_tc;
static double zexp_t0;

double zexp_GetZ(double q2){

  // T Cut
  double num = sqrt(zexp_tc - q2) - sqrt(zexp_tc - zexp_t0);
  double den = sqrt(zexp_tc - q2) + sqrt(zexp_tc - zexp_t0);

  return num/den;
}

void PrintZExpTerms(bool showFA){

  cout << " ZEXP State! " << endl;
  cout << " ------------------" << endl;
  cout << " T0 = " << zexp_t0 << endl;
  cout << " TC = " << zexp_tc << endl;

  int ncount = zexp_nterms;
  if (zexp_q4limit > 0) ncount += 4;
  for (int i = 0; i <= ncount; i++){
    cout << "ZEXP A" << i << " = " << zexp_aterms[i] << endl;
  }

  if (showFA){
    cout << " FA Values " << endl;
    cout << " FAZ(0.00) = " << zexp_FA(0.00,0.0) << endl;
    cout << " FAZ(0.25) = " << zexp_FA(0.25,0.0) << endl;
    cout << " FAZ(0.50) = " << zexp_FA(0.50,0.0) << endl;
    cout << " FAZ(0.75) = " << zexp_FA(0.75,0.0) << endl;
    cout << " FAZ(1.00) = " << zexp_FA(1.00,0.0) << endl;
    cout << " FAZ(1.50) = " << zexp_FA(1.50,0.0) << endl;
    cout << " FAZ(2.00) = " << zexp_FA(2.00,0.0) << endl;
    cout << " FAZ(3.00) = " << zexp_FA(3.00,0.0) << endl;
  }
}

double zexp_FA(double q2, double ma){
  (void) ma; // MA Ignored for this function
  
  // Read Params
  q2 = -fabs(q2);

  // Calculate z
  double z = zexp_GetZ(q2);
  double FA = 0.0;

  int ncount = zexp_nterms;
  if (zexp_q4limit > 0) ncount += 4;

  for (int i = 0; i <= ncount; i++){
    FA += pow(z,i) * zexp_aterms[i];
  }

  return FA;
}


void zexp_applysumrules(){
  //  cout << " ZEXP: Applying Sum Rules" << endl;
  //  PrintZExpTerms(false);

  // The Code below is from private correspondence
  // with Aaron Meyer on the calculation of sum Rules.
  // - P. Stowell

  // Gives the Q^-4 format at high Q^2
  double k0 = (double)zexp_nterms;
  double z0 = zexp_GetZ(0.0);

  double k1 = (double)zexp_nterms+1;
  double z1 = pow(z0, (int)k1);

  double k2 = (double)zexp_nterms+2;
  double z2 = pow(z0, (int)k2);

  double k3 = (double)zexp_nterms+3;
  double z3 = pow(z0, (int)k3);

  double k4 = (double)zexp_nterms+4;
  double z4 = pow(z0, (int)k4);

  // Get Delta (z shifts through terms)
  double del =  6.0
    - 1.0 * k4 * k3 * k2 * z1
    + 3.0 * k4 * k3 * z2 * k1
    - 3.0 * k4 * z3 * k2 * k1
    + 1.0 * z4 * k3 * k2 * k1;

  // Setup Starting Parameters
  double b0  = 0.0;
  double b1  = 0.0;
  double b2  = 0.0;
  double b3  = 0.0;
  double b0z = 1.267;

  for (int ki = 1;ki <= zexp_nterms;ki++){
    b0 += zexp_aterms[ki];
    b1 += ki * zexp_aterms[ki];
    b2 += ki * (ki - 1) * zexp_aterms[ki];
    b3 += ki * (ki - 1) * (ki - 2) * zexp_aterms[ki];

    b0z += zexp_aterms[ki]*pow(z0,ki);
  }

  // A0
  zexp_aterms[0] =
    (- 6.*b0z - b0*(del-6.) + b3*(-z1 + 3.*z2 - 3.*z3 + z4)
     + b2 * (3.*k2*z1 - 3.*(3.*k0+5.)*z2 + 3.*(3.*k0+4.)*z3 - 3.*k1*z4)
     + b1 * (-3.*k3*k2*z1 + 3.*k3*(3.*k0+4.)*z2
	     -3.*k1*(3.*k0+8.)*z3 + 3.*k2*k1*z4) ) / (del);

  // A1
  zexp_aterms[(int)k1] =                        \
    (- (b0-b0z)*k4*k3*k2                                \
     + b3*(1. - 0.5*k4*k3*z2 + k4*k2*z3 - 0.5*k3*k2*z4) \
     + b2*(-3.*k2 + k4*k3*k2*z2                         \
	   - k4*k2*(2.*k0+3.)*z3 + k3*k2*k1*z4)         \
     + b1*(3.*k3*k2 - 0.5*k4*k3*k3*k2*z2                \
	   + k4*k3*k2*k1*z3 - 0.5*k3*k2*k2*k1*z4)       \
     ) / (del) ;

  // A2
  zexp_aterms[(int)k2] =
    ( + 3.*(b0-b0z)*k4*k3*k1                                    \
      + b3*(-3. + 0.5*k4*k3*z1 - (3./2.)*k4*k1*z3 + k3*k1*z4)   \
      + b2*(3.*(3.*k0+5) - k4*k3*k2*z1 + 3*k4*k1*k1*z3          \
	    - k3*k1*(2.*k0+1.)*z4)                              \
      + b1*(-3.*k3*(3.*k0+4.) + 0.5*k4*k3*k3*k2*z1              \
	    -(3./2.)*k4*k3*k1*k0*z3 + k3*k2*k1*k0*z4)           \
      ) / (del);

  // A3
  zexp_aterms[(int)k3] =                                       \
    (- 3.*(b0-b0z)*k4*k2*k1                                    \
     + b3*(3. - k4*k2*z1 + (3./2.)*k4*k1*z2 - 0.5*k2*k1*z4)   \
     + b2*(-3.*(3.*k0+4.) + k4*k2*(2.*k0+3.)*z1               \
	   - 3.*k4*k1*k1*z2 + k2*k1*k0*z4)                    \
     + b1*(3.*k1*(3.*k0+8.) - k4*k3*k2*k1*z1                  \
	   +(3./2.)*k4*k3*k1*k0*z2 - (1./2.)*k2*k1*k1*k0*z4)  \
     ) / (del);

  // A4
  zexp_aterms[(int)k4] =                                      \
    ( + (b0-b0z)*k3*k2*k1                                     \
      + b3*(-1. + (1./2.)*k3*k2*z1 - k3*k1*z2 + 0.5*k2*k1*z3) \
      + b2*(3.*k1 - k3*k2*k1*z1 + k3*k1*(2.*k0+1.)*z2         \
	    - k2*k1*k0*z3)                                    \
      + b1*(-3.*k2*k1 + 0.5*k3*k2*k2*k1*z1                    \
	    -k3*k2*k1*k0*z2 + 0.5*k2*k1*k1*k0*z3)             \
      ) / (del);


  return;
}

void zexp_applyq0limit(){

  double z = zexp_GetZ(0.0);
  double FA = 0.0;

  for (int i = 1; i <= zexp_nterms; i++){
    FA = FA + pow(z, i)* zexp_aterms[i];
  }
  zexp_aterms[0]= -1.267 - FA;

  return;
}

///////////////////////////////////////////////////////////////
// Calculate the axial form factors
pair<double, double> fap(double q2, int kind) {

  double ksi = 3.706;

  static const double M12 = (PDG::mass_proton + PDG::mass_neutron) / 2;
  static const double MM = M12 * M12;

  //C Thorpe added Dec 2018
  //hyperon mass and kaon mass required for g3 calculation 
  //using Phys Rev D98 (2018) no.3 033005, eq. 48
  double hyp_mass;
  double kmass = PDG::mass_K;

  double Ga, Fpa, Gas, Fpas;
  double Fp = 0, Fa = 0;

  switch (kind) {
    case 0:  // cc
      //Fa = Axialfromq2(q2, MA_cc); 
      Fa = Axialfromq2(q2, rew.qel_cc_axial_mass.val); 
      Fa *= axialcorr(axialFFset, q2);
      Fp = 2 * MM * Fa / (piMass2 - q2);
      break;
    case 1:  // nc proton
      //Fa = 0.5 * Axialfromq2(q2, MA_nc); 
      Fa = 0.5 * Axialfromq2(q2, rew.qel_nc_axial_mass.val); 
      // Fp=2.0*M2*Fa/(piMass2 - q2) ;
      break;
    case 2:  // nc neutron
      //Fa = -0.5 * Axialfromq2(q2, MA_nc);
      Fa = -0.5 * Axialfromq2(q2, rew.qel_nc_axial_mass.val);
      // Fp=2.0*M2*Fa/(piMass2 - q2) ;
      break;
    case 3:
    case 6:  // mec cc
      Fa = Axialfromq2(q2, MA_cc_mec);
      Fa *= axialcorr(axialFFset, q2);
      Fp = 2 * MM * Fa / (piMass2 - q2);
      break;
    case 4:
    case 7:  // mec nc proton
      Fa = 0.5 * Axialfromq2(q2, MA_nc_mec);
      // Fp=2.0*M2*Fa/(piMass2 - q2) ;
      break;
    case 5:
    case 8:  // mec nc neutron
      Fa = -0.5 * Axialfromq2(q2, MA_nc_mec);
      // Fp=2.0*M2*Fa/(piMass2 - q2) ;
      break;

    //C Thorpe added Dec 2018
    //hyperon production channels

    //Lambda zero;
    case 12:
      // need to add x=F/(F+D) value to constants file 
      // x = 0.36543014996
      //Fa = Axialfromq2(q2, MA_hyp); 

      Fa = Axialfromq2(q2,MA_hyp);
      Fa *= (-1)*(1+2*Axial_x)/sqrt(6);

      //SU(3) symmetry breaking
      if(sym_break == true)
      {
        Fa *= 1.072;
      }
      hyp_mass = PDG::mass_Lambda;

      //Fp = Fa*(M12+hyp_mass)*(M12+hyp_mass)/(2*(kmass*kmass-q2));
      Fp = Fa*(M12+hyp_mass)*(M12+hyp_mass)/(2*(kmass*kmass-q2));
      break;

    //Sigma zero
    case 13:
      //need to add x=F/(F+D) value to constants file 
      // x = 0.36543014996
      Fa = Axialfromq2(q2, MA_hyp); 
      //Fa  = 1.267/((1-q2/(MA_hyp*MA_hyp))*(1-q2/(MA_hyp*MA_hyp)));
      Fa *= (1-2*Axial_x)/(pow(2,0.5));
      //SU(3) symmetry breaking
      if(sym_break == true)
      {
        Fa *= 1.051;
      }
      hyp_mass = PDG::mass_Sigma;
      Fp = Fa*(M12+hyp_mass)*(M12+hyp_mass)/(2*(kmass*kmass-q2));
      break;

    //Sigma minus
    case 14:
      // need to add x=F/(F+D) value to constants file 
      // x = 0.36543014996
      Fa = Axialfromq2(q2, MA_hyp);
      //Fa  = 1.267/((1-q2/(MA_hyp*MA_hyp))*(1-q2/(MA_hyp*MA_hyp)));
      Fa *= (1-2*Axial_x);
      //SU(3) symmetry breaking
      if(sym_break == true)
      {
        Fa *= 1.056;
      }
      hyp_mass = PDG::mass_SigmaM;
      Fp = Fa*(M12+hyp_mass)*(M12+hyp_mass)/(2*(kmass*kmass-q2));
      break;
  }

  if ((kind == 1 or kind == 2) and strange) {
    switch (strange) {
      case 1: {
        Fa -= -0.5 * delta_s / pow2(1 - q2 / MA_nc / MA_nc);
      } break;
      case 2:  // new implementation
      {
        Fa -= -0.5 * delta_s / pow2(1 - q2 / MA_s / MA_s);
      }
      default:
        break;  // no strange content
    }
  }

  return pair<double, double>(Fa, Fp);
}

///////////////////////////////////////////////////////////////
double axialcorr(int axialFF, double q2) {
  //////////////////////////////
  double min;   // maximal reduction
  double max;   // maximal enhancement
  double szer;  // reduction in Q2
  double dlug;  // enhancement range

  switch (axialFF) {
    case 2:
      min = 0.9;   // maximal reduction
      max = 1.1;   // maximal enhancement
      szer = 0.2;  // reduction in Q2
      dlug = 2.0;  // enhancement range
      break;

    case 3:
      min = 0.8;   // maximal reduction
      max = 1.2;   // maximal enhancement
      szer = 0.2;  // reduction in Q2
      dlug = 2.0;  // enhancement range

    default:
      return 1;
  }

  // parabolic approximation
  double x = -q2 / GeV2;
  if (x < szer)
    return 1.0 + 4.0 / szer / szer * (1.0 - min) * x * (x - szer);
  else if (x < dlug)
    return 1.0 -
           4.0 * (max - 1.0) / pow2(dlug - szer) * (x - szer) * (x - dlug);
  else
    return 1.0;
}

/////////////////////////////////////////////////////////////
// C Thorpe Added Dec 2018
// Second class current Form Factors in Hyperon production
/////////////////////////////////////////////////////////////

pair<double,double>g2(double q2,int kind){

  // real and imaginary parts of form factor g2
  // imaginary values corresponding to TRV
  // assume dipole form with same axial mass as g1

  double Rg2 = (-1)*Rg20/(pow(1-q2/(MA_hyp*MA_hyp),2));
  double Ig2 = (-1)*Ig20/(pow(1-q2/(MA_hyp*MA_hyp),2));

  switch(kind)
  {
  //Lambda zero production
  case 12:
    Rg2 *= (-1)*(1+2*Axial_x)/pow(6,0.5);
    Ig2 *= (-1)*(1+2*Axial_x)/pow(6,0.5);
    break;

  //Sigma zero production
  case 13:
    Rg2 *= (1-2*Axial_x)/(pow(2,0.5));
    Ig2 *= (1-2*Axial_x)/(pow(2,0.5));
    break;

  //Sigma minus production
  case 14:
    Rg2 *= (1-2*Axial_x);
    Ig2 *= (1-2*Axial_x);
    break;
  
  // for ds=0 quasielastic do not include SCC for the time being
  default:
    Rg2 =0;
    Ig2 =0;
    break;
  }

  return pair<double,double>(Rg2,Ig2);
}

///////////////////////////////////////////////////////////////
/// Form Factor Configuration
void ff_configure(params & p) {
  switch (p.qel_vector_ff_set) {
    case 1:
      FFfromq2 = DipoleFF;
      break;
    case 2:
      FFfromq2 = bbba05_FF;
      break;
    case 3:
      FFfromq2 = bba03_FF;
      break;
    case 4:
      FFfromq2 = JLab_FF;
      break;
    case 5:
      FFfromq2 = kg_FF;
      break;
    case 6:
      FFfromq2 = npar_FF;
      break;
    case 7:
      FFfromq2 = bbba07_FF;
      break;
    default:
      throw("bad ffset");
  };

  // Set BBBA07 Vector Pars
  if (p.qel_vector_ff_set == 7) {
    for(int i=0;i<7;i++)
    {
      p_AEp[i] = (&rew.bba07_AEp1)[i].val;
      p_AMp[i] = (&rew.bba07_AMp1)[i].val;
      p_AEn[i] = (&rew.bba07_AEn1)[i].val;
      p_AMn[i] = (&rew.bba07_AMn1)[i].val;
    }

  }

  // AXIAL FF SET
  axialFFset = p.qel_axial_ff_set;
  switch(axialFFset){
  case 1:
  case 2:
  case 3: Axialfromq2 = dipole_FA;  break;
  case 4: Axialfromq2 = bbba07_FA;  
    for(int i=0;i<7;i++)
      p_AAx[i] = (&rew.bba07_AAx1)[i].val;
    
          break;
  case 5: Axialfromq2 = comp2_FA;
    axial_ff_gamma = rew.qel_axial_2comp_gamma.val;
    axial_ff_alpha = rew.qel_axial_2comp_alpha.val;
    break;
  case 6: Axialfromq2 = comp3_FA;
    axial_ff_gamma = rew.qel_axial_2comp_gamma.val;
    axial_ff_alpha = rew.qel_axial_2comp_alpha.val;
    axial_ff_beta = rew.qel_axial_3comp_beta.val;
    axial_ff_theta = rew.qel_axial_3comp_theta.val;
    break;
  case 7:  Axialfromq2 = zexp_FA; // ZEXPANSION
    zexp_nterms = rew.zexp_nterms.val;
    if (zexp_nterms > 10)  // Truncate at 10 terms
      zexp_nterms = 10;

    // Get T values
    zexp_tc = rew.zexp_tc.val;
    zexp_t0 = rew.zexp_t0.val;
    
    // Get Terms
    for(int i=0;i<10;i++)
       zexp_aterms[i] = (&rew.zexp_a0)[i].val;

    // Set Limits
    zexp_q4limit = bool(rew.zexp_q4limit.val);
    if (zexp_q4limit) zexp_applysumrules();
    else zexp_applyq0limit();

    //    PrintZExpTerms(true);
    //    sleep(5);
    break;
    
  default:
    throw("bad axial ffset");
    break;
  }

  strange = p.qel_strange;
  strangeEM = p.qel_strangeEM;
  delta_s = rew.delta_s.val;
  MA_cc = rew.qel_cc_axial_mass.val;
  MA_nc = rew.qel_nc_axial_mass.val;
  MA_s = rew.qel_s_axial_mass.val;

  /////////////////////////////////
  //added by c thorpe
  //locate parameter values in params

  //hyperon axial mass
  MA_hyp = p.hyp_axial_mass;

  //SCC Setup
  //values of g2 at Q2 = 0
  Rg20 = p.hyp_g2_Re_part;
  Ig20 = p.hyp_g2_Im_part;

  //symmetry breaking setup
  if(p.hyp_su3_sym_breaking)
    sym_break = true;
  ////////////////////////
}

//________________________________________________________
