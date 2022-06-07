#include "LEAF/LQDM/include/LQDMPreselectionHists.h"
#include "LEAF/Analyzer/include/constants.h"
#include "LEAF/Analyzer/include/useful_functions.h"
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
#include <TMatrixDSym.h>
#include <TMatrixDSymEigen.h>
#include <TVectorD.h>
#include <Math/Vector2D.h>
#include <Math/Vector3D.h>
#include <iostream>
#include <sys/stat.h>

using namespace std;

LQDMPreselectionHists::LQDMPreselectionHists(TString dir_) : BaseHists(dir_){

  hmetpt = book<TH1D>("metpt", ";p_{T}^{miss} [GeV]; Events / bin", 60, 0, 2000);
  hmetphi = book<TH1D>("metphi", ";#phi(p_{T}^{miss}); Events / bin", 70, -3.5, 3.5);

  hht  = book<TH1D>("ht", ";H_{T} [GeV]; Events / bin", 90, 0, 3000);
  hht_rebin  = book<TH1D>("ht_rebin", ";H_{T} [GeV]; Events / bin", 50, 0, 1000);
  hhtvecpt  = book<TH1D>("htvecpt", ";vectorial H_{T} [GeV]; Events / bin", 90, 0, 3000);
  hhtvecpt_rebin  = book<TH1D>("htvecpt_rebin", ";vectorial H_{T} [GeV]; Events / bin", 50, 0, 1000);
  hhtvecphi = book<TH1D>("htvecphi", ";vectorial #phi(H); Events / bin", 70, -3.5, 3.5);
  hhtveceta = book<TH1D>("htveceta", ";vectorial #eta(H); Events / bin", 100, -5., 5.);
  hdphi_htvec_met = book<TH1D>("dphi_htvec_met", ";|#Delta#Phi(vectorial H, p_{T}^{miss})|; Events / bin", 35, 0, 3.5);
  hptratio_htvec_met = book<TH1D>("ptratio_htvec_met", ";vectorial H_{T} / p_{T}^{miss}; Events / bin", 60, 0, 6);

  hlt  = book<TH1D>("lt", ";L_{T} [GeV]; Events / bin", 90, 0, 3000);
  hlt_rebin  = book<TH1D>("lt_rebin", ";L_{T} [GeV]; Events / bin", 50, 0, 1000);
  hltvecpt  = book<TH1D>("ltvecpt", ";vectorial L_{T} [GeV]; Events / bin", 90, 0, 3000);
  hltvecpt_rebin  = book<TH1D>("ltvecpt_rebin", ";vectorial L_{T} [GeV]; Events / bin", 50, 0, 1000);
  hltvecphi = book<TH1D>("ltvecphi", ";vectorial #phi(L); Events / bin", 70, -3.5, 3.5);
  hltveceta = book<TH1D>("ltveceta", ";vectorial #eta(L); Events / bin", 100, -5., 5.);
  hdphi_ltvec_met = book<TH1D>("dphi_ltvec_met", ";|#Delta#Phi(vectorial L, p_{T}^{miss})|; Events / bin", 35, 0, 3.5);
  hptratio_ltvec_met = book<TH1D>("ptratio_ltvec_met", ";vectorial L_{T} / p_{T}^{miss}; Events / bin", 60, 0, 6);

  hstmet  = book<TH1D>("stmet", ";S_{T}^{MET} [GeV]; Events / bin", 90, 0, 3000);
  hstmet_rebin  = book<TH1D>("stmet_rebin", ";S_{T}^{MET} [GeV]; Events / bin", 50, 0, 1500);
  hstvecpt  = book<TH1D>("stvecpt", ";vectorial S_{T} [GeV]; Events / bin", 90, 0, 3000);
  hstvecpt_rebin  = book<TH1D>("stvecpt_rebin", ";vectorial S_{T} [GeV]; Events / bin", 50, 0, 1000);
  hstvecphi = book<TH1D>("stvecphi", ";vectorial #phi(S); Events / bin", 70, -3.5, 3.5);
  hstveceta = book<TH1D>("stveceta", ";vectorial #eta(S); Events / bin", 100, -5., 5.);
  hdphi_stvec_met = book<TH1D>("dphi_stvec_met", ";|#Delta#Phi(vectorial S, p_{T}^{miss})|; Events / bin", 35, 0, 3.5);
  hptratio_stvec_met = book<TH1D>("ptratio_stvec_met", ";vectorial S_{T} / p_{T}^{miss}; Events / bin", 60, 0, 6);

  hrt = book<TH1D>("rt", ";R_{T}; Events / bin", 24, 0, 1.2);
  hrt_rebin = book<TH1D>("rt_rebin", ";R_{T}; Events / bin", 12, 0, 1.2);
  hrt_rebin2 = book<TH1D>("rt_rebin2", ";R_{T}; Events / bin", 48, 0, 1.2);
  hrt_rebin3 = book<TH1D>("rt_rebin3", ";R_{T}; Events / bin", 96, 0, 1.2);



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
  hdphi_j1_met = book<TH1D>("dphi_j1_met", ";|#Delta#Phi(jet 1, p_{T}^{miss})|; Events / bin", 35, 0, 3.5);
  hptratio_j1_met = book<TH1D>("ptratio_j1_met", ";p_{T}^{jet 1} / p_{T}^{miss}; Events / bin", 60, 0, 6);
  hdphi_j2_met = book<TH1D>("dphi_j2_met", ";|#Delta#Phi(jet 2, p_{T}^{miss})|; Events / bin", 35, 0, 3.5);
  hptratio_j2_met = book<TH1D>("ptratio_j2_met", ";p_{T}^{jet 2} / p_{T}^{miss}; Events / bin", 60, 0, 6);
  hdphi_j3_met = book<TH1D>("dphi_j3_met", ";|#Delta#Phi(jet 3, p_{T}^{miss})|; Events / bin", 35, 0, 3.5);
  hptratio_j3_met = book<TH1D>("ptratio_j3_met", ";p_{T}^{jet 3} / p_{T}^{miss}; Events / bin", 60, 0, 6);

  hptratio_j2_j1 = book<TH1D>("ptratio_j2_j1", ";p_{T}^{jet 2} / p_{T}^{jet 1}; Events / bin", 100, 0, 1);
  hptratio_j3_j1 = book<TH1D>("ptratio_j3_j1", ";p_{T}^{jet 3} / p_{T}^{jet 1}; Events / bin", 100, 0, 1);
  hptratio_j3_j2 = book<TH1D>("ptratio_j3_j2", ";p_{T}^{jet 3} / p_{T}^{jet 2}; Events / bin", 100, 0, 1);

  // Sphericity tensor
  hs11 = book<TH1D>("s11", ";S_{11};Events / bin", 50, 0, 1);
  hs12 = book<TH1D>("s12", ";S_{12};Events / bin", 50, 0, 1);
  hs13 = book<TH1D>("s13", ";S_{13};Events / bin", 50, 0, 1);
  hs22 = book<TH1D>("s22", ";S_{22};Events / bin", 50, 0, 1);
  hs23 = book<TH1D>("s23", ";S_{23};Events / bin", 50, 0, 1);
  hs33 = book<TH1D>("s33", ";S_{33};Events / bin", 50, 0, 1);
  hsphericityeigenvalue1 = book<TH1D>("sphericityeigenvalue1", ";Sphericity, eigenvalue 1;Events / bin", 55, 0, 1.1);
  hsphericityeigenvalue2 = book<TH1D>("sphericityeigenvalue2", ";Sphericity, eigenvalue 2;Events / bin", 55, 0, 1.1);
  hsphericityeigenvalue3 = book<TH1D>("sphericityeigenvalue3", ";Sphericity, eigenvalue 3;Events / bin", 55, 0, 1.1);
  hsphericity = book<TH1D>("sphericity", ";Sphericity;Events / bin", 55, 0, 1.1);
  haplanarity = book<TH1D>("aplanarity", ";Aplanarity;Events / bin", 80, 0, 1.6);

  hproj2d_tau1_met = book<TH1D>("proj2d_tau1_met", ";<p_{T}^{#tau 1}, p_{T}^{miss}> [GeV^{2}];Events / bin", 100, -250, 250);
  hproj2d_mu1_met  = book<TH1D>("proj2d_mu1_met", ";<p_{T}^{#mu 1}, p_{T}^{miss}> [GeV^{2}];Events / bin", 100, -250, 250);
  hproj2d_e1_met   = book<TH1D>("proj2d_e1_met", ";<p_{T}^{e 1}, p_{T}^{miss}> [GeV^{2}];Events / bin", 100, -250, 250);
  hproj2d_j1_met   = book<TH1D>("proj2d_j1_met", ";<p_{T}^{jet 1}, p_{T}^{miss}> [GeV^{2}];Events / bin", 100, -500, 500);
  hproj2d_j2_met   = book<TH1D>("proj2d_j2_met", ";<p_{T}^{jet 2}, p_{T}^{miss}> [GeV^{2}];Events / bin", 100, -500, 500);
  hproj2d_j3_met   = book<TH1D>("proj2d_j3_met", ";<p_{T}^{jet 3}, p_{T}^{miss}> [GeV^{2}];Events / bin", 100, -500, 500);

  hproj2d_tau1_j1 = book<TH1D>("proj2d_tau1_j1", ";<p_{T}^{#tau 1}, p_{T}^{jet 1}> [GeV^{2}];Events / bin", 100, -250, 250);
  hproj2d_mu1_j1  = book<TH1D>("proj2d_mu1_j1", ";<p_{T}^{#mu 1}, p_{T}^{jet 1}> [GeV^{2}];Events / bin", 100, -250, 250);
  hproj2d_e1_j1   = book<TH1D>("proj2d_e1_j1", ";<p_{T}^{e 1}, p_{T}^{jet 1}> [GeV^{2}];Events / bin", 100, -250, 250);
  hproj2d_met_j1  = book<TH1D>("proj2d_met_j1", ";<p_{T}^{miss}, p_{T}^{jet 1}> [GeV^{2}];Events / bin", 100, -500, 500);
  hproj2d_j2_j1   = book<TH1D>("proj2d_jet2_j1", ";<p_{T}^{jet 2}, p_{T}^{jet 1}> [GeV^{2}];Events / bin", 100, -500, 500);
  hproj2d_j3_j1   = book<TH1D>("proj2d_jet3_j1", ";<p_{T}^{jet 3}, p_{T}^{jet 1}> [GeV^{2}];Events / bin", 100, -500, 500);

  hproj3d_tau1_j1  = book<TH1D>("proj3d_tau1_j1", ";<p^{#tau 1}, p^{jet 1}> [GeV^{2}];Events / bin", 100, -250, 250);
  hproj3d_mu1_j1   = book<TH1D>("proj3d_mu1_j1", ";<p^{#mu 1}, p^{jet 1}> [GeV^{2}];Events / bin", 100, -250, 250);
  hproj3d_e1_j1    = book<TH1D>("proj3d_e1_j1", ";<p^{e 1}, p^{jet 1}> [GeV^{2}];Events / bin", 100, -250, 250);
  hproj3d_j2_j1    = book<TH1D>("proj3d_jet2_j1", ";<p^{jet 2}, p^{jet 1}> [GeV^{2}];Events / bin", 100, -500, 500);
  hproj3d_j3_j1    = book<TH1D>("proj3d_jet3_j1", ";<p^{jet 3}, p_{jet 1}> [GeV^{2}];Events / bin", 100, -500, 500);

  hproj2dnorm_tau1_met = book<TH1D>("proj2dnorm_tau1_met", ";norm. <p_{T}^{#tau 1}, p_{T}^{miss}> [GeV^{2}];Events / bin", 100, -1, 1);
  hproj2dnorm_mu1_met  = book<TH1D>("proj2dnorm_mu1_met", ";norm. <p_{T}^{#mu 1}, p_{T}^{miss}> [GeV^{2}];Events / bin", 100, -1, 1);
  hproj2dnorm_e1_met   = book<TH1D>("proj2dnorm_e1_met", ";norm. <p_{T}^{e 1}, p_{T}^{miss}> [GeV^{2}];Events / bin", 100, -1, 1);
  hproj2dnorm_j1_met   = book<TH1D>("proj2dnorm_j1_met", ";norm. <p_{T}^{jet 1}, p_{T}^{miss}> [GeV^{2}];Events / bin", 100, -1, 1);
  hproj2dnorm_j2_met   = book<TH1D>("proj2dnorm_j2_met", ";norm. <p_{T}^{jet 2}, p_{T}^{miss}> [GeV^{2}];Events / bin", 100, -1, 1);
  hproj2dnorm_j3_met   = book<TH1D>("proj2dnorm_j3_met", ";norm. <p_{T}^{jet 3}, p_{T}^{miss}> [GeV^{2}];Events / bin", 100, -1, 1);

  hproj2dnorm_tau1_j1 = book<TH1D>("proj2dnorm_tau1_j1", ";norm. <p_{T}^{#tau 1}, p_{T}^{jet 1}> [GeV^{2}];Events / bin", 100, -1, 1);
  hproj2dnorm_mu1_j1  = book<TH1D>("proj2dnorm_mu1_j1", ";norm. <p_{T}^{#mu 1}, p_{T}^{jet 1}> [GeV^{2}];Events / bin", 100, -1, 1);
  hproj2dnorm_e1_j1   = book<TH1D>("proj2dnorm_e1_j1", ";norm. <p_{T}^{e 1}, p_{T}^{jet 1}> [GeV^{2}];Events / bin", 100, -1, 1);
  hproj2dnorm_met_j1  = book<TH1D>("proj2dnorm_met_j1", ";norm. <p_{T}^{miss}, p_{T}^{jet 1}> [GeV^{2}];Events / bin", 100, -1, 1);
  hproj2dnorm_j2_j1   = book<TH1D>("proj2dnorm_jet2_j1", ";norm. <p_{T}^{jet 2}, p_{T}^{jet 1}> [GeV^{2}];Events / bin", 100, -1, 1);
  hproj2dnorm_j3_j1   = book<TH1D>("proj2dnorm_jet3_j1", ";norm. <p_{T}^{jet 3}, p_{T}^{jet 1}> [GeV^{2}];Events / bin", 100, -1, 1);

  hproj3dnorm_tau1_j1  = book<TH1D>("proj3dnorm_tau1_j1", ";norm. <p^{#tau 1}, p^{jet 1}> [GeV^{2}];Events / bin", 100, -1, 1);
  hproj3dnorm_mu1_j1   = book<TH1D>("proj3dnorm_mu1_j1", ";norm. <p^{#mu 1}, p^{jet 1}> [GeV^{2}];Events / bin", 100, -1, 1);
  hproj3dnorm_e1_j1    = book<TH1D>("proj3dnorm_e1_j1", ";norm. <p^{e 1}, p^{jet 1}> [GeV^{2}];Events / bin", 100, -1, 1);
  hproj3dnorm_j2_j1    = book<TH1D>("proj3dnorm_jet2_j1", ";norm. <p^{jet 2}, p^{jet 1}> [GeV^{2}];Events / bin", 100, -1, 1);
  hproj3dnorm_j3_j1    = book<TH1D>("proj3dnorm_jet3_j1", ";norm. <p^{jet 3}, p_{jet 1}> [GeV^{2}];Events / bin", 100, -1, 1);

  hreco_wmass = book<TH1D>("hreco_wmass", ";reco. W mass [GeV]; Events / bin", 100, 0, 1000);
  hreco_wpt = book<TH1D>("hreco_wpt", ";reco. W p_{T} [GeV]; Events / bin", 40, 0, 400);


  hnpv = book<TH1D>("npv", ";N_{PV}; Events / bin", 101, -0.5, 100.5);
  hnpvgood = book<TH1D>("npvgood", ";N_{good PV}; Events / bin", 101, -0.5, 100.5);
  hsumweights = book<TH1D>("sumweights", ";;Sum of event weights", 1, 0.5, 1.5);




}

