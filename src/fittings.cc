#include "fittings.h"
#include "Fillhistograms.h"
#include <vector>

void Load_Valencia_Samples(TH2D *real_hist, double &Normalization_1, Int_t &Normalization_2, int choice)
{
  double Pmax = 1.8;  // in GeV
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
    default : std::cerr << "\n\t Wrong Choice !! \n";
              std::exit(1);
  }
  double E[4] = {0.5, 1.0, 2.0, 5.0}; 
  TFile *file = new TFile("data/nucleon_phasespace_valencia.root");

  //Load the histogram along with its cross section;
  std::ostringstream objname;
  objname << "hist" << choice;
  std::string objname_string = objname.str();
  const char *objname_c = objname_string.c_str();
  TH2D *hist = (TH2D*)file->Get(objname_c);
  Int_t nentries = (Int_t)hist->GetEntries();
  TVectorD *cross_sections = (TVectorD*)file->Get("cross_sections");
  double total_xsec = (*cross_sections)(choice -1);
  double pp_xsec = hist->Integral();

  Normalization_1 = total_xsec;
  Normalization_2 = nentries;
  //Save the Loaded histogram before cutting
  //----------------------------------------------------------------------------------
  TCanvas *c1 = new TCanvas("c1");
  gStyle->SetPalette(kSunset);
  hist->SetStats(0);
  hist->SetContour(20);
  hist->Scale(nentries/total_xsec);
  c1->SetRightMargin(0.2);
  hist->Draw("colz");
  
  std::ostringstream plotname;
  plotname << "E=" << E[choice-1] << "_valencia_before.pdf";
  std::string plotname_string = plotname.str();
  const char *plotname_c = plotname_string.c_str();
  c1->SaveAs(plotname_c);
  //----------------------------------------------------------------------------------

  std::cout << "\n\t Nucleon Phase-space saved as : " << "\t\t" << plotname_string << "\n";
  std::cout << "\t Total inclusive cross section = " << "\t\t" << total_xsec * std::pow(10, -38) << "\n";
  std::cout << "\t pp contribution (cross section) = " << "\t\t" << pp_xsec*std::pow(10,-38) << "\n";
  std::cout << "\t Total events : " << nentries << "\n";
  std::cout << "\t pp events : " << hist->Integral() << "\n";
  std::cout << "\t Ratio : " << hist->Integral()/nentries << "\n";

  delete c1;

  
  //Inserting the loaded hist in memory
  //-----------------------------------------------------------------------------------------
  std::ostringstream histtitle;
  histtitle << "#nu_{#mu} #rightarrow ^{12}C_{6} E_{#nu} = " << E[choice -1] << " [GeV]; p_{1} [GeV/c] Leading Proton; p_{2} [GeV/c] SubLeading Proton";
  std::string histtitle_string = histtitle.str();
  const char* histtitle_c = histtitle_string.c_str();

  real_hist->SetTitle(histtitle_c); 
  for(int binx = 1; binx <= real_hist->GetNbinsX(); binx++)
  {
    for(int biny = 1; biny <= real_hist->GetNbinsY(); biny++)
    {
      real_hist->SetBinContent(binx, biny, hist->GetBinContent(binx, biny));
    }
  }
  //-------------------------------------------------------------------------------------------

}

