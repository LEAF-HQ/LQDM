#include "Analyzer/LQDM/include/LQDMPreselectionHists.h"
#include "Analyzer/include/constants.h"
#include "Analyzer/include/useful_functions.h"
#include <TH1F.h>
#include <TFile.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLine.h>
#include <TStyle.h>
#include <TKey.h>
#include <TTree.h>
#include <TLatex.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <iostream>
#include <sys/stat.h>

using namespace std;

LQDMPreselectionHists::LQDMPreselectionHists(TString dir_) : BaseHists(dir_){

  hmetpt = book<TH1D>("metpt", ";p_{T}^{miss} [GeV]; Events / bin", 60, 0, 2000);
  hmetphi = book<TH1D>("metphi", ";#phi(p_{T}^{miss}); Events / bin", 70, -3.5, 3.5);
  hht  = book<TH1D>("ht", ";H_{T} [GeV]; Events / bin", 90, 0, 3000);
  hht_rebin  = book<TH1D>("ht_rebin", ";H_{T} [GeV]; Events / bin", 50, 0, 1000);
  hlt  = book<TH1D>("lt", ";L_{T} [GeV]; Events / bin", 90, 0, 3000);
  hlt_rebin  = book<TH1D>("lt_rebin", ";L_{T} [GeV]; Events / bin", 50, 0, 1000);
  hstmet  = book<TH1D>("stmet", ";S_{T}^{MET} [GeV]; Events / bin", 90, 0, 3000);
  hstmet_rebin  = book<TH1D>("stmet_rebin", ";S_{T}^{MET} [GeV]; Events / bin", 50, 0, 1500);
  hmt_taumet  = book<TH1D>("mt_taumet", ";M_{T}(#tau, MET) [GeV]; Events / bin", 90, 0, 3000);
  hmt_taumet_rebin  = book<TH1D>("mt_taumet_rebin", ";M_{T}(#tau, MET) [GeV]; Events / bin", 50, 0, 1500);

  bins2 = {200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2200, 2400, 2700, 3000};
  hmt_taumet_rebinlimit  = book<TH1D>("mt_taumet_rebinlimit", ";M_{T}(#tau, MET) [GeV]; Events / bin", bins2.size()-1, bins2.data());

  bins3 = {200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000, 1200, 1400, 3000};
  hmt_taumet_rebinlimit2  = book<TH1D>("mt_taumet_rebinlimit2", ";M_{T}(#tau, MET) [GeV]; Events / bin", bins3.size()-1, bins3.data());


  hmt_mumet  = book<TH1D>("mt_mumet", ";M_{T}(#mu, MET) [GeV]; Events / bin", 90, 0, 3000);
  hmt_mumet_rebin  = book<TH1D>("mt_mumet_rebin", ";M_{T}(#mu, MET) [GeV]; Events / bin", 50, 0, 1500);
  hmt_mumet_rebinlimit  = book<TH1D>("mt_mumet_rebinlimit", ";M_{T}(#mu, MET) [GeV]; Events / bin", bins2.size()-1, bins2.data());
  hmt_mumet_rebinlimit2  = book<TH1D>("mt_mumet_rebinlimit2", ";M_{T}(#mu, MET) [GeV]; Events / bin", bins3.size()-1, bins3.data());

  hmt_emet  = book<TH1D>("mt_emet", ";M_{T}(e, MET) [GeV]; Events / bin", 90, 0, 3000);
  hmt_emet_rebin  = book<TH1D>("mt_emet_rebin", ";M_{T}(e, MET) [GeV]; Events / bin", 50, 0, 1500);
  hmt_emet_rebinlimit  = book<TH1D>("mt_emet_rebinlimit", ";M_{T}(e, MET) [GeV]; Events / bin", bins2.size()-1, bins2.data());
  hmt_emet_rebinlimit2  = book<TH1D>("mt_emet_rebinlimit2", ";M_{T}(e, MET) [GeV]; Events / bin", bins3.size()-1, bins3.data());

  hmemu   = book<TH1D>("memu", ";M_{e#mu} [GeV]; Events / bin", 50, 0, 500);
  hmetau  = book<TH1D>("metau", ";M_{e#tau} [GeV]; Events / bin", 50, 0, 500);
  hmmutau = book<TH1D>("mmutau", ";M_{#mu#tau} [GeV]; Events / bin", 50, 0, 500);
  hmtautau = book<TH1D>("mtautau", ";M_{#tau#tau} [GeV]; Events / bin", 50, 0, 500);

  hdphi_tau1_met = book<TH1D>("dphi_tau1_met", ";|#Delta#Phi(#tau 1, p_{T}^{miss})|; Events / bin", 35, 0, 3.5);
  hptratio_tau1_met = book<TH1D>("ptratio_tau1_met", ";p_{T}^{#tau 1} / p_{T}^{miss}; Events / bin", 60, 0, 6);
  hdphi_mu1_met = book<TH1D>("dphi_mu1_met", ";|#Delta#Phi(#mu 1, p_{T}^{miss})|; Events / bin", 35, 0, 3.5);
  hptratio_mu1_met = book<TH1D>("ptratio_mu1_met", ";p_{T}^{#mu 1} / p_{T}^{miss}; Events / bin", 60, 0, 6);
  hdphi_e1_met = book<TH1D>("dphi_e1_met", ";|#Delta#Phi(e 1, p_{T}^{miss})|; Events / bin", 35, 0, 3.5);
  hptratio_e1_met = book<TH1D>("ptratio_e1_met", ";p_{T}^{e 1} / p_{T}^{miss}; Events / bin", 60, 0, 6);

  hreco_wmass = book<TH1D>("hreco_wmass", ";reco. W mass [GeV]; Events / bin", 100, 0, 1000);
  hreco_wpt = book<TH1D>("hreco_wpt", ";reco. W p_{T} [GeV]; Events / bin", 40, 0, 400);


  hnpv = book<TH1D>("npv", ";N_{PV}; Events / bin", 101, -0.5, 100.5);
  hnpvgood = book<TH1D>("npvgood", ";N_{good PV}; Events / bin", 101, -0.5, 100.5);
  hsumweights = book<TH1D>("sumweights", ";;Sum of event weights", 1, 0.5, 1.5);


  //    //


}

