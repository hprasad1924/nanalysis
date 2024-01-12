#include "hyperon_cascade.h" 
#include <iostream>
#include "pdg.h"
#include <math.h>
#include "particle.h"
#include <fstream>

// Created by c thorpe Jan 2019

///////////////////////////////////////////////

// Cross section fits from  
// SK Signh and MJ Vicente Vacas Phys Rev D74 (2006) 053009 
// see appendix 

// calculate cross sections depending on initial state
// and which final states are allowed by kinematics

// if the nucleon inserted into the interaction is a proton and code is trying to calculate a cross 
// section for a process where a neutron is in the initial state


// Phase space ratios now handled in phase_space function below

//////////////////////////////////////////////

/*
   Setup the cross section data and store in sigma[]
   Argument list:

   E = CMS energy
   Plab = hyperon momentum in nucleon rest frame
   sigma[] = stores hyperon/nucleon cross sections
   hyp_state = indicates the initial state 

*/

///////////////////////////////////////////////////

void hyperon_exp_xsec(double E, double Plab, double sigma[] , int hyp_state){

   bool Print = false; 

   //momentum in  units of GeV
   //convert from MeV to GeV
   Plab /= GeV; 
   double x;


   //EVERYTHING ELSE IN MeV 
   // all calculations except for sigma 1-4 are dimensionless 
   // phase space ratios so as long as the units used for R are the same 
   // then its is safe to use different units 

   if(Plab > 2.1){x = 2.1;}
   else {x=Plab;}


   double M_N1; //initial nucleon mass
   double M_N2; //final nucleon mass
   double M_L = PDG::mass_Lambda; //lambda mass
   double M_S; // sigma mass

   //CMS momentum ratio (used as a phase space correction)
   double R;


   //Singh and Vicente Vacas fits
   double sigma_1 = (39.66 - 100.45*x + 92.44*x*x - 21.4*x*x*x)/(Plab)*millibarn;
   double sigma_2 =  (31.1 - 30.94*x + 8.16*x*x)*millibarn;
   double sigma_3 = (11.77/Plab + 19.07)*millibarn;
   double sigma_4 =  (22.4/Plab - 1.08)*millibarn;



   // check what final states are energetically accessible
   // and set cross sections

   /////////////////////////////////////////////////////////////////////
   // INITIAL HYPERON LAMBDA 
   /////////////////////////////////////////////////////////////////////////

   if(hyp_state == 0 || hyp_state == 4){

      double KE = (sqrt(Plab*Plab*1e6 + PDG::mass_Lambda*PDG::mass_Lambda) - PDG::mass_Lambda)/1e3;

      //first cross section is always accessible
      //set elastic cross sections sigma[0] and sigma[3]

      /////////////////////////////////
      // LAMBDA P -> LAMBDA P
      ////////////////////////////////
      sigma[0] = sigma_1;
      if(Print) std::cout << "LZ+P to LZ+P " << Plab << " " << KE << " " << sigma[0] << std::endl;

      /////////////////////////////
      // LAMBDA N -> LAMBDA N
      //////////////////////////////
      sigma[3] = sigma[0];
      if(Print) std::cout << "LZ+N to LZ+N " << Plab << " " << KE << " " << sigma[3] << std::endl;

      //////////////////////////////////
      // LAMBDA P -> SIGMA+ N
      ///////////////////////////////////

      E = cms_energy(Plab*GeV,PDG::mass_proton,PDG::mass_Lambda);

      if(E > (PDG::mass_SigmaP + PDG::mass_neutron)){

         M_S = PDG::mass_SigmaP;
         M_N1 = PDG::mass_proton;
         M_N2 = PDG::mass_neutron;



         R=phase_space(E,M_S,M_N2,M_L,M_N1);


         sigma[1] = 2*sigma_2*R;
         if(Print) std::cout << "LZ+P to SP+N " << Plab << " " << KE << " " << sigma[1] << std::endl;


         //////////////////////////////////
         // LAMBDA P -> SIGMA0 P
         //////////////////////////////////



         if(E > (PDG::mass_Sigma + PDG::mass_proton)){

            M_N1 = PDG::mass_proton;
            M_N2 = PDG::mass_proton;
            M_S = PDG::mass_Sigma;


            R = phase_space(E,M_S,M_N2,M_L,M_N1);

            sigma[2] = sigma_2*R;
            if(Print) std::cout << "LZ+P to SZ+P " << Plab << " " << KE << " " << sigma[2] << std::endl;

         }
         else {sigma[2] = 0;}

      }
      else {sigma[1]=0; sigma[2] = 0;}


      ///////////////////////////////
      // LAMBDA N -> SIGMA0 N
      ////////////////////////////////

      E = cms_energy(Plab*GeV,PDG::mass_neutron,PDG::mass_Lambda);

      if(E > (PDG::mass_Sigma + PDG::mass_neutron)){

         M_N1 = PDG::mass_neutron;
         M_N2 = PDG::mass_neutron;
         M_S = PDG::mass_Sigma;

         //	E = cms_energy(Plab*GeV,M_N1,M_L);

         R = phase_space(E,M_S,M_N2,M_L,M_N1);

         sigma[4] = sigma_2*R;
         if(Print) std::cout << "LZ+N to SZ+N " << Plab << " " << KE << " " << sigma[4] << std::endl;

         //	std::cout << "sigma4: " << sigma[4] << std::endl;


         ////////////////////////////////////////
         // LAMBDA N -> SIGMA- P
         //////////////////////////////////////

         if(E > (PDG::mass_SigmaM + PDG::mass_proton)){

            M_S = PDG::mass_SigmaM;
            M_N1 = PDG::mass_neutron;
            M_N2 = PDG::mass_proton;

            //	E = cms_energy(Plab*GeV,M_N1,M_L);

            R = phase_space(E,M_S,M_N2,M_L,M_N1);


            sigma[5] = 2*sigma_2*R;
            if(Print) std::cout << "LZ+N to SM+P " << Plab << " " << KE << " " << sigma[5] << std::endl;

            //	std::cout << "sigma5: " << sigma[5] << std::endl;

         }
         else {sigma[5] = 0;}

      }

      else {sigma[4] = 0; sigma[5] = 0;}
   }


   //////////////////////////////////////////////////////////////////////////////
   // INITIAL HYPERON SIGMA0 
   ///////////////////////////////////////////////////////////////////////////////

   //sigma0 and proton or neutron in initial state
   else if(hyp_state == 1 || hyp_state == 5){
      //all final states are energetically accessible at any momentum

      double KE = (sqrt(Plab*Plab*1e6 + PDG::mass_Sigma*PDG::mass_Sigma) - PDG::mass_Sigma)/1e3;

      E = cms_energy(Plab*GeV,PDG::mass_proton,PDG::mass_Sigma);

      //////////////////////////
      // SIGMA0 P -> SIGMA0 P
      ///////////////////////////

      sigma[0] = sigma_4; //sigma0 p -> sigma0 p 
      if(Print) std::cout << "SZ+P to SZ+P " << Plab << " " << KE << " " << sigma[0] << std::endl;


      ////////////////////////////////////
      // SIGMA 0 P -> LAMBDA P
      //////////////////////////////////////

      M_S = PDG::mass_Sigma;
      M_N1 = PDG::mass_proton;
      M_N2 = PDG::mass_proton;

      R = phase_space(E,M_L,M_N2,M_S,M_N1);

      sigma[1] = sigma_2*R;
      if(Print) std::cout << "SZ+P to LZ+P " << Plab << " " << KE << " " << sigma[1] << std::endl;


      //////////////////////////////////
      // SIGMA0 P -> SIGMA+ N 
      /////////////////////////////////

      sigma[2] = sigma_4; 
      if(Print) std::cout << "SZ+P to SP+N " << Plab << " " << KE << " " << sigma[2] << std::endl;

      /////////////////////////////////////
      // SIGMA0 N -> SIGMA0 N
      /////////////////////////////////////

      E = cms_energy(Plab*GeV,PDG::mass_neutron,PDG::mass_Sigma);

      sigma[3] = sigma_4; //sigma0 n -> sigma0 n
      if(Print) std::cout << "SZ+N to SZ+N " << Plab << " " << KE << " " << sigma[3] << std::endl;

      ////////////////////////////////////
      //SIGMA0 N -> LAMBDA N
      ////////////////////////////////

      sigma[4] = sigma[1];
      if(Print) std::cout << "SZ+N to LZ+N " << Plab << " " << KE << " " << sigma[4] << std::endl;

      ///////////////////////////////////////
      // SIGMA0 N -> SIGMA- P
      /////////////////////////////////////

      if(E > (PDG::mass_SigmaM + PDG::mass_proton)){
         sigma[5] = sigma_4;
         if(Print) std::cout << "SZ+N to SM+P " << Plab << " " << KE << " " << sigma[5] << std::endl;
      }
      else {sigma[5] = 0;}

   }

   ////////////////////////////////////////////////////////////////////////////////////////
   // SIGMA MINUS INITIAL HYPERON
   ///////////////////////////////////////////////////////////////////////////////////////

   else if(hyp_state == 2 || hyp_state == 6){

      double KE = (sqrt(Plab*Plab*1e6 + PDG::mass_SigmaM*PDG::mass_SigmaM) - PDG::mass_SigmaM)/1e3;

      E = cms_energy(Plab*GeV,PDG::mass_proton,PDG::mass_SigmaM);

      /////////////////////////
      // SIGMA- P -> SIGMA- P
      /////////////////////////

      sigma[0] = sigma_4;
      if(Print) std::cout << "SM+P to SM+P " << Plab << " " << KE << " " << sigma[0] << std::endl;

      ///////////////////////////
      // SIGMA- P -> LAMBDA N
      //////////////////////////

      M_S = PDG::mass_SigmaM;
      M_N1 = PDG::mass_proton;
      M_N2 = PDG::mass_neutron;

      R = phase_space(E,M_L,M_N2,M_S,M_N1);

      sigma[1] = 2*sigma_2*R; //sigmaM p -> Lambda n
      if(Print) std::cout << "SM+P to LZ+N " << Plab << " " << KE << " " << sigma[1] << std::endl;

      //////////////////////////
      // SIGMA- P -> SIGMA0 N
      ////////////////////////


      sigma[2] = sigma_4; //sigmaM p -> Sigma0 n 
      if(Print) std::cout << "SM+P to SZ+N " << Plab << " " << KE << " " << sigma[2] << std::endl;

      /////////////////////////////////
      // SIGMA- N -> SIGMA- N
      ////////////////////////////////

      E = cms_energy(Plab*GeV,PDG::mass_neutron,PDG::mass_SigmaM);

      sigma[3] = sigma_3; //sigmaM n -> sigmaM n 
      if(Print) std::cout << "SM+N to SM+N " << Plab << " " << KE << " " << sigma[3] << std::endl;

      /////////////////////////////////

      //no other final states for sigmaM and neutron
      sigma[4] = 0;
      sigma[5] = 0;

   }

   ///////////////////////////////////////////////////////////////////////////////////////
   // INITIAL HYPERON SIGMA PLUS 
   //////////////////////////////////////////////////////////////////////////////////////


   else if(hyp_state == 3 || hyp_state == 7){

      double KE = (sqrt(Plab*Plab*1e6 + PDG::mass_SigmaP*PDG::mass_SigmaP) - PDG::mass_SigmaP)/1e3;

      E = cms_energy(Plab*GeV,PDG::mass_proton,PDG::mass_SigmaP);

      M_S = PDG::mass_SigmaP;



      /////////////////////////////
      // SIGMA+ P -> SIGMA+ P
      /////////////////////////////

      sigma[0] = sigma_3; //sigmaP p -> sigmaP p
      if(Print) std::cout << "SP+P to SP+P " << Plab << " " << KE << " " << sigma[0] << std::endl;


      ////////////////////////////

      //no other final states for sigmaP proton
      sigma[1] = 0;
      sigma[2] = 0;

      /////////////////////////
      // SIGMA+ N -> SIGMA+ N 
      ///////////////////////////

      E = cms_energy(Plab*GeV,PDG::mass_neutron,PDG::mass_SigmaP);

      sigma[3] = sigma_4; //sigmaP n -> sigmaP n 
      if(Print) std::cout << "SP+N to SP+N " << Plab << " " << KE << " " << sigma[3] << std::endl;

      /////////////////////////////
      // SIGMA+ N -> LAMBDA P
      ////////////////////////////

      M_N1 = PDG::mass_neutron;
      M_N2 = PDG::mass_proton;

      R = phase_space(E,M_L,M_N2,M_S,M_N1);

      sigma[4] = 2*sigma_2*R; //sigmaP n -> lambda p 
      if(Print) std::cout << "SP+N to LZ+P " << Plab << " " << KE << " " << sigma[4] << std::endl;

      /////////////////////////////
      // SIGMA+ N -> Sigma0 P
      ////////////////////////////

      //no third reaction for this initial state
      sigma[5] = 0;

      ////////////////////////////////////////////////////////////////////////////////////////////

   }

   else
   { 
      for(int i=0;i<6;i++)
      {
         std::cout << "hyperon cross section error" << std::endl;
         sigma[i] = 0;
      }	   
   }

}

