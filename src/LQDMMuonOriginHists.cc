#include "LEAF/LQDM/include/LQDMMuonOriginHists.h"
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
#include <iostream>
#include <sys/stat.h>

using namespace std;

LQDMMuonOriginHists::LQDMMuonOriginHists(TString dir_) : BaseHists(dir_){


  hmuongenorigin = book<TH1D>("muongenorigin", ";#mu -- 1: from hard #tau, 2: from hadron, 0: else, -1: unmatched; Events / bin", 4, -1.5, 2.5);
  hmuonclosestorigin = book<TH1D>("muonclosestorigin", ";closest #mu -- 1: from hard #tau, 2: from hadron, 0: else, -1: unmatched; Events / bin", 4, -1.5, 2.5);


  hnmuonsfromtau = book<TH1D>("nmuonsfromtau", ";N_{#mu from #tau}; Events / bin", 11, -0.5, 10.5);
  hmuonfromtaupt = book<TH1D>("muonfromtaupt", ";p_{T}^{#mu from #tau} [GeV]; Events / bin", 50, 0, 1500);
  hmuonfromtaupt_rebin = book<TH1D>("muonfromtaupt_rebin", ";p_{T}^{#mu from #tau} [GeV]; Events / bin", 50, 0, 200);
  hmuonfromtaupt_rebin2 = book<TH1D>("muonfromtaupt_rebin2", ";p_{T}^{#mu from #tau} [GeV]; Events / bin", 50, 0, 50);
  hmuonfromtaueta = book<TH1D>("muonfromtaueta", ";#eta^{#mu from #tau};Events / bin", 100, -5., 5.);
  hmuonfromtauphi = book<TH1D>("muonfromtauphi", ";#phi^{#mu from #tau};Events / bin", 70, -3.5, 3.5);
  hmuonfromtaumass = book<TH1D>("muonfromtaumass", ";m^{#mu from #tau} [GeV];Events / bin", 50, 0, 1500);
  hmuonfromtauenergy = book<TH1D>("muonfromtauenergy", ";E^{#mu from #tau} [GeV];Events / bin", 50, 0, 1500);
  hmuonfromtaupfiso = book<TH1D>("muonfromtaupfiso", ";Iso_{PF}^{#mu from #tau};Events / bin", 100, 0, 1);
  hmuonfromtaudxy = book<TH1D>("muonfromtaudxy", ";#mu from #tau d_{xy} wrt. PV [cm];Events / bin", 250, 0, 0.5);
  hmuonfromtaudz = book<TH1D>("muonfromtaudz", ";#mu from #tau d_{z} wrt. PV [cm];Events / bin", 250, 0, 0.5);
  hmuonfromtaud = book<TH1D>("muonfromtaud", ";#mu from #tau d wrt. PV [cm];Events / bin", 250, 0, 0.5);
  hmuonfromtau_dxy_vs_dz = book<TH2D>("muonfromtau_dxy_vs_dz", ";#mu from #tau d_{xy} wrt. PV [cm];#mu from #tau d_{z} wrt. PV [cm];Events / bin", 250, 0, 0.5, 250, 0, 0.5);
  hmuonfromtaugendrmin = book<TH1D>("muonfromtaugendrmin", ";#DeltaR(#mu from #tau, closest gen #mu);Events / bin", 60, 0, 3);

  hnmuonsfromhad = book<TH1D>("nmuonsfromhad", ";N_{#mu from had.}; Events / bin", 11, -0.5, 10.5);
  hmuonfromhadpt = book<TH1D>("muonfromhadpt", ";p_{T}^{#mu from had.} [GeV]; Events / bin", 50, 0, 1500);
  hmuonfromhadpt_rebin = book<TH1D>("muonfromhadpt_rebin", ";p_{T}^{#mu from had.} [GeV]; Events / bin", 50, 0, 200);
  hmuonfromhadpt_rebin2 = book<TH1D>("muonfromhadpt_rebin2", ";p_{T}^{#mu from had.} [GeV]; Events / bin", 50, 0, 50);
  hmuonfromhadeta = book<TH1D>("muonfromhadeta", ";#eta^{#mu from had.};Events / bin", 100, -5., 5.);
  hmuonfromhadphi = book<TH1D>("muonfromhadphi", ";#phi^{#mu from had.};Events / bin", 70, -3.5, 3.5);
  hmuonfromhadmass = book<TH1D>("muonfromhadmass", ";m^{#mu from had.} [GeV];Events / bin", 50, 0, 1500);
  hmuonfromhadenergy = book<TH1D>("muonfromhadenergy", ";E^{#mu from had.} [GeV];Events / bin", 50, 0, 1500);
  hmuonfromhadpfiso = book<TH1D>("muonfromhadpfiso", ";Iso_{PF}^{#mu not from #tau};Events / bin", 100, 0, 1);
  hmuonfromhaddxy = book<TH1D>("muonfromhaddxy", ";#mu from had. d_{xy} wrt. PV [cm];Events / bin", 250, 0, 0.5);
  hmuonfromhaddz = book<TH1D>("muonfromhaddz", ";#mu from had. d_{z} wrt. PV [cm];Events / bin", 250, 0, 0.5);
  hmuonfromhadd = book<TH1D>("muonfromhadd", ";#mu from had. d wrt. PV [cm];Events / bin", 250, 0, 0.5);
  hmuonfromhad_dxy_vs_dz = book<TH2D>("muonfromhad_dxy_vs_dz", ";#mu from had. d_{xy} wrt. PV [cm];#mu from had. d_{z} wrt. PV [cm];Events / bin", 250, 0, 0.5, 250, 0, 0.5);
  hmuonfromhadgendrmin = book<TH1D>("muonfromhadgendrmin", ";#DeltaR(#mu from had., closest gen #mu);Events / bin", 60, 0, 3);

  hnmuonsunmatched = book<TH1D>("nmuonsunmatched", ";N_{#mu unmatched}; Events / bin", 11, -0.5, 10.5);
  hmuonunmatchedpt = book<TH1D>("muonunmatchedpt", ";p_{T}^{#mu unmatched} [GeV]; Events / bin", 50, 0, 1500);
  hmuonunmatchedpt_rebin = book<TH1D>("muonunmatchedpt_rebin", ";p_{T}^{#mu unmatched} [GeV]; Events / bin", 50, 0, 200);
  hmuonunmatchedpt_rebin2 = book<TH1D>("muonunmatchedpt_rebin2", ";p_{T}^{#mu unmatched} [GeV]; Events / bin", 50, 0, 50);
  hmuonunmatchedeta = book<TH1D>("muonunmatchedeta", ";#eta^{#mu unmatched};Events / bin", 100, -5., 5.);
  hmuonunmatchedphi = book<TH1D>("muonunmatchedphi", ";#phi^{#mu unmatched};Events / bin", 70, -3.5, 3.5);
  hmuonunmatchedmass = book<TH1D>("muonunmatchedmass", ";m^{#mu unmatched} [GeV];Events / bin", 50, 0, 1500);
  hmuonunmatchedenergy = book<TH1D>("muonunmatchedenergy", ";E^{#mu unmatched} [GeV];Events / bin", 50, 0, 1500);
  hmuonunmatchedpfiso = book<TH1D>("muonunmatchedpfiso", ";Iso_{PF}^{#mu unmatched};Events / bin", 100, 0, 1);
  hmuonunmatcheddxy = book<TH1D>("muonunmatcheddxy", ";#mu unmatched d_{xy} wrt. PV [cm];Events / bin", 250, 0, 0.5);
  hmuonunmatcheddz = book<TH1D>("muonunmatcheddz", ";#mu unmatched d_{z} wrt. PV [cm];Events / bin", 250, 0, 0.5);
  hmuonunmatchedd = book<TH1D>("muonunmatchedd", ";#mu unmatched d wrt. PV [cm];Events / bin", 250, 0, 0.5);
  hmuonunmatched_dxy_vs_dz = book<TH2D>("muonunmatched_dxy_vs_dz", ";#mu unmatched d_{xy} wrt. PV [cm];#mu unmatched d_{z} wrt. PV [cm];Events / bin", 250, 0, 0.5, 250, 0, 0.5);
  hmuonunmatchedgendrmin = book<TH1D>("muonunmatchedgendrmin", ";#DeltaR(#mu unmatched, closest gen #mu);Events / bin", 60, 0, 3);

  hmuon1genorigin = book<TH1D>("muon1genorigin", ";#mu 1 -- 1: from hard #tau, 2: from hadron, 0: else, -1: unmatched; Events / bin", 4, -1.5, 2.5);
  hmuon2genorigin = book<TH1D>("muon2genorigin", ";#mu 2 -- 1: from hard #tau, 2: from hadron, 0: else, -1: unmatched; Events / bin", 4, -1.5, 2.5);
  hmuon3genorigin = book<TH1D>("muon3genorigin", ";#mu 3 -- 1: from hard #tau, 2: from hadron, 0: else, -1: unmatched; Events / bin", 4, -1.5, 2.5);
  hmuon4genorigin = book<TH1D>("muon4genorigin", ";#mu 4 -- 1: from hard #tau, 2: from hadron, 0: else, -1: unmatched; Events / bin", 4, -1.5, 2.5);

}