void LQDMPreselectionHists::fill(const LQDMEvent & event){
  double weight = event.weight;

  double ht = 0;
  TLorentzVector htvec(0.,0.,0.,0.);
  double lt = 0;
  TLorentzVector ltvec(0.,0.,0.,0.);
  for (Jet & jet : *event.jets_ak4chs){
    ht += jet.pt();
    htvec += jet.p4();
  }
  for (Electron & e : *event.electrons){
    lt += e.pt();
    ltvec += e.p4();
  }
  for (Muon & mu : *event.muons){
    lt += mu.pt();
    ltvec += mu.p4();
  }
  for (Tau & tau : *event.taus){
    lt += tau.pt();
    ltvec += tau.p4();
  }
  TLorentzVector stvec = htvec + ltvec;
  double stmet = event.met->pt() + ht + lt;

  hmetpt->Fill(event.met->pt(), weight);
  hmetphi->Fill(event.met->phi(), weight);

  hht->Fill(ht, weight);
  hht_rebin->Fill(ht, weight);
  hhtvecpt->Fill(htvec.Pt(), weight);
  hhtvecpt_rebin->Fill(htvec.Pt(), weight);
  hhtvecphi->Fill(htvec.Phi(), weight);
  hhtveceta->Fill(htvec.Eta(), weight);
  Particle dummyparticle;
  dummyparticle.set_p4(htvec.Pt(), htvec.Eta(), htvec.Phi(), htvec.M());
  hdphi_htvec_met->Fill(deltaPhi(dummyparticle, *event.met), weight);
  hptratio_htvec_met->Fill(htvec.Pt() / event.met->pt(), weight);

  hlt->Fill(lt, weight);
  hlt_rebin->Fill(lt, weight);
  hltvecpt->Fill(ltvec.Pt(), weight);
  hltvecpt_rebin->Fill(ltvec.Pt(), weight);
  hltvecphi->Fill(ltvec.Phi(), weight);
  hltveceta->Fill(ltvec.Eta(), weight);
  dummyparticle.set_p4(ltvec.Pt(), ltvec.Eta(), ltvec.Phi(), ltvec.M());
  hdphi_ltvec_met->Fill(deltaPhi(dummyparticle, *event.met), weight);
  hptratio_ltvec_met->Fill(ltvec.Pt() / event.met->pt(), weight);

  hstmet->Fill(stmet, weight);
  hstmet_rebin->Fill(stmet, weight);
  hstvecpt->Fill(stvec.Pt(), weight);
  hstvecpt_rebin->Fill(stvec.Pt(), weight);
  hstvecphi->Fill(stvec.Phi(), weight);
  hstveceta->Fill(stvec.Eta(), weight);
  dummyparticle.set_p4(stvec.Pt(), stvec.Eta(), stvec.Phi(), stvec.M());
  hdphi_stvec_met->Fill(deltaPhi(dummyparticle, *event.met), weight);
  hptratio_stvec_met->Fill(stvec.Pt() / event.met->pt(), weight);



  double rt = event.met->pt() / stmet;
  hrt->Fill(rt, weight);
  hrt_rebin->Fill(rt, weight);
  hrt_rebin2->Fill(rt, weight);
  hrt_rebin3->Fill(rt, weight);

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
  if(event.jets_ak4chs->size() > 0){
    Jet j = event.jets_ak4chs->at(0);
    hdphi_j1_met->Fill(deltaPhi(j, *event.met), weight);
    hptratio_j1_met->Fill(j.pt() / event.met->pt(), weight);
  }

  if(event.jets_ak4chs->size() > 1){
    Jet j = event.jets_ak4chs->at(1);
    hdphi_j2_met->Fill(deltaPhi(j, *event.met), weight);
    hptratio_j2_met->Fill(j.pt() / event.met->pt(), weight);
    hptratio_j2_j1->Fill(event.jets_ak4chs->at(1).pt() / event.jets_ak4chs->at(0).pt(), weight);
  }

  if(event.jets_ak4chs->size() > 2){
    Jet j = event.jets_ak4chs->at(2);
    hdphi_j3_met->Fill(deltaPhi(j, *event.met), weight);
    hptratio_j3_met->Fill(j.pt() / event.met->pt(), weight);
    hptratio_j3_j1->Fill(event.jets_ak4chs->at(2).pt() / event.jets_ak4chs->at(0).pt(), weight);
    hptratio_j3_j2->Fill(event.jets_ak4chs->at(2).pt() / event.jets_ak4chs->at(1).pt(), weight);
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

  // Sphericity
  double s11 = -1., s12 = -1., s13 = -1., s22 = -1., s23 = -1., s33 = -1., mag = -1.;
  for(const Jet jet : *event.jets_ak4chs){
    mag += (jet.p4().Px()*jet.p4().Px()+jet.p4().Py()*jet.p4().Py()+jet.p4().Pz()*jet.p4().Pz());
    s11 += jet.p4().Px()*jet.p4().Px();
    s12 += jet.p4().Px()*jet.p4().Py();
    s13 += jet.p4().Px()*jet.p4().Pz();
    s22 += jet.p4().Py()*jet.p4().Py();
    s23 += jet.p4().Py()*jet.p4().Pz();
    s33 += jet.p4().Pz()*jet.p4().Pz();
  }
  for(const Muon mu : *event.muons){
    mag += (mu.p4().Px()*mu.p4().Px()+mu.p4().Py()*mu.p4().Py()+mu.p4().Pz()*mu.p4().Pz());
    s11 += mu.p4().Px()*mu.p4().Px();
    s12 += mu.p4().Px()*mu.p4().Py();
    s13 += mu.p4().Px()*mu.p4().Pz();
    s22 += mu.p4().Py()*mu.p4().Py();
    s23 += mu.p4().Py()*mu.p4().Pz();
    s33 += mu.p4().Pz()*mu.p4().Pz();
  }
  for(const Electron ele : *event.electrons){
    mag += (ele.p4().Px()*ele.p4().Px()+ele.p4().Py()*ele.p4().Py()+ele.p4().Pz()*ele.p4().Pz());
    s11 += ele.p4().Px()*ele.p4().Px();
    s12 += ele.p4().Px()*ele.p4().Py();
    s13 += ele.p4().Px()*ele.p4().Pz();
    s22 += ele.p4().Py()*ele.p4().Py();
    s23 += ele.p4().Py()*ele.p4().Pz();
    s33 += ele.p4().Pz()*ele.p4().Pz();
  }
  for(const Tau tau : *event.taus){
    mag += (tau.p4().Px()*tau.p4().Px()+tau.p4().Py()*tau.p4().Py()+tau.p4().Pz()*tau.p4().Pz());
    s11 += tau.p4().Px()*tau.p4().Px();
    s12 += tau.p4().Px()*tau.p4().Py();
    s13 += tau.p4().Px()*tau.p4().Pz();
    s22 += tau.p4().Py()*tau.p4().Py();
    s23 += tau.p4().Py()*tau.p4().Pz();
    s33 += tau.p4().Pz()*tau.p4().Pz();
  }

  s11 = s11 / mag;
  s12 = s12 / mag;
  s13 = s13 / mag;
  s22 = s22 / mag;
  s23 = s23 / mag;
  s33 = s33 / mag;

  vector<double> S_elements = {s11, s12, s13, s12, s22, s23, s13, s23, s33};
  TMatrixDSymEigen S = TMatrixDSymEigen(TMatrixDSym(3, &S_elements[0]));
  TVectorD S_eigenvalues = S.GetEigenValues();
  if(S_eigenvalues[0] >= S_eigenvalues[1] && S_eigenvalues[0] >= S_eigenvalues[2]){
    hsphericity->Fill(1.5*(S_eigenvalues[1] + S_eigenvalues[2]), weight);
    haplanarity->Fill(1.5*S_eigenvalues[0], weight);
    hsphericityeigenvalue1->Fill(S_eigenvalues[0], weight);
    if(S_eigenvalues[1] >= S_eigenvalues[2]){
      hsphericityeigenvalue2->Fill(S_eigenvalues[1], weight);
      hsphericityeigenvalue3->Fill(S_eigenvalues[2], weight);
    }
    else{
      hsphericityeigenvalue2->Fill(S_eigenvalues[2], weight);
      hsphericityeigenvalue3->Fill(S_eigenvalues[1], weight);
    }
  }
  else if(S_eigenvalues[1] >= S_eigenvalues[0] && S_eigenvalues[1] >= S_eigenvalues[2]){
    hsphericity->Fill(1.5*(S_eigenvalues[0] + S_eigenvalues[2]), weight);
    haplanarity->Fill(1.5*S_eigenvalues[1], weight);
    hsphericityeigenvalue1->Fill(S_eigenvalues[1], weight);
    if(S_eigenvalues[0] >= S_eigenvalues[2]){
      hsphericityeigenvalue2->Fill(S_eigenvalues[0], weight);
      hsphericityeigenvalue3->Fill(S_eigenvalues[2], weight);
    }
    else{
      hsphericityeigenvalue2->Fill(S_eigenvalues[2], weight);
      hsphericityeigenvalue3->Fill(S_eigenvalues[0], weight);
    }
  }
  else if(S_eigenvalues[2] >= S_eigenvalues[0] && S_eigenvalues[2] >= S_eigenvalues[1]){
    hsphericity->Fill(1.5*(S_eigenvalues[0] + S_eigenvalues[1]), weight);
    haplanarity->Fill(1.5*S_eigenvalues[2], weight);
    hsphericityeigenvalue1->Fill(S_eigenvalues[2], weight);
    if(S_eigenvalues[1] >= S_eigenvalues[2]){
      hsphericityeigenvalue2->Fill(S_eigenvalues[1], weight);
      hsphericityeigenvalue3->Fill(S_eigenvalues[2], weight);
    }
    else{
      hsphericityeigenvalue2->Fill(S_eigenvalues[2], weight);
      hsphericityeigenvalue3->Fill(S_eigenvalues[1], weight);
    }
  }

  hs11->Fill(s11, weight);
  hs12->Fill(s12, weight);
  hs13->Fill(s13, weight);
  hs22->Fill(s22, weight);
  hs23->Fill(s23, weight);
  hs33->Fill(s33, weight);

  ROOT::Math::XYVector met_p2(0,0), tau1_p2(0,0), mu1_p2(0,0), e1_p2(0,0), j1_p2(0,0), j2_p2(0,0), j3_p2(0,0);
  ROOT::Math::XYZVector tau1_p3(0,0,0), mu1_p3(0,0,0), e1_p3(0,0,0), j1_p3(0,0,0), j2_p3(0,0,0), j3_p3(0,0,0);
  met_p2 = ROOT::Math::XYVector(event.met->p4().Px(), event.met->p4().Py());
  if(event.taus->size() > 0){
    tau1_p2 = ROOT::Math::XYVector(event.taus->at(0).p4().Px(), event.taus->at(0).p4().Py());
    tau1_p3 = ROOT::Math::XYZVector(event.taus->at(0).p4().Px(), event.taus->at(0).p4().Py(), event.taus->at(0).p4().Pz());
  }
  if(event.muons->size() > 0){
    mu1_p2 = ROOT::Math::XYVector(event.muons->at(0).p4().Px(), event.muons->at(0).p4().Py());
    mu1_p3 = ROOT::Math::XYZVector(event.muons->at(0).p4().Px(), event.muons->at(0).p4().Py(), event.muons->at(0).p4().Pz());
  }
  if(event.electrons->size() > 0){
    e1_p2 = ROOT::Math::XYVector(event.electrons->at(0).p4().Px(), event.electrons->at(0).p4().Py());
    e1_p3 = ROOT::Math::XYZVector(event.electrons->at(0).p4().Px(), event.electrons->at(0).p4().Py(), event.electrons->at(0).p4().Pz());
  }
  if(event.jets_ak4chs->size() > 0){
    j1_p2 = ROOT::Math::XYVector(event.jets_ak4chs->at(0).p4().Px(), event.jets_ak4chs->at(0).p4().Py());
    j1_p3 = ROOT::Math::XYZVector(event.jets_ak4chs->at(0).p4().Px(), event.jets_ak4chs->at(0).p4().Py(), event.jets_ak4chs->at(0).p4().Pz());

    if(event.jets_ak4chs->size() > 1){
      j2_p2 = ROOT::Math::XYVector(event.jets_ak4chs->at(1).p4().Px(), event.jets_ak4chs->at(1).p4().Py());
      j2_p3 = ROOT::Math::XYZVector(event.jets_ak4chs->at(1).p4().Px(), event.jets_ak4chs->at(1).p4().Py(), event.jets_ak4chs->at(1).p4().Pz());

      if(event.jets_ak4chs->size() > 2){
        j3_p2 = ROOT::Math::XYVector(event.jets_ak4chs->at(2).p4().Px(), event.jets_ak4chs->at(2).p4().Py());
        j3_p3 = ROOT::Math::XYZVector(event.jets_ak4chs->at(2).p4().Px(), event.jets_ak4chs->at(2).p4().Py(), event.jets_ak4chs->at(2).p4().Pz());
      }
    }
  }

  if(event.taus->size() > 0)        hproj2d_tau1_met->Fill( tau1_p2.Dot(met_p2.Unit()), weight);
  if(event.muons->size() > 0)       hproj2d_mu1_met->Fill(   mu1_p2.Dot(met_p2.Unit()), weight);
  if(event.electrons->size() > 0)   hproj2d_e1_met->Fill(     e1_p2.Dot(met_p2.Unit()), weight);
  if(event.jets_ak4chs->size() > 0) hproj2d_j1_met->Fill(     j1_p2.Dot(met_p2.Unit()), weight);
  if(event.jets_ak4chs->size() > 1) hproj2d_j2_met->Fill(     j2_p2.Dot(met_p2.Unit()), weight);
  if(event.jets_ak4chs->size() > 2) hproj2d_j3_met->Fill(     j3_p2.Dot(met_p2.Unit()), weight);

  if(event.taus->size() > 0)        hproj2dnorm_tau1_met->Fill( tau1_p2.Unit().Dot(met_p2.Unit()), weight);
  if(event.muons->size() > 0)       hproj2dnorm_mu1_met->Fill(   mu1_p2.Unit().Dot(met_p2.Unit()), weight);
  if(event.electrons->size() > 0)   hproj2dnorm_e1_met->Fill(     e1_p2.Unit().Dot(met_p2.Unit()), weight);
  if(event.jets_ak4chs->size() > 0) hproj2dnorm_j1_met->Fill(     j1_p2.Unit().Dot(met_p2.Unit()), weight);
  if(event.jets_ak4chs->size() > 1) hproj2dnorm_j2_met->Fill(     j2_p2.Unit().Dot(met_p2.Unit()), weight);
  if(event.jets_ak4chs->size() > 2) hproj2dnorm_j3_met->Fill(     j3_p2.Unit().Dot(met_p2.Unit()), weight);


  if(event.jets_ak4chs->size() > 0){
    hproj2dnorm_met_j1->Fill(   met_p2.Unit().Dot(j1_p2.Unit()), weight);
    if(event.taus->size() > 0)        hproj2dnorm_tau1_j1->Fill( tau1_p2.Unit().Dot(j1_p2.Unit()), weight);
    if(event.muons->size() > 0)       hproj2dnorm_mu1_j1->Fill(   mu1_p2.Unit().Dot(j1_p2.Unit()), weight);
    if(event.electrons->size() > 0)   hproj2dnorm_e1_j1->Fill(     e1_p2.Unit().Dot(j1_p2.Unit()), weight);
    if(event.jets_ak4chs->size() > 1) hproj2dnorm_j2_j1->Fill(     j2_p2.Unit().Dot(j1_p2.Unit()), weight);
    if(event.jets_ak4chs->size() > 2) hproj2dnorm_j3_j1->Fill(     j3_p2.Unit().Dot(j1_p2.Unit()), weight);

    if(event.taus->size() > 0)        hproj3dnorm_tau1_j1->Fill( tau1_p3.Unit().Dot(j1_p3.Unit()), weight);
    if(event.muons->size() > 0)       hproj3dnorm_mu1_j1->Fill(   mu1_p3.Unit().Dot(j1_p3.Unit()), weight);
    if(event.electrons->size() > 0)   hproj3dnorm_e1_j1->Fill(     e1_p3.Unit().Dot(j1_p3.Unit()), weight);
    if(event.jets_ak4chs->size() > 1) hproj3dnorm_j2_j1->Fill(     j2_p3.Unit().Dot(j1_p3.Unit()), weight);
    if(event.jets_ak4chs->size() > 2) hproj3dnorm_j3_j1->Fill(     j3_p3.Unit().Dot(j1_p3.Unit()), weight);

    hproj2d_met_j1->Fill(   met_p2.Dot(j1_p2.Unit()), weight);
    if(event.taus->size() > 0)        hproj2d_tau1_j1->Fill( tau1_p2.Dot(j1_p2.Unit()), weight);
    if(event.muons->size() > 0)       hproj2d_mu1_j1->Fill(   mu1_p2.Dot(j1_p2.Unit()), weight);
    if(event.electrons->size() > 0)   hproj2d_e1_j1->Fill(     e1_p2.Dot(j1_p2.Unit()), weight);
    if(event.jets_ak4chs->size() > 1) hproj2d_j2_j1->Fill(     j2_p2.Dot(j1_p2.Unit()), weight);
    if(event.jets_ak4chs->size() > 2) hproj2d_j3_j1->Fill(     j3_p2.Dot(j1_p2.Unit()), weight);

    if(event.taus->size() > 0)        hproj3d_tau1_j1->Fill( tau1_p3.Dot(j1_p3.Unit()), weight);
    if(event.muons->size() > 0)       hproj3d_mu1_j1->Fill(   mu1_p3.Dot(j1_p3.Unit()), weight);
    if(event.electrons->size() > 0)   hproj3d_e1_j1->Fill(     e1_p3.Dot(j1_p3.Unit()), weight);
    if(event.jets_ak4chs->size() > 1) hproj3d_j2_j1->Fill(     j2_p3.Dot(j1_p3.Unit()), weight);
    if(event.jets_ak4chs->size() > 2) hproj3d_j3_j1->Fill(     j3_p3.Dot(j1_p3.Unit()), weight);
  }

  hnpv->Fill(event.npv, weight);
  hnpvgood->Fill(event.npv_good, weight);
  hsumweights->Fill(1, weight);
}