////////////////////////////////////////////////////////////////////////////////////////

/*
   Select final state based on cross sections and what the initial hyperon-nucleon state is

   Argument list

   hyp_state contains information about the initial pair of particles
   sigma[] the list of hyperon-nucleon cross sections 
   ij argument given to scattering code to select differential cross sections
   p[] array containing the final particles
   */

///////////////////////////////////////////////////////////////////////////////////////

void hyperon_state(int hyp_state,double sigma[],int &ij, particle p[]){


   // Decide how to shape the cross sections based on the electric charges
   // of the initial/final state particles and if they
   // have a matching p/n interaction process

   // ij are set as follows
   // 0 if process is either ++ -> ++ or 00 -> 00 
   // 1 if process is either  +0 -> 0+ or 0- -> 0- 
   // 2 for all other combinations


   //generate random number 
   double R = frandom();

   double sigma_p = sigma[0]+sigma[1]+sigma[2]; //hyperon-proton total cross section
   double sigma_n = sigma[3]+sigma[4]+sigma[5]; //hyperon-neutron total cross section

   //lambda proton
   if(hyp_state == 0){


      //final state is sigma0 p 
      if(R > ((sigma[0] + sigma[1])/sigma_p)){

         //lambda + proton -> sigma0 + proton

         ij = 1;

         p[0].set_pdg_and_mass(PDG::pdg_Sigma,PDG::mass_Sigma);
         p[1].set_pdg_and_mass(PDG::pdg_proton,PDG::mass_proton);

      }
      //final state is sigmaP n    
      else if(R > (sigma[0]/sigma_p)){

         //lambda + proton -> sigmaP + neutron

         ij = 1;

         p[0].set_pdg_and_mass(PDG::pdg_SigmaP,PDG::mass_SigmaP);
         p[1].set_pdg_and_mass(PDG::pdg_neutron,PDG::mass_neutron);

      }
      //final state is lambda p
      else {

         //lambda + proton -> sigmaP + neutron

         ij = 1;

         p[0].set_pdg_and_mass(PDG::pdg_Lambda,PDG::mass_Lambda);
         p[1].set_pdg_and_mass(PDG::pdg_proton,PDG::mass_proton);

      }

   }
   //lambda neutron
   else if(hyp_state == 4){

      //final state is sigmaM p 
      if(R > ((sigma[3] + sigma[4])/sigma_n)){

         //lambda + neutron -> sigmaM + proton

         ij = 2;

         p[0].set_pdg_and_mass(PDG::pdg_SigmaM,PDG::mass_SigmaM);
         p[1].set_pdg_and_mass(PDG::pdg_proton,PDG::mass_proton);
      }
      //final state is sigma0 n    
      else if(R > (sigma[3]/sigma_n)){


         //lambda + neutron -> sigma0 + neutron

         ij = 0;

         p[0].set_pdg_and_mass(PDG::pdg_Sigma,PDG::mass_Sigma);
         p[1].set_pdg_and_mass(PDG::pdg_neutron,PDG::mass_neutron);

      }
      //final state is lambda n
      else {

         //lambda + neutron -> lambda + neutron

         ij = 0;

         p[0].set_pdg_and_mass(PDG::pdg_Lambda,PDG::mass_Lambda);
         p[1].set_pdg_and_mass(PDG::pdg_neutron,PDG::mass_neutron);



         //  std::cout << "Final state lambda neutron" << std::endl;
         // std::cout << p[0].pdg << "  " << p[1].pdg << std::endl;

      }
   }

   //sigma 0 proton
   else  if(hyp_state == 1){


      //final state is sigmaP neutron 
      if(R > ((sigma[0] + sigma[1])/sigma_p)){

         ij=1;

         p[0].set_pdg_and_mass(PDG::pdg_SigmaP,PDG::mass_SigmaP);
         p[1].set_pdg_and_mass(PDG::pdg_neutron,PDG::mass_neutron);
      }
      //final state is Lambda proton    
      else if(R > (sigma[0]/sigma_p)){

         ij=1;

         p[0].set_pdg_and_mass(PDG::pdg_Lambda,PDG::mass_Lambda);
         p[1].set_pdg_and_mass(PDG::pdg_proton,PDG::mass_proton);


      }
      //final state is sigma 0 proton
      else {

         ij=1;

         p[0].set_pdg_and_mass(PDG::pdg_Sigma,PDG::mass_Sigma);
         p[1].set_pdg_and_mass(PDG::pdg_proton,PDG::mass_proton);

      }
   }

   //initial state is sigma0 neutron
   else  if(hyp_state == 5){

      //final state is sigmaM p 
      if(R > ((sigma[3] + sigma[4])/sigma_n)){

         ij=2;

         p[0].set_pdg_and_mass(PDG::pdg_SigmaM,PDG::mass_SigmaM);
         p[1].set_pdg_and_mass(PDG::pdg_proton,PDG::mass_proton);
      }
      //final state is Lambda n    
      else if(R > (sigma[3]/sigma_n)){

         ij=0;

         p[0].set_pdg_and_mass(PDG::pdg_Lambda,PDG::mass_Lambda);
         p[1].set_pdg_and_mass(PDG::pdg_neutron,PDG::mass_neutron);

         // std::cout << "initial state sigma0 neutron" << std::endl;

      }
      //final state is Sigma0 neutron
      else {

         ij=0;

         p[0].set_pdg_and_mass(PDG::pdg_Sigma,PDG::mass_Sigma);
         p[1].set_pdg_and_mass(PDG::pdg_neutron,PDG::mass_neutron);

      }
   }


   //sigmaM proton
   else if(hyp_state == 2){

      ij=2;

      //final state is sigma0 neutron 
      if(R > ((sigma[0] + sigma[1])/sigma_p)){

         p[0].set_pdg_and_mass(PDG::pdg_Sigma,PDG::mass_Sigma);
         p[1].set_pdg_and_mass(PDG::pdg_neutron,PDG::mass_neutron);
      }
      //final state is Lambda neutron    
      else if(R > (sigma[0]/sigma_p)){

         p[0].set_pdg_and_mass(PDG::pdg_Lambda,PDG::mass_Lambda);
         p[1].set_pdg_and_mass(PDG::pdg_neutron,PDG::mass_neutron);

         //  std::cout << "initial state sigmaM proton" << std::endl;

      }
      //final state is sigma M  proton
      else {

         p[0].set_pdg_and_mass(PDG::pdg_SigmaM,PDG::mass_SigmaM);
         p[1].set_pdg_and_mass(PDG::pdg_proton,PDG::mass_proton);

      }
   }

   //initial state is sigmaM neutron
   else  if(hyp_state == 6){

      ij = 1;

      //only one final state allowed

      p[0].set_pdg_and_mass(PDG::pdg_SigmaM,PDG::mass_SigmaM);
      p[1].set_pdg_and_mass(PDG::pdg_neutron,PDG::mass_neutron);

   }


   //sigmaP proton
   else if(hyp_state == 3){

      ij=0;

      //only one allowed final state

      p[0].set_pdg_and_mass(PDG::pdg_SigmaP,PDG::mass_SigmaP);
      p[1].set_pdg_and_mass(PDG::pdg_proton,PDG::mass_proton);


   }


   //initial state is sigmaP neutron
   else if(hyp_state == 7){

      ij=1;

      //final state is Lambda p   
      if(R > (sigma[3]/sigma_n)){

         p[0].set_pdg_and_mass(PDG::pdg_Lambda,PDG::mass_Lambda);
         p[1].set_pdg_and_mass(PDG::pdg_proton,PDG::mass_proton);

         //   std::cout << "initial state sigmaP neutron" << std::endl;

      }
      //final state is SigmaP neutron
      else {

         p[0].set_pdg_and_mass(PDG::pdg_SigmaP,PDG::mass_SigmaP);
         p[1].set_pdg_and_mass(PDG::pdg_neutron,PDG::mass_neutron);

      }
   }
   else 
   { 

      ij=3; 


   }

}

