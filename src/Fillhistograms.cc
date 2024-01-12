#include "Fillhistograms.h"
#include "mecevent_common.h"

double coshad(vect p1out, vect p2out)
{
	vect Psys = p1out + p2out;									//4-momentum of outgoing nucleons 
	double s = Psys * Psys;										//Total energy 
	vec boost_v = vec(Psys)/Psys.t;									//direction of the boost
	
	vect Psys_CM = Psys.boost(-boost_v);
	vect p1out_prime = p1out.boost(-boost_v);							//Boost back to CM frame nucleon 1
	vect p2out_prime = p2out.boost(-boost_v);							//Boost back to CM frame nucleon 2


	double cosine_had = vec(p1out_prime) * vec(boost_v) / vec(boost_v).length() / vec(p1out_prime).length();

	return cosine_had;
}

double coshad(vect p1out, vect p2out, double &beta, double &gamma, double &E)
{
	vect Psys = p1out + p2out;									//4-momentum of outgoing nucleons 
	double s = Psys * Psys;										//Total energy 
	vec boost_v = vec(Psys)/Psys.t;									//direction of the boost
	
	vect Psys_CM = Psys.boost(-boost_v);
	vect p1out_prime = p1out.boost(-boost_v);							//Boost back to CM frame nucleon 1
	vect p2out_prime = p2out.boost(-boost_v);							//Boost back to CM frame nucleon 2

	E = Psys_CM.t/2.0;										//Energy of outgoing Nucleon in CM frame
	beta = boost_v.length();
	gamma = 1.0/std::sqrt( 1 - beta*beta);
	double cosine_had = vec(p1out_prime) * vec(boost_v) / vec(boost_v).length() / vec(p1out_prime).length();

	return cosine_had;
}

double reweighting_function(double kos, double Par, int n, double kappa)
{
	if (Par > 0)
	{
		return (n + 1)*(1 - Par + Par*(std::pow(std::fabs(kos/kappa), n)) )/(n + 1.0 - n*Par);
	}
	return  (n+1)*(1 + Par*std::pow(std::fabs(kos/kappa), n))/(n + 1 + Par);
	
}



void Fillhist_ct(TH1D *hist, event *ev, bool reweight, double P, int n)
{
  if(!reweight)
  {
    double cosine_hadronic = coshad(vect(ev->out[1]), vect(ev->out[2]));
    hist->Fill(cosine_hadronic);
  } 
  else {
    double Ecm = 0;
    double beta = 0;
    double gamma = 1;
    double cosine_hadronic = coshad(vect(ev->out[1]), vect(ev->out[2]), beta, gamma, Ecm);
    double Pcm = sqrt(Ecm * Ecm - MN2);
    double kappa = TMath::Min( (gamma*Ecm - ev->in[1].his_fermi - MN)/beta/gamma/Pcm, 1.0 );

    hist->Fill(cosine_hadronic, ev->weight*reweighting_function(cosine_hadronic, P, n, kappa)  );  
  }
}

void Fillhist_inclusive(TH2D *hist, event *ev, bool reweight, double P, int n)
{
  if(!reweight)
  {
    hist->Fill(ev->qv()/GeV, ev->q0()/GeV);
  }
  else 
  {
    double Ecm = 0;
    double beta = 0;
    double gamma = 1;
    double cosine_hadronic = coshad(vect(ev->out[1]), vect(ev->out[2]), beta, gamma, Ecm);
    double Pcm = sqrt(Ecm * Ecm - MN2);
    double kappa = TMath::Min( (gamma*Ecm - ev->in[1].his_fermi - MN)/beta/gamma/Pcm, 1.0 );
    hist->Fill(ev->qv()/GeV, ev->q0()/GeV, ev->weight*reweighting_function(cosine_hadronic, P, n, kappa));
  }
}

void Fillhist_exclusive(TH2D *hist, TH2D *hist2, event *ev, bool reweight, double P, int n){
  if(!reweight)
  {
    if (ev->out[1].pdg == 2212 && ev->out[2].pdg == 2212)
    {
      hist->Fill(ev->part_max_mom(2212, false)/GeV, ev->part_sec_mom(2212, false)/GeV);
    }
    if ( (ev->out[1].pdg == 2212 && ev->out[2].pdg == 2112) || (ev->out[1].pdg == 2112 && ev->out[2].pdg == 2212) )
    {
	hist2->Fill(ev->part_max_mom(2212, false)/GeV, ev->part_max_mom(2112, false)/GeV);
    }
  }
  else 
  {
    if(ev->out[1].pdg == 2212 && ev->out[2].pdg == 2212)
    {
      double Ecm = 0;
      double beta = 0;
      double gamma = 1;
      double cosine_hadronic = coshad(vect(ev->out[1]), vect(ev->out[2]), beta, gamma, Ecm);
      double Pcm = sqrt(Ecm * Ecm - MN2);
      double kappa = TMath::Min( (gamma*Ecm - ev->in[1].his_fermi - MN)/beta/gamma/Pcm, 1.0 );
    
      hist->Fill( ev->part_max_mom(2212, false)/GeV, ev->part_sec_mom(2212, false)/GeV,ev->weight*reweighting_function(cosine_hadronic, P, n, kappa));
    }
    hist2->Fill(0.0,0.0);
  }
}


