#include "LQDM/include/LQDMEvent.h"
#include "include/constants.h"
#include <TH1D.h>
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

LQDMEvent::LQDMEvent(){
}

LQDMEvent::~LQDMEvent(){
}

void LQDMEvent::clear(){
  RecoEvent::clear();
}

void LQDMEvent::reset(){
  RecoEvent::reset();
}