void Load_Nuwro_Samples(TH2D *real_hist, int choice, std::vector<double> *q, std::vector<double> *q0, std::vector<double> *p1out, std::vector<int> *pdg1, std::vector<double> *p2out, std::vector<int> *pdg2, std::vector<double> *kos, std::vector<double> *cosboundary, std::vector<double> *e_weight)
{
    double Pmax = 1.8;
    double E[4] = {0.5, 1.0, 2.0, 5.0};

    std::cout << "\t Loading corresponding Nuwro file ... \n";
    TFile *nuwro_file;
    switch(choice)
    {
      case 1: nuwro_file = new TFile("data/tuple_E=0.5.root");
              Pmax = 1.0;
              break;
      case 2: nuwro_file = new TFile("data/tuple_E=1.0.root");
              Pmax = 1.8;
              break;
      case 3: nuwro_file = new TFile("data/tuple_E=2.0.root");
              Pmax = 2.0;
              break;
      case 4: nuwro_file = new TFile("data/tuple_E=5.0.root");
              Pmax = 2.0;
              break;
      default : std::cerr << "\n\t Choice does not exist !! Wrong Choice !! exiting ... \n";
                break;
    }
    if (nuwro_file->IsZombie())
    {
      std::cerr << "\n\t File not found !! exiting ... \n";
      std::exit(1);
    }
    TH2D *hist = new TH2D("hist", "; p_{1} [GeV/c] Leading Proton; p_{2} [GeV/c] SubLeading Proton", 20, 0, Pmax, 20, 0, Pmax);
    TNtupleD *raw_data = (TNtupleD*)nuwro_file->Get("raw_data");
    Int_t nuwro_entries = (Int_t)raw_data->GetEntries();
    

    double *row_content;
    for(Int_t i = 0; i < raw_data->GetEntries(); i++)
    {
      raw_data->GetEntry(i);
      row_content = (double*)raw_data->GetArgs();
      q0->push_back(row_content[1]);
      q->push_back(row_content[0]);
      pdg1->push_back(row_content[3]);
      pdg2->push_back(row_content[5]);
      p1out->push_back(row_content[2]);
      p2out->push_back(row_content[4]);
      kos->push_back(row_content[6]);
      e_weight->push_back(row_content[8]);
      cosboundary->push_back(row_content[7]);

      if (row_content[3] == 2212 && row_content[5] == 2212)
      {
        if (row_content[2] > row_content[4])
        {
          hist->Fill( row_content[2]/GeV, row_content[4]/GeV);
        }
        else 
        {
          hist->Fill( row_content[4]/GeV, row_content[2]/GeV);
        }
      }
    }

    int DOF = 0;
    for(int binx = 1; binx <= hist->GetNbinsX(); binx++)
    {
      for(int biny = 1; biny <= hist->GetNbinsY(); biny++)
      {
        if (hist->GetBinContent(binx, biny) > 0)
        {
          ++DOF;
        }
      }
    }

    TCanvas *c1 = new TCanvas("c1");
    gStyle->SetPalette(kSunset);
    //hist->SetStats(0);
    hist->SetContour(20);
    c1->SetRightMargin(0.2);
    hist->Draw("colz");

    std::ostringstream plotname;
    plotname << "E=" << E[choice-1] << "_nuwro.pdf";
    std::string plotname_string = plotname.str();
    const char *plotname_c = plotname_string.c_str();
    c1->SaveAs(plotname_c);
    
    std::cout << "\t Total Number of Events : " << "\t\t" << nuwro_entries << "\n";
    std::cout << "\t pp events : " << "\t\t" << hist->Integral() << "\n";
    std::cout << "\t DOF = : " << "\t\t" << DOF << "\n";

  //Inserting the loaded hist in memory
  //---------------------------------------------------------------------------------------------------------------------------------------------------
  std::ostringstream histtitle;
  histtitle << "#nu_{#mu} #rightarrow ^{12}C_{6} E_{#nu} = " << E[choice -1] << " [GeV]; p_{1} [GeV/c] Leading Proton; p_{2} [GeV/c] SubLeading Proton";
  std::string histtitle_string = histtitle.str();
  const char* histtitle_c = histtitle_string.c_str();

  real_hist->SetTitle(histtitle_c);
  for(int binx = 1; binx <= real_hist->GetNbinsX(); binx++)
  {
    for(int biny = 1; biny <= real_hist->GetNbinsY(); biny++)
    {
      real_hist->SetBinContent(binx, biny, hist->GetBinContent(binx, biny));
    }
  }
  //----------------------------------------------------------------------------------------------------------------------------------------------------


}

