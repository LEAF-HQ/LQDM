#include "LEAF/LQDM/include/LQDMElectronOriginHists.h"
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

LQDMElectronOriginHists::LQDMElectronOriginHists(TString dir_) : BaseHists(dir_){



  helectronclosestorigin = book<TH1D>("electronclosestorigin", ";closest e -- 1: from hard #tau, 2: from hadron, 0: else, -1: unmatched; Events / bin", 4, -1.5, 2.5);
  helectronclosestd = book<TH1D>("electronclosestd", ";closest e d [cm]; Events / bin", 250, 0, 0.5);

  hnelectronsfromtau = book<TH1D>("nelectronsfromtau", ";N_{e from #tau}; Events / bin", 11, -0.5, 10.5);
  helectronfromtaupt = book<TH1D>("electronfromtaupt", ";p_{T}^{e from #tau} [GeV]; Events / bin", 50, 0, 1500);
  helectronfromtaupt_rebin = book<TH1D>("electronfromtaupt_rebin", ";p_{T}^{e from #tau} [GeV]; Events / bin", 50, 0, 200);
  helectronfromtaupt_rebin2 = book<TH1D>("electronfromtaupt_rebin2", ";p_{T}^{e from #tau} [GeV]; Events / bin", 50, 0, 50);
  helectronfromtaueta = book<TH1D>("electronfromtaueta", ";#eta^{e from #tau};Events / bin", 100, -5., 5.);
  helectronfromtauphi = book<TH1D>("electronfromtauphi", ";#phi^{e from #tau};Events / bin", 70, -3.5, 3.5);
  helectronfromtaumass = book<TH1D>("electronfromtaumass", ";m^{e from #tau} [GeV];Events / bin", 50, 0, 1500);
  helectronfromtauenergy = book<TH1D>("electronfromtauenergy", ";E^{e from #tau} [GeV];Events / bin", 50, 0, 1500);
  helectronfromtaupfiso = book<TH1D>("electronfromtaupfiso", ";Iso_{PF}^{e from #tau};Events / bin", 100, 0, 1);
  helectronfromtaudxy = book<TH1D>("electronfromtaudxy", ";e from #tau d_{xy} wrt. PV [cm];Events / bin", 250, 0, 0.5);
  helectronfromtaudz = book<TH1D>("electronfromtaudz", ";e from #tau d_{z} wrt. PV [cm];Events / bin", 250, 0, 0.5);
  helectronfromtaud = book<TH1D>("electronfromtaud", ";e from #tau d wrt. PV [cm];Events / bin", 250, 0, 0.5);
  helectronfromtau_dxy_vs_dz = book<TH2D>("electronfromtau_dxy_vs_dz", ";e from #tau d_{xy} wrt. PV [cm];e from #tau d_{z} wrt. PV [cm];Events / bin", 250, 0, 0.5, 250, 0, 0.5);
  helectronfromtaugendrmin = book<TH1D>("electronfromtaugendrmin", ";#DeltaR(e from #tau, closest gen e);Events / bin", 60, 0, 3);

  hnelectronsfromhad = book<TH1D>("nelectronsfromhad", ";N_{e from had.}; Events / bin", 11, -0.5, 10.5);
  helectronfromhadpt = book<TH1D>("electronfromhadpt", ";p_{T}^{e from had.} [GeV]; Events / bin", 50, 0, 1500);
  helectronfromhadpt_rebin = book<TH1D>("electronfromhadpt_rebin", ";p_{T}^{e from had.} [GeV]; Events / bin", 50, 0, 200);
  helectronfromhadpt_rebin2 = book<TH1D>("electronfromhadpt_rebin2", ";p_{T}^{e from had.} [GeV]; Events / bin", 50, 0, 50);
  helectronfromhadeta = book<TH1D>("electronfromhadeta", ";#eta^{e from had.};Events / bin", 100, -5., 5.);
  helectronfromhadphi = book<TH1D>("electronfromhadphi", ";#phi^{e from had.};Events / bin", 70, -3.5, 3.5);
  helectronfromhadmass = book<TH1D>("electronfromhadmass", ";m^{e from had.} [GeV];Events / bin", 50, 0, 1500);
  helectronfromhadenergy = book<TH1D>("electronfromhadenergy", ";E^{e from had.} [GeV];Events / bin", 50, 0, 1500);
  helectronfromhadpfiso = book<TH1D>("electronfromhadpfiso", ";Iso_{PF}^{e from had.};Events / bin", 100, 0, 1);
  helectronfromhaddxy = book<TH1D>("electronfromhaddxy", ";e from had. d_{xy} wrt. PV [cm];Events / bin", 250, 0, 0.5);
  helectronfromhaddz = book<TH1D>("electronfromhaddz", ";e from had. d_{z} wrt. PV [cm];Events / bin", 250, 0, 0.5);
  helectronfromhadd = book<TH1D>("electronfromhadd", ";e from had. d wrt. PV [cm];Events / bin", 250, 0, 0.5);
  helectronfromhad_dxy_vs_dz = book<TH2D>("electronfromhad_dxy_vs_dz", ";e from had. d_{xy} wrt. PV [cm];e from had. d_{z} wrt. PV [cm];Events / bin", 250, 0, 0.5, 250, 0, 0.5);
  helectronfromhadgendrmin = book<TH1D>("electronfromhadgendrmin", ";#DeltaR(e from had., closest gen e);Events / bin", 60, 0, 3);

  hnelectronsunmatched = book<TH1D>("nelectronsunmatched", ";N_{e unmatched}; Events / bin", 11, -0.5, 10.5);
  helectronunmatchedpt = book<TH1D>("electronunmatchedpt", ";p_{T}^{e unmatched} [GeV]; Events / bin", 50, 0, 1500);
  helectronunmatchedpt_rebin = book<TH1D>("electronunmatchedpt_rebin", ";p_{T}^{e unmatched} [GeV]; Events / bin", 50, 0, 200);
  helectronunmatchedpt_rebin2 = book<TH1D>("electronunmatchedpt_rebin2", ";p_{T}^{e unmatched} [GeV]; Events / bin", 50, 0, 50);
  helectronunmatchedeta = book<TH1D>("electronunmatchedeta", ";#eta^{e unmatched};Events / bin", 100, -5., 5.);
  helectronunmatchedphi = book<TH1D>("electronunmatchedphi", ";#phi^{e unmatched};Events / bin", 70, -3.5, 3.5);
  helectronunmatchedmass = book<TH1D>("electronunmatchedmass", ";m^{e unmatched} [GeV];Events / bin", 50, 0, 1500);
  helectronunmatchedenergy = book<TH1D>("electronunmatchedenergy", ";E^{e unmatched} [GeV];Events / bin", 50, 0, 1500);
  helectronunmatchedpfiso = book<TH1D>("electronunmatchedpfiso", ";Iso_{PF}^{e unmatched};Events / bin", 100, 0, 1);
  helectronunmatcheddxy = book<TH1D>("electronunmatcheddxy", ";e unmatched d_{xy} wrt. PV [cm];Events / bin", 250, 0, 0.5);
  helectronunmatcheddz = book<TH1D>("electronunmatcheddz", ";e unmatched d_{z} wrt. PV [cm];Events / bin", 250, 0, 0.5);
  helectronunmatchedd = book<TH1D>("electronunmatchedd", ";e unmatched d wrt. PV [cm];Events / bin", 250, 0, 0.5);
  helectronunmatched_dxy_vs_dz = book<TH2D>("electronunmatched_dxy_vs_dz", ";e unmatched d_{xy} wrt. PV [cm];e unmatched d_{z} wrt. PV [cm];Events / bin", 250, 0, 0.5, 250, 0, 0.5);
  helectronunmatchedgendrmin = book<TH1D>("electronunmatchedgendrmin", ";#DeltaR(e unmatched, closest gen e);Events / bin", 60, 0, 3);

  helectrongenorigin = book<TH1D>("electrongenorigin", ";e -- 1: from hard #tau, 2: from hadron, 0: else, -1: unmatched; Events / bin", 4, -1.5, 2.5);
  hnelectronsgenfromtau = book<TH1D>("nelectronsgenfromtau", ";Number of e from #tau decay; Events / bin", 11, -0.5, 10.5);
  hnelectronsgenfromhadron = book<TH1D>("nelectronsgenfromhadron", ";Number of e from hadron decay; Events / bin", 11, -0.5, 10.5);
  hnelectronsgenfromelse = book<TH1D>("nelectronsgenfromelse", ";Number of e from other decay; Events / bin", 11, -0.5, 10.5);
  hnelectronsgenunmatched = book<TH1D>("nelectronsgenunmatched", ";Number of unmatched e; Events / bin", 11, -0.5, 10.5);
  helectron1genorigin = book<TH1D>("electron1genorigin", ";e 1 -- 1: from hard #tau, 2: from hadron, 0: else, -1: unmatched; Events / bin", 4, -1.5, 2.5);
  helectron2genorigin = book<TH1D>("electron2genorigin", ";e 2 -- 1: from hard #tau, 2: from hadron, 0: else, -1: unmatched; Events / bin", 4, -1.5, 2.5);
  helectron3genorigin = book<TH1D>("electron3genorigin", ";e 3 -- 1: from hard #tau, 2: from hadron, 0: else, -1: unmatched; Events / bin", 4, -1.5, 2.5);
  helectron4genorigin = book<TH1D>("electron4genorigin", ";e 4 -- 1: from hard #tau, 2: from hadron, 0: else, -1: unmatched; Events / bin", 4, -1.5, 2.5);

}

