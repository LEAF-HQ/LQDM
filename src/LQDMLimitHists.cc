#include "LQDM/Analyzer/include/LQDMLimitHists.h"
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
  bins3 = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 360, 390, 420, 450, 480, 510, 540, 570, 600, 630, 660, 690, 720, 750, 780, 810, 840, 870, 900, 930, 960, 990, 1020, 1050, 1080, 1110, 1140, 1170, 1200, 1230, 1260, 1290, 1320, 1350, 1380, 1410, 1440, 1470, 1500, 1550, 1600, 1650, 1700, 1750, 1800, 1850, 1900, 1950, 2000, 2100};
  // bins4 = {200, 250, 300, 350, 400, 450, 500, 550, 600, 700, 800, 1000};
  // bins5 = {200, 250, 300, 350, 400, 450, 500, 550, 600, 700, 800, 1000};

  hstmet1  = book<TH1D>("stmet1", ";S_{T}^{MET} [GeV]; Events / bin", 90, 0, 3000);
  hstmet2  = book<TH1D>("stmet2", ";S_{T}^{MET} [GeV]; Events / bin", bins2.size()-1, bins2.data());
  hstmet3  = book<TH1D>("stmet3", ";S_{T}^{MET} [GeV]; Events / bin", bins3.size()-1, bins3.data());
  // hstmet4  = book<TH1D>("stmet4", ";S_{T}^{MET} [GeV]; Events / bin", bins4.size()-1, bins4.data());
  // hstmet5  = book<TH1D>("stmet5", ";S_{T}^{MET} [GeV]; Events / bin", bins5.size()-1, bins5.data());


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

  fillval = stmet;
  if(stmet >= bins3[bins3.size()-1]) fillval = bins3[bins3.size()-1] - 0.01;
  hstmet3->Fill(fillval, weight);
}
