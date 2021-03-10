#include "LQDM/include/LQDMPreselectionHists.h"
#include "include/constants.h"
#include "include/useful_functions.h"
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
  hstmet  = book<TH1D>("stmet", ";S_{T}^{MET} [GeV]; Events / bin", 90, 0, 3000);

  hmemu   = book<TH1D>("memu", ";M_{e#mu} [GeV]; Events / bin", 50, 0, 500);
  hmetau  = book<TH1D>("metau", ";M_{e#tau} [GeV]; Events / bin", 50, 0, 500);
  hmmutau = book<TH1D>("mmutau", ";M_{#mu#tau} [GeV]; Events / bin", 50, 0, 500);



  hsumweights = book<TH1D>("sumweights", ";;Sum of event weights", 1, 0.5, 1.5);


}

void LQDMPreselectionHists::fill(const LQDMEvent & event){
  double weight = event.weight;

  double stmet = event.met->pt();
  for (Jet & jet : *event.jets) stmet += jet.pt();
  for (Electron & e : *event.electrons) stmet += e.pt();
  for (Muon & mu : *event.muons) stmet += mu.pt();
  for (Tau & tau : *event.taus) stmet += tau.pt();

  hmetpt->Fill(event.met->pt(), weight);
  hmetphi->Fill(event.met->phi(), weight);
  hstmet->Fill(stmet, weight);


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


  hsumweights->Fill(1, weight);
}