void LQDMPreselectionHists::fill(const LQDMEvent & event){
  double weight = event.weight;

  double ht = 0;
  double lt = 0;
  for (Jet & jet : *event.jets) ht += jet.pt();
  for (Electron & e : *event.electrons) lt += e.pt();
  for (Muon & mu : *event.muons) lt += mu.pt();
  for (Tau & tau : *event.taus) lt += tau.pt();
  double stmet = event.met->pt() + ht + lt;

  hmetpt->Fill(event.met->pt(), weight);
  hmetphi->Fill(event.met->phi(), weight);

  hht->Fill(ht, weight);
  hht_rebin->Fill(ht, weight);

  hlt->Fill(lt, weight);
  hlt_rebin->Fill(lt, weight);

  hstmet->Fill(stmet, weight);
  hstmet_rebin->Fill(stmet, weight);

  if(event.taus->size() > 0){
    Tau t = event.taus->at(0);
    double ET_tau = t.e() * t.pt()/TMath::Sqrt(t.e()*t.e()-t.m()*t.m());
    double mt_taumet = sqrt(2*event.met->pt() * ET_tau * (1-cos(deltaPhi(*event.met, t))));

    hmt_taumet->Fill(mt_taumet, weight);
    hmt_taumet_rebin->Fill(mt_taumet, weight);

    double fillval = mt_taumet;
    if(mt_taumet >= bins2[bins2.size()-1]) fillval = bins2[bins2.size()-1] - 0.01;
    hmt_taumet_rebinlimit->Fill(fillval, weight);

    double fillval2 = mt_taumet;
    if(mt_taumet >= bins3[bins3.size()-1]) fillval2 = bins3[bins3.size()-1] - 0.01;
    hmt_taumet_rebinlimit2->Fill(fillval2, weight);

    hdphi_tau1_met->Fill(deltaPhi(t, *event.met), weight);
    hptratio_tau1_met->Fill(t.pt() / event.met->pt(), weight);
  }

  if(event.muons->size() > 0){
    Muon m = event.muons->at(0);
    double ET_mu = m.e() * m.pt()/TMath::Sqrt(m.e()*m.e()-m.m()*m.m());
    double mt_mumet = sqrt(2*event.met->pt() * ET_mu * (1-cos(deltaPhi(*event.met, m))));

    hmt_mumet->Fill(mt_mumet, weight);
    hmt_mumet_rebin->Fill(mt_mumet, weight);

    double fillval = mt_mumet;
    if(mt_mumet >= bins2[bins2.size()-1]) fillval = bins2[bins2.size()-1] - 0.01;
    hmt_mumet_rebinlimit->Fill(fillval, weight);

    double fillval2 = mt_mumet;
    if(mt_mumet >= bins3[bins3.size()-1]) fillval2 = bins3[bins3.size()-1] - 0.01;
    hmt_mumet_rebinlimit2->Fill(fillval2, weight);

    hdphi_mu1_met->Fill(deltaPhi(m, *event.met), weight);
    hptratio_mu1_met->Fill(m.pt() / event.met->pt(), weight);
  }

  if(event.electrons->size() > 0){
    Electron e = event.electrons->at(0);
    double ET_e = e.e() * e.pt()/TMath::Sqrt(e.e()*e.e()-e.m()*e.m());
    double mt_emet = sqrt(2*event.met->pt() * ET_e * (1-cos(deltaPhi(*event.met, e))));

    hmt_emet->Fill(mt_emet, weight);
    hmt_emet_rebin->Fill(mt_emet, weight);

    double fillval = mt_emet;
    if(mt_emet >= bins2[bins2.size()-1]) fillval = bins2[bins2.size()-1] - 0.01;
    hmt_emet_rebinlimit->Fill(fillval, weight);

    double fillval2 = mt_emet;
    if(mt_emet >= bins3[bins3.size()-1]) fillval2 = bins3[bins3.size()-1] - 0.01;
    hmt_emet_rebinlimit2->Fill(fillval2, weight);

    hdphi_e1_met->Fill(deltaPhi(e, *event.met), weight);
    hptratio_e1_met->Fill(e.pt() / event.met->pt(), weight);
  }

  for (Electron & e : *event.electrons){
    for (Muon & mu : *event.muons){
      hmemu->Fill((e.p4() + mu.p4()).M(), weight);
    }
    for (Tau & tau : *event.taus){
      hmetau->Fill((e.p4() + tau.p4()).M(), weight);
    }
  }

  for (Muon & mu : *event.muons){
    for (Tau & tau : *event.taus){
      hmmutau->Fill((mu.p4() + tau.p4()).M(), weight);
    }
  }


  for(size_t i=0; i<event.taus->size(); i++){
    for(size_t j=0; j<event.taus->size(); j++){
      if(j <= i) continue;
      hmtautau->Fill((event.taus->at(i).p4() + event.taus->at(j).p4()).M(), weight);
    }
  }

  if(event.muons->size() > 0){
    vector<TLorentzVector> solutions = NeutrinoReconstruction(event.muons->at(0).p4(), event.met->p4());
    if(solutions.size() > 0){
      hreco_wmass->Fill((solutions[0] + event.muons->at(0).p4()).M(), weight);
      hreco_wpt->Fill((solutions[0] + event.muons->at(0).p4()).Pt(), weight);
    }
  }



  hnpv->Fill(event.npv, weight);
  hnpvgood->Fill(event.npv_good, weight);
  hsumweights->Fill(1, weight);
}
