#include "Analyzer/LQDM/include/LQDMLimitHists.h"
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

LQDMLimitHists::LQDMLimitHists(TString dir_) : BaseHists(dir_){

  bins2 = {200, 250, 300, 350, 400, 450, 500, 550, 600, 700, 800, 1000};

  hstmet1  = book<TH1D>("stmet1", ";S_{T}^{MET} [GeV]; Events / bin", 90, 0, 3000);
  hstmet2  = book<TH1D>("stmet2", ";S_{T}^{MET} [GeV]; Events / bin", bins2.size()-1, bins2.data());


}

void LQDMLimitHists::fill(const LQDMEvent & event){
  double weight = event.weight;

  double stmet = event.met->pt();
  for (Jet & jet : *event.jets) stmet += jet.pt();
  for (Electron & e : *event.electrons) stmet += e.pt();
  for (Muon & mu : *event.muons) stmet += mu.pt();
  for (Tau & tau : *event.taus) stmet += tau.pt();


  hstmet1->Fill(stmet, weight);

  double fillval = stmet;
  if(stmet >= bins2[bins2.size()-1]) fillval = bins2[bins2.size()-1] - 0.01;
  hstmet2->Fill(fillval, weight);
}
