#include <iostream>
#include <stdio.h>
#include <string>
#include "nuwro.h"
#include "tests.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2.h"
#include "Fillhistograms.h"
#include "fittings.h"
#include "TNtupleD.h"
#include "TNtuple.h"
#include "mecevent_common.h"
#include "TMath.h"
#include <stdlib.h>


const int progressBarWidth = 20;

void ReadRoot_Nuwro()
{
        bool reweight = false;
        double P = 0;
        int n = 1;

        std::string filename;
        char input;
        std::cout << "\t Enter File (complete address) : ";
        std::cin >> filename;

        std::cout << "\t Do you want reweighting (y/n) : ";
        std::cin >> input;
        if (input == 'y')
        {
                int choice;
                reweight = true;
                std::cout << "\t\t (1) reweight with P \n";
                std::cout << "\t\t (2) reweight with (P,n) \n";
                std::cout << "\t\t ";
                std::cin >> choice;
                switch (choice)
                {
                        case 1: std::cout << "\t\t P = ";
                                std::cin >> P;
                                break;
                        case 2: std::cout << "\t\t P = ";
                                std::cin >> P;
                                std::cout << "\t\t n = ";
                                std::cin >> n ;
                                break;
                        default: std::cerr << "\t\t Wrong input !! reweighting is turned OFF \n";
                                 reweight = false;
                                break;
                }
        }

        const char *filename_c = filename.c_str();
        TFile *file = new TFile(filename_c);
        if (file->IsZombie())
        {
                std::cerr << "File does not exist !! \n";
                std::exit(1);
        }
        bool inclusive=0, cthad=0, exclusive=0;
        std::string flag;
        do 
        {
                std::cout << "\t Toggle between options to keep (enter <stop> to stop): \n";
                std::cout << "\t <exit> \n";
                std::cout << "\t (1) Inclusive Phasespace (keyword = inc/inc_OFF): " << inclusive << " \n";
                std::cout << "\t (2) Distribution of cos_theta_had (keyword = ct/ct_OFF): " << cthad << " \n";
                std::cout << "\t (3) Outgoing Nucleon Phase-space (keyword = exc/exc_OFF): " << exclusive << " \n";
                std::cin >> flag;
                if (flag == "exit")
                {
                        std::exit(0);
                }
                else if (flag == "inc" || flag == "inc_OFF")
                {
                        if (flag == "inc_OFF")
                        {
                                inclusive = 0;
                        }
                        inclusive=1;
                }
                else if (flag == "ct")
                {
                        if (flag == "ct_OFF")
                        {
                                cthad = 0;
                        }

                        cthad = 1;
                }
                else if (flag == "exc")
                {
                        if (flag == "exc_OFF")
                        {
                                exclusive = 0;
                        }

                        exclusive = 1;
                }

        
        }while(flag!="stop");

        TTree *tree = (TTree*)file->Get("treeout");
        event *e = new event();
        tree->SetBranchAddress("e", &e);
        Int_t nentries = (Int_t)tree->GetEntries();

        int NBins=80, NBinx=120, NBiny=120, NBinx2=120, NBiny2=120;
        double PMax = 1.8;

        if(cthad)
        {       
                std::cout << "\n\t\t\t--------------------------------------------\n";
                std::cout << "\t\t\t For costheta_{had} distribution \n";
                std::cout << "\t\t\t----------------------------------------------\n";
                std::cout << "\n\t Enter no of bins : ";
                std::cin >> NBins; 
        }
        if(inclusive)
        {
                std::cout << "\n\t\t\t--------------------------------------------\n";
                std::cout << "\t\t\t For inclusive phase-space distribution \n";
                std::cout << "\t\t\t----------------------------------------------\n";
                std::cout << "\t Enter no. of bins in  X-directions : ";
                std::cin >> NBinx;
                std::cout << "\t Enter no. of bins in Y-directions : ";
                std::cin >> NBiny;

        }
        if(exclusive)
        {
                std::cout << "\n\t\t\t--------------------------------------------\n";
                std::cout << "\t\t\t For exclusive phase-space  distribution \n";
                std::cout << "\t\t\t----------------------------------------------\n";
                std::cout << "\n\t Enter no. of bins in  X-directions : ";
                std::cin >> NBinx2;
                std::cout << "\t Enter no. of bins in Y-directions : ";
                std::cin >> NBiny2;

                tree->GetEntry(0); //Just to get the energy of the neutrino
                if (e->in[0].E() < 1*GeV)
                {
                        PMax = 1.0;
                }
		else if (e->in[0].E() < 1.8*GeV)
		{
			PMax = 1.8;
		}
		else
		{
			PMax = 2.0;
		}
        }
        TH1D *hist1D = new TH1D("hist1D_ct", "; cos#theta_{had}; entries", NBins, -1, 1);
        TH2D *hist2D_inc = new TH2D("hist2D_inc", "Inclusive Phase-space; q [GeV/c]; #omega [GeV]", NBinx, 0, 1.2, NBiny, 0, 1.2);
        TH2D *hist2D_exc = new TH2D("hist2D_exc", "Exclusive Phase-space; p_{1} [GeV/c] Leading Proton; p_{2} [GeV/c] SubLeading Proton", NBinx2, 0, PMax, NBiny2, 0, PMax);
        TH2D *hist2D_exc2 = new TH2D("hist2D_exc2", "Exculisve Phase-space; p_{1} [GeV/c] Neutron; p_{2} [GeV/c] Proton", NBinx2, 0, PMax, NBiny2, 0, PMax);

               
        for(Int_t i = 0; i < nentries; i++)
        {
                tree->GetEntry(i);
                if(cthad)
                {
                        Fillhist_ct(hist1D, e, reweight, P, n);
                        //hist1D->Fill(coshad(vect(e->out[1]), vect(e->out[2])) );
                }
                if(inclusive)
                {
                        Fillhist_inclusive(hist2D_inc, e, reweight, P, n);
                        //hist2D_inc->Fill(e->qv()/GeV, e->q0()/GeV);
                }
                if(exclusive)
                {
                        Fillhist_exclusive(hist2D_exc, hist2D_exc2, e, reweight, P, n);
                        //hist2D_exc->Fill(e->part_max_mom(2212,false)/GeV, e->part_sec_mom(2212,false)/GeV);
                }

                //Calculate the percentage of the progress
		double percentage = static_cast<double>(i+1) / nentries * 100.0;
		//Calculate the number of progressbar character
		int progressBarCount = static_cast<int>(percentage / 100 * progressBarWidth );

		
		//Print the Progress Bar
		std::cout << "\t Filling :" << "\t" << " [";
		for(int k = 0; k < progressBarCount; k++)
		{
			std::cout << "=" ;
		}
		for(int k = progressBarCount; k < progressBarWidth; k++)
		{
			std::cout << " ";
		}
		std::cout << "] " << static_cast<int>(percentage) << "%" << "\r";
		std::cout.flush();	
        }


        std::string outfilename;
        std::cout << "\n \tWhat name should I use to save histograms : ";
        std::cin >> outfilename;

        const char *outfile_c = outfilename.c_str();
        TFile *outfile = TFile::Open(outfile_c, "RECREATE");
        if(cthad)
        {
                hist1D->Write();
        }
        if(inclusive)
        {
                hist2D_inc->Write();
        }
        if(exclusive)
        {
                hist2D_exc->Write();
                hist2D_exc2->Write();
        }
        outfile->Close();
        file->Close();

}
int ReadRoot()
{
        int choice;
        std::cout << "\t (1) Read NTuples \n";
        std::cout << "\t (2) Read Histograms \n";
        std::cin >> choice;

        switch(choice)
        {
                case 1: std::cout << "\t Reading NTuples Root .. \n";
                        break;
                case 2: std::cout << "\t Reading histograms root .. \n";
                        break;
                default :
                        std::cerr << "\n \t !! No Matching Choice !! \n";
                        std::cout << "\t Returning back to main menu ... \n";
                        int errorcode = 1;
                        return 1;
        }

        return 0;
}
void WriteRoot()
{
        std::vector<std::string> filenames;
        std::string directory, name;
        std::cout << "\t Enter directory(s) of the root file : \n";
        std::cin >>directory;
        std::cout << "\t Enter filenames (without .root) (write <stop> to stop): \n";
        do {
                std::ostringstream fileaddress;
                std::cin >> name;
                fileaddress << name << ".root";
                if(name != "stop")
                {
                        filenames.push_back(fileaddress.str());
                }
        }while (name != "stop");

        for(int i = 0; i < filenames.size(); i++)
        {
                TNtupleD raw_data("raw_data", "Data of Events", "q:omega:p1:pdg1:p2:pdg2:kos:kosboundary:weight");

                std::ostringstream outfilename;
                outfilename << "tuple_" << filenames.at(i);
                std::string outfilename_string = outfilename.str();
                const char *outfilename_c = outfilename_string.c_str();
                TFile *outfile = new TFile(outfilename_c, "RECREATE");

                std::ostringstream filename;
                filename << directory << filenames.at(i).c_str();
                std::string filename_string = filename.str();
                const char *filename_c = filename_string.c_str();

                TFile *file = new TFile(filename_c);
                TTree *tree = (TTree*)file->Get("treeout");
                event *e = new event();
                tree->SetBranchAddress("e", &e);

                Int_t nentries = (Int_t)tree->GetEntries();

                std::vector<double> q;
                std::vector<double> omega;
                std::vector<int> pdg1;
                std::vector<int> pdg2;
                std::vector<double> p1;
                std::vector<double> p2;
                std::vector<double> kos;
                std::vector<double> kosboundary;
                std::vector<double> weight;

                q.reserve(nentries);
                omega.reserve(nentries);
                pdg1.reserve(nentries);
                pdg2.reserve(nentries);
                p1.reserve(nentries);
                p2.reserve(nentries);
                kos.reserve(nentries);
                kosboundary.reserve(nentries);
                weight.reserve(nentries);

                for(Int_t j = 0; j < nentries; j++)
                {
                tree->GetEntry(j);

                q.push_back(e->qv());
                omega.push_back(e->q0());
                pdg1.push_back(e->out[1].pdg);
                pdg2.push_back(e->out[2].pdg);
                p1.push_back(vec(e->out[1]).length());
                p2.push_back(vec(e->out[2]).length());

                double beta, gamma, Ecm;
                double Efermi = e->in[1].his_fermi;
                double cosine_had =  coshad(vect(e->out[1]), vect(e->out[2]), beta, gamma, Ecm);
                double Pcm = sqrt(Ecm * Ecm - MN2);
                double cosboundary = TMath::Min(1.0, (gamma*Ecm - Efermi - MN)/beta/gamma/Pcm );

                kos.push_back(cosine_had);
                weight.push_back(e->weight);
                kosboundary.push_back(cosboundary);


                //Calculate the percentage of the progress
                        double percentage = static_cast<double>(j+1) / nentries * 100.0;
                        //Calculate the number of progressbar character
                        int progressBarCount = static_cast<int>(percentage / 100 * progressBarWidth );


                        //Print the Progress Bar
                        std::cout << "\t Writing data in new .root file : " << "\t\t" << " [";
                        for(int k = 0; k < progressBarCount; k++)
                        {
                        std::cout << "=" ;
                        }
                        for(int k = progressBarCount; k < progressBarWidth; k++)
                        {
                        std::cout << " ";
                        }
                        std::cout << "] " << static_cast<int>(percentage) << "%" << "\r";
                        std::cout.flush();


                }
                std::cout << std::endl;

                delete tree;
                file->Close();
                delete file;

                for(Int_t n = 0; n < nentries; n++)
                {
                        raw_data.Fill(q.at(n), omega.at(n), p1.at(n), pdg1.at(n), p2.at(n), pdg2.at(n), kos.at(n), kosboundary.at(n), weight.at(n));
                }
                outfile->cd();
                raw_data.Write();
                outfile->Close();
                delete outfile;

                std::cout << "\n File : " << outfilename_string <<  ": Written sucessfully !! \n";
        }

        std::cout << "\n Job Done !! \n Exiting ... \n";



}
void Fittings()
{
        std::cout << "\n\t-----------------------------------------------------------------\n";
        std::cout << "\t\t\t Fitting Procedure !(Same for E_{nu} = {0.5, 1.0, 2.0, 5.0} [GeV] \n\n";
        std::cout << "\t 1. We first load the Valencia samples \n";
        std::cout << "\t\t a. Print the histogram \n";
        std::cout << "\t\t b. Print Total inclusive cross section and pp cross section \n";
        std::cout << "\t\t c. Print number of pp events for 2 Million statistics \n";
        std::cout << "\t 2. Load Nuwro samples \n";
        std::cout << "\t\t a. Print the histogram \n";
        std::cout << "\t\t b. Print no. of pp event \n";
        std::cout << "\t 3. Restrict Valencia phase-space to only those bins where nuwro gives non-zero output \n";
        std::cout << "\t\t a. Print Total inclusive cross section and pp cross section after restrictions \n";
        std::cout << "\t\t b. Print no. of pp events for new total inclusive cross section and pp cross section  for 2 Million statistics \n";
        std::cout << "\t\t c. Scale nuwro histogram by no. of pp events in Valencia code \n";
        std::cout << "\t\t d. Scan for Minima in (Par, n) space by re-weighting. \n";
        std::cout << "\t 4. Produce NuWro phase-space for best fit values and valencia phase-psace in one canvas \n";
        std::cout << "\t-----------------------------------------------------------------\n\n";

        std::vector<double> q;
        std::vector<double> omega;
        std::vector<int> pdg1;
        std::vector<int> pdg2;
        std::vector<double> p1;
        std::vector<double> p2;
        std::vector<double> kos;
        std::vector<double> kosboundary;
        std::vector<double> weight;

        int choice=0;
        char instruction = 'n';
        
        std::cout << "\t-------------------------------------------------\n";
        std::cout << "\t\t\t Step 1. Load Valencia Samples \n";
        std::cout << "\t-------------------------------------------------\n";
        
        std::cout << "\t Available E_{#nu} \n";
        std::cout << "\t (1) 0.5 [GeV] \n";
        std::cout << "\t (2) 1.0 [GeV] \n";
        std::cout << "\t (3) 2.0 [GeV] \n";
        std::cout << "\t (4) 5.0 [GeV] \n";
        std::cout << "\t";
        std::cin >> choice;
        if( (choice > 4) && (choice < 1) )
        {
                std::cerr << "Wrong Choice !! exiting ... \n";
                std::exit(1);
        }
        
        double Pmax = 1800/GeV;
        double E[4] = {0.5, 1.0, 2.0, 5.0};

        switch(choice)
        {
                case 1: Pmax = 1.0;
                        break;
                case 2: Pmax = 1.8;
                        break;
                case 3: Pmax = 2.0;
                        break;
                case 4: Pmax = 2.0;
                        break;
        }
        
        TH2D *valencia_hist = new TH2D("valencia_hist", "", 20, 0, Pmax, 20, 0, Pmax);
        TH2D *valencia_hist_restricted = new TH2D("valencia_hist_restricted", "", 20, 0, Pmax, 20, 0, Pmax);
        double valencia_totalxsec=0;
        Int_t nentries;

        TH2D *nuwro_hist = new TH2D("nuwro", "", 20, 0, Pmax, 20, 0, Pmax);


        Load_Valencia_Samples(valencia_hist, valencia_totalxsec, nentries, choice);
       
        std::cout << "\t Proceed (y/n) ?: ";
        std::cin >> instruction;
        if(instruction == 'n')
        {
                std::exit(0);
        }

        std::cout << "\t-------------------------------------------------\n";
        std::cout << "\t\t\t Step 2. Load Nuwro Samples \n";
        std::cout << "\t-------------------------------------------------\n";
        
        Load_Nuwro_Samples(nuwro_hist, choice, &q, &omega, &p1, &pdg1, &p2, &pdg2, &kos, &kosboundary, &weight);
        std::cout << "\t Proceed (y/n) ?: ";
        std::cin >> instruction;
        if(instruction == 'n')
        {
                std::exit(0);
        }
       

        std::cout << "\t-------------------------------------------------\n";
        std::cout << "\t\t\t Step 2. Restrict Valencia Phase-space \n";
        std::cout << "\t-------------------------------------------------\n";

        Restrict_Phase_space((TH2D*)valencia_hist_restricted, valencia_hist, nuwro_hist, valencia_totalxsec, nentries, choice);

        std::cout << "\t Procced (y/n) ?: ";
        std::cin >> instruction;
        if(instruction == 'n')
        {
                std::exit(0);
        }

        std::cout << "\n\t Normalizing nuwro histogram by valenicia(restricted) histogram ... \n";
        nuwro_hist->Scale(valencia_hist_restricted->Integral()/nuwro_hist->Integral());


        double P=-1;
        int n=1;

        double delta_P = 0.01;
        int delta_n = 1;

        double maximum_P = 1.0;
        int maximum_n = 10;

               int DOF = 0;
        double chisq = 0;

        double optimal_parameters[3] = {0,0, 20000};

        std::ostringstream textfile;
        textfile << "/home/hprasad/nanalysis/" << "E=" << E[choice-1] << "_fitresult.txt";
        std::string textfile_string = textfile.str();

        std::cout << "\n\n\t Writing chi'square / DOF in  the file " << textfile_string << "\n";
        std::ofstream textfile_ptr;
        textfile_ptr.open(textfile_string, std::ios::out);
        while(P <= maximum_P)
        {
                n = 1;
                while(n <= maximum_n)
                {
                        chisq = 0;
                        TH2D *reweighted_hist = new TH2D("reweighted_hist", " ;p_{1} [GeV/c] Leading Proton; p_{2} [GeV/c] SubLeading Proton", nuwro_hist->GetNbinsX(), 0, Pmax, nuwro_hist->GetNbinsY(), 0, Pmax);
                        Fit_2D_histogram(valencia_hist_restricted, reweighted_hist, chisq, DOF, nuwro_hist, &q, &omega, &p1, &pdg1, &p2, &pdg2, &kos, &kosboundary, &weight, P, n);

                        textfile_ptr << P << " " << n << " " << chisq << " " << DOF << " " << chisq/DOF << "\n";
                        textfile_ptr.flush();
                        
                        if (chisq/DOF < optimal_parameters[2])
                        {
                                optimal_parameters[0] = P;
                                optimal_parameters[1] = n;
                                optimal_parameters[2] = chisq/DOF;
                        }
                        delete reweighted_hist;
                        n += delta_n;

                }
                
                P += delta_P;
        }
        textfile_ptr.close();

        std::cout << "\n\n Fit Results written in : " << textfile_string << "\n\n";

        std::cout << "\t-------------------------------------------------\n";
        std::cout << "\t\t\t Optimal Values \n";
        std::cout << "\t-------------------------------------------------\n";
        std::cout << "\t Parameter P = " << "\t\t" << optimal_parameters[0] << "\n";
        std::cout << "\t Parameter n = " << "\t\t" << optimal_parameters[1] << "\n";
        std::cout << "\t chisquare/DOF = " << "\t\t" << optimal_parameters[2] << "\n";



}

int main()
{
  
  unsigned int choice, errorcode=0;
  start:
  std::cout << "(1) Read Root File (Nuwro) \n";
  std::cout << "(2) Read Root File (TNtuple or Histograms/plots) \n";
  std::cout << "(3) Write nuwro root to TNtuples \n";
  std::cout << "(4) Fittings \n";
  std::cin >> choice;
  
  switch(choice)
  { 
    case 1: ReadRoot_Nuwro();
            break;
    case 2: errorcode = ReadRoot();
            break;
    case 3: WriteRoot();
            break;
    case 4: Fittings();
            break;
    default: std::cerr << "Exiting .. \n";
             std::exit(0);
  }

  if(errorcode)
  {
        errorcode = 0; // Reset the error code
        goto start;
  }

  return 0;
}