/////////////////////////////////////////////////////////////////////////////////////
/* Hyperon Phase space corrections


   Added by C Thorpe Nov 2019

   Calculate momenta of outgoing particles for two different final (2 particle) states
   in the CMS and take their ratio


   compare the reactions:

   rs -> 1a+2a 
   rs -> 1b+2b

   Argument list:
   rs = cms energy
   M1a = Mass of particle 1a
   M2a = Mass of particle 2a
   M1b = Mass of paritcle 1b
   M2b = Mass of particle 2b

*/

//////////////////////////////////////////////////////////////////////////////////////

double phase_space(double rs, double M1a, double M2a, double M1b, double M2b){

   double a = cms_momentum2(rs*rs,M1a*M1a,M2a*M2a);
   double b = cms_momentum2(rs*rs,M1b*M1b,M2b*M2b);

   // If either process is forbidden
   if(a < 0 || b < 0) return 0;

   //a =  pow(rs,4) + pow(M1a,4) + pow(M2a,4) - 2*(rs*rs*M1a*M1a + rs*rs*M2a*M2a + M1a*M1a*M2a*M2a);
   //b = pow(rs,4) + pow(M1b,4) + pow(M2b,4) - 2*(rs*rs*M1b*M1b + rs*rs*M2b*M2b + M1b*M1b*M2b*M2b);

   return sqrt(a/b);

}

/////////////////////////////////////////////////////////////////////////////////////////

/*
   Compute CMS energy of an initial state given the momentum of the hyperon in the nucleon rest frame
   and the masses of the hyperon and nucleon
*/

double cms_energy(double Plab, double M_N, double M_Y){

   return sqrt(M_N*M_N + M_Y*M_Y + 2*M_N*sqrt(M_Y*M_Y+Plab*Plab));

}

/////////////////////////////////////////////////////////////////////////////////////////