void Restrict_Phase_space(TH2D *hist, TH2D *sample_hist, TH2D *restrictor, double totalxsec, Int_t nentries, int choice)
{
  double E[4] = {0.5, 1.0, 2.0, 5.0};
  double Pmax = 1800/GeV;

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
    default: std::cerr << "\t Wrong choice !! \n\t exiting...\n";
             std::exit(1);
  }
  std::ostringstream histtitle;
  histtitle << "#nu_{#mu} #rightarrow ^{12}C_{6} E_{#nu} = " << E[choice -1] << " [GeV]; p_{1} [GeV/c] Leading Proton; p_{2} [GeV/c] SubLeading Proton";
  std::string histtitle_string = histtitle.str();
  const char* histtitle_c = histtitle_string.c_str();

  hist->SetTitle(histtitle_c);

  sample_hist->Scale(totalxsec/nentries);

  for(int binx = 1; binx <= restrictor->GetNbinsX(); binx++)
  {
    for(int biny = 1; biny <= restrictor->GetNbinsY(); biny++)
    {
      if ( restrictor->GetBinContent(binx, biny) > 0) 
      {
        hist->SetBinContent(binx, biny, sample_hist->GetBinContent(binx, biny) );
      }
    }
  }


  //Renormalize the histogram
  double ppxsec = hist->Integral();
  sample_hist->Scale(nentries / totalxsec);
  hist->Scale(nentries / totalxsec);

  std::cout << "\t Total number of events : " << "\t\t\t" << nentries << "\n";
  std::cout << "\t Total cross section  : " << "\t\t\t" << totalxsec*std::pow(10,-38) << "[cm^{2}] \n";
  std::cout << "\t pp cross section (restricted) : " << "\t\t\t" << ppxsec*std::pow(10,-38) << "[cm^{2}] \n";
  std::cout << "\t pp events (restricted) : " << "\t\t\t" << hist->Integral() << "\n";

  //Save the Loaded histogram after cutting
  //----------------------------------------------------------------------------------
  TCanvas *c1 = new TCanvas("c1");
  gStyle->SetPalette(kSunset);
  hist->SetStats(0);
  hist->SetContour(20);
  c1->SetRightMargin(0.2);
  hist->Draw("colz");
  
  std::ostringstream plotname;
  plotname << "E=" << E[choice-1] << "_valencia_after.pdf";
  std::string plotname_string = plotname.str();
  const char *plotname_c = plotname_string.c_str();
  c1->SaveAs(plotname_c);
  //----------------------------------------------------------------------------------

}

void Fit_2D_histogram(TH2D *hist, TH2D *reweight_hist, double &chisquare, int &dof, TH2D *org_hist, std::vector<double> *q, std::vector<double> *q0, std::vector<double> *p1out, std::vector<int> *pdg1, std::vector<double> *p2out, std::vector<int> *pdg2, std::vector<double> *kos, std::vector<double> *cosboundary, std::vector<double> *e_weight, double par, int n)
{
  double newtotalweight = 0;

  for(Int_t i = 0 ; i < e_weight->size(); i++)
  {

    double new_weight = e_weight->at(i)*reweighting_function(kos->at(i), par, n, cosboundary->at(i));
    newtotalweight += new_weight;

    if (pdg1->at(i) == 2212 && pdg2->at(i) == 2212)
    {
      if (p1out->at(i) > p2out->at(i))
      {
        reweight_hist->Fill(p1out->at(i)/GeV, p2out->at(i)/GeV, new_weight);
      }
      else 
      {
        reweight_hist->Fill(p2out->at(i)/GeV, p1out->at(i)/GeV, new_weight);
      }
    }


  }

  reweight_hist->Scale(e_weight->size()/newtotalweight);
  reweight_hist->Scale(org_hist->Integral()/reweight_hist->Integral());


  chisquare = 0;
  dof = 0;

  for(int binx = 1; binx <= reweight_hist->GetNbinsX(); binx++)
  {
    for(int biny = 1; biny <= reweight_hist->GetNbinsY(); biny++)
    {
      if (reweight_hist->GetBinContent(binx, biny) > 0) 
      {
        ++dof;
        double N_nuwro = reweight_hist->GetBinContent(binx, biny);
        double N_valencia = hist->GetBinContent(binx, biny);
        chisquare += std::pow(N_nuwro - N_valencia, 2)/0.5/(N_nuwro + N_valencia);
      }
    }
  }

  //std::cout << "\t" << "P = " << par << " " << "n = " << n << " " << "chisquare = " << chisquare << " " << "DOF = " << dof << " " << "chisq (Normalized) = " << chisquare/dof << "\n";

}