void LQDMElectronOriginHists::fill(const LQDMEvent & event){
  if(event.is_data) return;
  double weight = event.weight;



  // Loop through jets
  // ====================

  size_t nelectrons = event.electrons->size();
  size_t nfromtau = 0;
  size_t nfromhadron = 0;
  size_t nfromelse = 0;
  size_t nunmatched = 0;

  double d_max = 999999.;
  int gen_origin_closest = -1;
  for(size_t i=0; i<nelectrons; i++){
    Electron e = event.electrons->at(i);
    float gendr_min = 99999.;
    int gen_origin = -1;
    for(const auto & gp : *event.genparticles_pruned){
      if(abs(gp.pdgid()) != 11) continue;
      if(!gp.get_statusflag(GenParticle::StatusFlag::isLastCopy)) continue;
      float dr = deltaR(e, gp);
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

    if(e.d0() < d_max){
      d_max = e.d0();
      gen_origin_closest = gen_origin;
    }

    helectrongenorigin->Fill(gen_origin, weight);



    if(gen_origin == 1){
      helectronfromtaupt->Fill(e.pt(), weight);
      helectronfromtaupt_rebin->Fill(e.pt(), weight);
      helectronfromtaupt_rebin2->Fill(e.pt(), weight);
      helectronfromtaueta->Fill(e.eta(), weight);
      helectronfromtauphi->Fill(e.phi(), weight);
      helectronfromtaumass->Fill(e.m(), weight);
      helectronfromtauenergy->Fill(e.e(), weight);
      helectronfromtaupfiso->Fill(e.iso_rel_03(), weight);
      helectronfromtaudxy->Fill(e.dxy(), weight);
      helectronfromtaudz->Fill(e.dz(), weight);
      helectronfromtaud->Fill(e.d0(), weight);
      helectronfromtau_dxy_vs_dz->Fill(e.dxy(), e.dz(), weight);
      helectronfromtaugendrmin->Fill(gendr_min, weight);
    }
    else if(gen_origin == 2){
      helectronfromhadpt->Fill(e.pt(), weight);
      helectronfromhadpt_rebin->Fill(e.pt(), weight);
      helectronfromhadpt_rebin2->Fill(e.pt(), weight);
      helectronfromhadeta->Fill(e.eta(), weight);
      helectronfromhadphi->Fill(e.phi(), weight);
      helectronfromhadmass->Fill(e.m(), weight);
      helectronfromhadenergy->Fill(e.e(), weight);
      helectronfromhadpfiso->Fill(e.iso_rel_03(), weight);
      helectronfromhaddxy->Fill(e.dxy(), weight);
      helectronfromhaddz->Fill(e.dz(), weight);
      helectronfromhadd->Fill(e.d0(), weight);
      helectronfromhad_dxy_vs_dz->Fill(e.dxy(), e.dz(), weight);
      helectronfromhadgendrmin->Fill(gendr_min, weight);
    }
    else if(gen_origin==-1){
      helectronunmatchedpt->Fill(e.pt(), weight);
      helectronunmatchedpt_rebin->Fill(e.pt(), weight);
      helectronunmatchedpt_rebin2->Fill(e.pt(), weight);
      helectronunmatchedeta->Fill(e.eta(), weight);
      helectronunmatchedphi->Fill(e.phi(), weight);
      helectronunmatchedmass->Fill(e.m(), weight);
      helectronunmatchedenergy->Fill(e.e(), weight);
      helectronunmatchedpfiso->Fill(e.iso_rel_03(), weight);
      helectronunmatcheddxy->Fill(e.dxy(), weight);
      helectronunmatcheddz->Fill(e.dz(), weight);
      helectronunmatchedd->Fill(e.d0(), weight);
      helectronunmatched_dxy_vs_dz->Fill(e.dxy(), e.dz(), weight);
      helectronunmatchedgendrmin->Fill(gendr_min, weight);
    }
  }
  hnelectronsfromtau->Fill(nfromtau, weight);
  hnelectronsunmatched->Fill(nunmatched, weight);
  hnelectronsfromhad->Fill(nfromhadron, weight);
  hnelectronsgenfromtau->Fill(nfromtau, weight);
  hnelectronsgenfromhadron->Fill(nfromhadron, weight);
  hnelectronsgenfromelse->Fill(nfromelse, weight);
  hnelectronsgenunmatched->Fill(nunmatched, weight);

  helectronclosestorigin->Fill(gen_origin_closest, weight);
  helectronclosestd->Fill(d_max, weight);
}