void LQDMMuonOriginHists::fill(const LQDMEvent & event){
  double weight = event.weight;



  // Loop through muons
  // ====================


  size_t nmuons = event.muons->size();
  size_t nfromtau = 0;
  size_t nfromhadron = 0;
  size_t nfromelse = 0;
  size_t nunmatched = 0;

  double d_max = 999999.;
  int gen_origin_closest = -1;
  for(size_t i=0; i<nmuons; i++){
    Muon m = event.muons->at(i);
    float gendr_min = 99999.;
    int gen_origin = -1;
    for(const auto & gp : *event.genparticles_pruned){
      if(abs(gp.pdgid()) != 13) continue;
      if(!gp.get_statusflag(GenParticle::StatusFlag::isLastCopy)) continue;
      float dr = deltaR(m, gp);
      if(dr < gendr_min){
        gendr_min = dr;
        if(gendr_min < 0.2){ // these are updated only for better matches than the previous closest match
          if(gp.get_statusflag(GenParticle::StatusFlag::isDirectHardProcessTauDecayProduct)) gen_origin = 1;
          else if(gp.get_statusflag(GenParticle::StatusFlag::isDirectHadronDecayProduct)) gen_origin = 2;
          else gen_origin = 0;
        }
      }
    }
    if(gen_origin == 1) nfromtau++;
    else if (gen_origin == 2) nfromhadron++;
    else if (gen_origin == 0) nfromelse++;
    else if (gen_origin == -1) nunmatched++;

    if(m.d0() < d_max){
      d_max = m.d0();
      gen_origin_closest = gen_origin;
    }


    hmuongenorigin->Fill(gen_origin, weight);

    if(gen_origin == 1){
      hmuonfromtaupt->Fill(m.pt(), weight);
      hmuonfromtaupt_rebin->Fill(m.pt(), weight);
      hmuonfromtaupt_rebin2->Fill(m.pt(), weight);
      hmuonfromtaueta->Fill(m.eta(), weight);
      hmuonfromtauphi->Fill(m.phi(), weight);
      hmuonfromtaumass->Fill(m.m(), weight);
      hmuonfromtauenergy->Fill(m.e(), weight);
      hmuonfromtaupfiso->Fill(m.iso_rel_04(), weight);
      hmuonfromtaudxy->Fill(m.dxy(), weight);
      hmuonfromtaudz->Fill(m.dz(), weight);
      hmuonfromtaud->Fill(m.d0(), weight);
      hmuonfromtau_dxy_vs_dz->Fill(m.dxy(), m.dz(), weight);
      hmuonfromtaugendrmin->Fill(gendr_min, weight);
    }
    else if(gen_origin == 2){
      hmuonfromhadpt->Fill(m.pt(), weight);
      hmuonfromhadpt_rebin->Fill(m.pt(), weight);
      hmuonfromhadpt_rebin2->Fill(m.pt(), weight);
      hmuonfromhadeta->Fill(m.eta(), weight);
      hmuonfromhadphi->Fill(m.phi(), weight);
      hmuonfromhadmass->Fill(m.m(), weight);
      hmuonfromhadenergy->Fill(m.e(), weight);
      hmuonfromhadpfiso->Fill(m.iso_rel_04(), weight);
      hmuonfromhaddxy->Fill(m.dxy(), weight);
      hmuonfromhaddz->Fill(m.dz(), weight);
      hmuonfromhadd->Fill(m.d0(), weight);
      hmuonfromhad_dxy_vs_dz->Fill(m.dxy(), m.dz(), weight);
      hmuonfromhadgendrmin->Fill(gendr_min, weight);
    }
    else if(gen_origin == -1){
      hmuonunmatchedpt->Fill(m.pt(), weight);
      hmuonunmatchedpt_rebin->Fill(m.pt(), weight);
      hmuonunmatchedpt_rebin2->Fill(m.pt(), weight);
      hmuonunmatchedeta->Fill(m.eta(), weight);
      hmuonunmatchedphi->Fill(m.phi(), weight);
      hmuonunmatchedmass->Fill(m.m(), weight);
      hmuonunmatchedenergy->Fill(m.e(), weight);
      hmuonunmatchedpfiso->Fill(m.iso_rel_04(), weight);
      hmuonunmatcheddxy->Fill(m.dxy(), weight);
      hmuonunmatcheddz->Fill(m.dz(), weight);
      hmuonunmatchedd->Fill(m.d0(), weight);
      hmuonunmatched_dxy_vs_dz->Fill(m.dxy(), m.dz(), weight);
      hmuonunmatchedgendrmin->Fill(gendr_min, weight);
    }

    if(i==0){
      hmuon1genorigin->Fill(gen_origin, weight);
    }
    else if(i==1){
      hmuon2genorigin->Fill(gen_origin, weight);
    }
    else if(i==2){
      hmuon3genorigin->Fill(gen_origin, weight);
    }
    else if(i==3){
      hmuon4genorigin->Fill(gen_origin, weight);
    }
  }
  hnmuonsfromtau->Fill(nfromtau, weight);
  hnmuonsfromhad->Fill(nfromhadron, weight);
  hnmuonsunmatched->Fill(nunmatched, weight);

  hmuonclosestorigin->Fill(gen_origin_closest, weight);
}
