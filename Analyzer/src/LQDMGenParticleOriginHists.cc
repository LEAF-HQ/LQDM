#include "LQDM/Analyzer/include/LQDMGenParticleOriginHists.h"
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

LQDMGenParticleOriginHists::LQDMGenParticleOriginHists(TString dir_) : BaseHists(dir_){


  hgenmuorigin = book<TH1D>("genmuorigin", ";#mu -- 1: from hard #tau, 2: from hadron, 0: else ; Events / bin", 3, -0.5, 2.5);
  hgenmu1origin = book<TH1D>("genmu1origin", ";#mu 1 -- 1: from hard #tau, 2: from hadron, 0: else ; Events / bin", 3, -0.5, 2.5);
  hgenmu2origin = book<TH1D>("genmu2origin", ";#mu 2 -- 1: from hard #tau, 2: from hadron, 0: else ; Events / bin", 3, -0.5, 2.5);

  hgenmusfromtautotal = book<TH1D>("genmusfromtautotal", ";binc = N_{gen #mu from #tau}; Events / bin", 1, -0.5, 0.5);
  hgenmusfromtaumatched = book<TH1D>("genmusfromtaumatched", ";binc = N_{matched gen #mu from #tau}; Events / bin", 1, -0.5, 0.5);
  hptgenmufromtau = book<TH1D>("ptgenmufromtau", ";p_{T}^{gen #mu from #tau} [GeV]; Events / bin", 40, 0, 1200);
  hptgenmufromtau_rebin = book<TH1D>("ptgenmufromtau_rebin", ";p_{T}^{gen #mu from #tau} [GeV]; Events / bin", 50, 0, 200);
  hptgenmufromtau_rebin2 = book<TH1D>("ptgenmufromtau_rebin2", ";p_{T}^{gen #mu from #tau} [GeV]; Events / bin", 50, 0, 50);
  hptgenmufromtaumatched = book<TH1D>("ptgenmufromtaumatched", ";p_{T}^{gen and matched #mu from #tau} [GeV]; Events / bin", 40, 0, 1200);
  hptgenmufromtaumatched_rebin = book<TH1D>("ptgenmufromtaumatched_rebin", ";p_{T}^{gen and matched #mu from #tau} [GeV]; Events / bin", 50, 0, 200);
  hptgenmufromtaumatched_rebin2 = book<TH1D>("ptgenmufromtaumatched_rebin2", ";p_{T}^{gen and matched #mu from #tau} [GeV]; Events / bin", 50, 0, 50);
  hdrgendimufromtau = book<TH1D>("drgendimufromtau", ";#DeltaR(gen #mu from #tau i, gen #mu j); Events / bin", 200, 0, 10);
  hdphigendimufromtau = book<TH1D>("dphigendimufromtau", ";#Delta#Phi(gen #mu from #tau i, gen #mu j); Events / bin", 35, 0, 3.5);
  hdetagendimufromtau = book<TH1D>("detagendimufromtau", ";#Delta#eta(gen #mu from #tau i, gen #mu j); Events / bin", 100, 0, 10);
  hdrgenmufromtauvistau = book<TH1D>("drgenmufromtauvistau", ";#DeltaR(gen #mu from #tau i, gen vis #tau j); Events / bin", 200, 0, 10);
  hdphigenmufromtauvistau = book<TH1D>("dphigenmufromtauvistau", ";#Delta#Phi(gen #mu from #tau i, gen vis #tau j); Events / bin", 35, 0, 3.5);
  hdetagenmufromtauvistau = book<TH1D>("detagenmufromtauvistau", ";#Delta#eta(gen #mu from #tau i, gen vis #tau j); Events / bin", 100, 0, 10);
  hdrgenmufromtaue = book<TH1D>("drgenmufromtaue", ";#DeltaR(gen #mu from #tau i, gen e j); Events / bin", 200, 0, 10);
  hdphigenmufromtaue = book<TH1D>("dphigenmufromtaue", ";#Delta#Phi(gen #mu from #tau i, gen e j); Events / bin", 35, 0, 3.5);
  hdetagenmufromtaue = book<TH1D>("detagenmufromtaue", ";#Delta#eta(gen #mu from #tau i, gen e j); Events / bin", 100, 0, 10);

  hgenmusfromhadtotal = book<TH1D>("genmusfromhadtotal", ";binc = N_{gen #mu from had}; Events / bin", 1, -0.5, 0.5);
  hgenmusfromhadmatched = book<TH1D>("genmusfromhadmatched", ";binc = N_{matched gen #mu from had}; Events / bin", 1, -0.5, 0.5);
  hptgenmufromhad = book<TH1D>("ptgenmufromhad", ";p_{T}^{gen #mu from had} [GeV]; Events / bin", 40, 0, 1200);
  hptgenmufromhad_rebin = book<TH1D>("ptgenmufromhad_rebin", ";p_{T}^{gen #mu from had} [GeV]; Events / bin", 50, 0, 200);
  hptgenmufromhad_rebin2 = book<TH1D>("ptgenmufromhad_rebin2", ";p_{T}^{gen #mu from had} [GeV]; Events / bin", 50, 0, 50);
  hptgenmufromhadmatched = book<TH1D>("ptgenmufromhadmatched", ";p_{T}^{gen and matched #mu from had} [GeV]; Events / bin", 40, 0, 1200);
  hptgenmufromhadmatched_rebin = book<TH1D>("ptgenmufromhadmatched_rebin", ";p_{T}^{gen and matched #mu from had} [GeV]; Events / bin", 50, 0, 200);
  hptgenmufromhadmatched_rebin2 = book<TH1D>("ptgenmufromhadmatched_rebin2", ";p_{T}^{gen and matched #mu from had} [GeV]; Events / bin", 50, 0, 50);
  hdrgendimufromhad = book<TH1D>("drgendimufromhad", ";#DeltaR(gen #mu from had i, gen #mu j); Events / bin", 200, 0, 10);
  hdphigendimufromhad = book<TH1D>("dphigendimufromhad", ";#Delta#Phi(gen #mu from had i, gen #mu j); Events / bin", 35, 0, 3.5);
  hdetagendimufromhad = book<TH1D>("detagendimufromhad", ";#Delta#eta(gen #mu from had i, gen #mu j); Events / bin", 100, 0, 10);
  hdrgenmufromhadvistau = book<TH1D>("drgenmufromhadvistau", ";#DeltaR(gen #mu from had i, gen vis #tau j); Events / bin", 200, 0, 10);
  hdphigenmufromhadvistau = book<TH1D>("dphigenmufromhadvistau", ";#Delta#Phi(gen #mu from had i, gen vis #tau j); Events / bin", 35, 0, 3.5);
  hdetagenmufromhadvistau = book<TH1D>("detagenmufromhadvistau", ";#Delta#eta(gen #mu from had i, gen vis #tau j); Events / bin", 100, 0, 10);
  hdrgenmufromhade = book<TH1D>("drgenmufromhade", ";#DeltaR(gen #mu from had i, gen e j); Events / bin", 200, 0, 10);
  hdphigenmufromhade = book<TH1D>("dphigenmufromhade", ";#Delta#Phi(gen #mu from had i, gen e j); Events / bin", 35, 0, 3.5);
  hdetagenmufromhade = book<TH1D>("detagenmufromhade", ";#Delta#eta(gen #mu from had i, gen e j); Events / bin", 100, 0, 10);


  hgenelorigin = book<TH1D>("genelorigin", ";e -- 1: from hard #tau, 2: from hadron, 0: else ; Events / bin", 3, -0.5, 2.5);
  hgenel1origin = book<TH1D>("genel1origin", ";e 1 -- 1: from hard #tau, 2: from hadron, 0: else ; Events / bin", 3, -0.5, 2.5);
  hgenel2origin = book<TH1D>("genel2origin", ";e 2 -- 1: from hard #tau, 2: from hadron, 0: else ; Events / bin", 3, -0.5, 2.5);

  hgenelsfromtautotal = book<TH1D>("genelsfromtautotal", ";binc = total N_{gen e from #tau}; Events / bin", 1, -0.5, 0.5);
  hgenelsfromtaumatched = book<TH1D>("genelsfromtaumatched", ";binc = total N_{matched gen e from #tau}; Events / bin", 1, -0.5, 0.5);
  hptgenelfromtau = book<TH1D>("ptgenelfromtau", ";p_{T}^{gen e from #tau} [GeV]; Events / bin", 40, 0, 1200);
  hptgenelfromtau_rebin = book<TH1D>("ptgenelfromtau_rebin", ";p_{T}^{gen e from #tau} [GeV]; Events / bin", 50, 0, 200);
  hptgenelfromtau_rebin2 = book<TH1D>("ptgenelfromtau_rebin2", ";p_{T}^{gen e from #tau} [GeV]; Events / bin", 50, 0, 50);
  hptgenelfromtaumatched = book<TH1D>("ptgenelfromtaumatched", ";p_{T}^{gen and matched e from #tau} [GeV]; Events / bin", 40, 0, 1200);
  hptgenelfromtaumatched_rebin = book<TH1D>("ptgenelfromtaumatched_rebin", ";p_{T}^{gen e from #tau} [GeV]; Events / bin", 50, 0, 200);
  hptgenelfromtaumatched_rebin2 = book<TH1D>("ptgenelfromtaumatched_rebin2", ";p_{T}^{gen e from #tau} [GeV]; Events / bin", 50, 0, 50);
  hdrgendiefromtau = book<TH1D>("drgendiefromtau", ";#DeltaR(gen #e from #tau i, gen e j); Events / bin", 200, 0, 10);
  hdphigendiefromtau = book<TH1D>("dphigendiefromtau", ";#Delta#Phi(gen e from #tau i, gen e j); Events / bin", 35, 0, 3.5);
  hdetagendiefromtau = book<TH1D>("detagendiefromtau", ";#Delta#eta(gen e from #tau i, gen e j); Events / bin", 100, 0, 10);
  hdrgenefromtauvistau = book<TH1D>("drgenefromtauvistau", ";#DeltaR(gen e from #tau i, gen vis #tau j); Events / bin", 200, 0, 10);
  hdphigenefromtauvistau = book<TH1D>("dphigenefromtauvistau", ";#Delta#Phi(gen e from #tau i, gen vis #tau j); Events / bin", 35, 0, 3.5);
  hdetagenefromtauvistau = book<TH1D>("detagenefromtauvistau", ";#Delta#eta(gen e from #tau i, gen vis #tau j); Events / bin", 100, 0, 10);
  hdrgenefromtaumu = book<TH1D>("drgenefromtaumu", ";#DeltaR(gen e from #tau i, gen #mu j); Events / bin", 200, 0, 10);
  hdphigenefromtaumu = book<TH1D>("dphigenefromtaumu", ";#Delta#Phi(gen e from #tau i, gen #mu j); Events / bin", 35, 0, 3.5);
  hdetagenefromtaumu = book<TH1D>("detagenefromtaumu", ";#Delta#eta(gen e from #tau i, gen #mu j); Events / bin", 100, 0, 10);

  hgenelsfromhadtotal = book<TH1D>("genelsfromhadtotal", ";binc = total N_{gen e from had}; Events / bin", 1, -0.5, 0.5);
  hgenelsfromhadmatched = book<TH1D>("genelsfromhadmatched", ";binc = total N_{matched gen e from had}; Events / bin", 1, -0.5, 0.5);
  hptgenelfromhad = book<TH1D>("ptgenelfromhad", ";p_{T}^{gen e from had} [GeV]; Events / bin", 40, 0, 1200);
  hptgenelfromhad_rebin = book<TH1D>("ptgenelfromhad_rebin", ";p_{T}^{gen e from had} [GeV]; Events / bin", 50, 0, 200);
  hptgenelfromhad_rebin2 = book<TH1D>("ptgenelfromhad_rebin2", ";p_{T}^{gen e from had} [GeV]; Events / bin", 50, 0, 50);
  hptgenelfromhadmatched = book<TH1D>("ptgenelfromhadmatched", ";p_{T}^{gen and matched e from had} [GeV]; Events / bin", 40, 0, 1200);
  hptgenelfromhadmatched_rebin = book<TH1D>("ptgenelfromhadmatched_rebin", ";p_{T}^{gen e from had} [GeV]; Events / bin", 50, 0, 200);
  hptgenelfromhadmatched_rebin2 = book<TH1D>("ptgenelfromhadmatched_rebin2", ";p_{T}^{gen e from had} [GeV]; Events / bin", 50, 0, 50);
  hdrgendiefromhad = book<TH1D>("drgendiefromhad", ";#DeltaR(gen e from had i, gen e j); Events / bin", 200, 0, 10);
  hdphigendiefromhad = book<TH1D>("dphigendiefromhad", ";#Delta#Phi(gen e from had i, gen e j); Events / bin", 35, 0, 3.5);
  hdetagendiefromhad = book<TH1D>("detagendiefromhad", ";#Delta#eta(gen e from had i, gen e j); Events / bin", 100, 0, 10);
  hdrgenefromhadvistau = book<TH1D>("drgenefromhadvistau", ";#DeltaR(gen e from had i, gen vis #tau j); Events / bin", 200, 0, 10);
  hdphigenefromhadvistau = book<TH1D>("dphigenefromhadvistau", ";#Delta#Phi(gen e from had i, gen vis #tau j); Events / bin", 35, 0, 3.5);
  hdetagenefromhadvistau = book<TH1D>("detagenefromhadvistau", ";#Delta#eta(gen e from had i, gen vis #tau j); Events / bin", 100, 0, 10);
  hdrgenefromhadmu = book<TH1D>("drgenefromhadmu", ";#DeltaR(gen e from had i, gen #mu j); Events / bin", 200, 0, 10);
  hdphigenefromhadmu = book<TH1D>("dphigenefromhadmu", ";#Delta#Phi(gen e from had i, gen #mu j); Events / bin", 35, 0, 3.5);
  hdetagenefromhadmu = book<TH1D>("detagenefromhadmu", ";#Delta#eta(gen e from had i, gen #mu j); Events / bin", 100, 0, 10);



  hgenborigin = book<TH1D>("genborigin", ";b -- 1: is hard proc., 2: from hard proc., 0: else ; Events / bin", 3, -0.5, 2.5);
  hgenb1origin = book<TH1D>("genb1origin", ";b 1 -- 1: is hard proc., 2: from hard proc., 0: else ; Events / bin", 3, -0.5, 2.5);
  hgenb2origin = book<TH1D>("genb2origin", ";b 2 -- 1: is hard proc., 2: from hard proc., 0: else ; Events / bin", 3, -0.5, 2.5);

  hgenbsfromhardtotal = book<TH1D>("genbsfromhardtotal", ";binc = total N_{gen b from hard proc.}; Events / bin", 1, -0.5, 0.5);
  hgenbsfromhardmatched = book<TH1D>("genbsfromhardmatched", ";binc = total N_{matched gen b from hard proc.}; Events / bin", 1, -0.5, 0.5);
  hptgenbfromhard = book<TH1D>("ptgenbfromhard", ";p_{T}^{gen b from hard proc.} [GeV]; Events / bin", 40, 0, 1200);
  hptgenbfromhard_rebin = book<TH1D>("ptgenbfromhard_rebin", ";p_{T}^{gen b from hard proc.} [GeV]; Events / bin", 50, 0, 200);
  hptgenbfromhard_rebin2 = book<TH1D>("ptgenbfromhard_rebin2", ";p_{T}^{gen b from hard proc.} [GeV]; Events / bin", 50, 0, 50);
  hptgenbfromhardmatched = book<TH1D>("ptgenbfromhardmatched", ";p_{T}^{gen and matched b from hard proc.} [GeV]; Events / bin", 40, 0, 1200);
  hptgenbfromhardmatched_rebin = book<TH1D>("ptgenbfromhardmatched_rebin", ";p_{T}^{gen b from hard proc.} [GeV]; Events / bin", 50, 0, 200);
  hptgenbfromhardmatched_rebin2 = book<TH1D>("ptgenbfromhardmatched_rebin2", ";p_{T}^{gen b from hard proc.} [GeV]; Events / bin", 50, 0, 50);
  hdrgendibfromhard = book<TH1D>("drgendibfromhard", ";#DeltaR(gen b from hard proc. i, gen b from hard proc. j); Events / bin", 200, 0, 10);
  hdphigendibfromhard = book<TH1D>("dphigendibfromhard", ";#Delta#Phi(gen b from hard proc. i, gen b from hard proc. j); Events / bin", 35, 0, 3.5);
  hdetagendibfromhard = book<TH1D>("detagendibfromhard", ";#Delta#eta(gen b from hard proc. i, gen b from hard proc. j); Events / bin", 100, 0, 10);
  hdrgenbfromhardvistau = book<TH1D>("drgenbfromhardvistau", ";#DeltaR(gen b from hard proc. i, gen vis #tau j); Events / bin", 200, 0, 10);
  hdphigenbfromhardvistau = book<TH1D>("dphigenbfromhardvistau", ";#Delta#Phi(gen b from hard proc. i, gen vis #tau j); Events / bin", 35, 0, 3.5);
  hdetagenbfromhardvistau = book<TH1D>("detagenbfromhardvistau", ";#Delta#eta(gen b from hard proc. i, gen vis #tau j); Events / bin", 100, 0, 10);
  hdrgenbfromhardmu = book<TH1D>("drgenbfromhardmu", ";#DeltaR(gen b from hard proc. i, gen #mu j); Events / bin", 200, 0, 10);
  hdphigenbfromhardmu = book<TH1D>("dphigenbfromhardmu", ";#Delta#Phi(gen b from hard proc. i, gen #mu j); Events / bin", 35, 0, 3.5);
  hdetagenbfromhardmu = book<TH1D>("detagenbfromhardmu", ";#Delta#eta(gen b from hard proc. i, gen #mu j); Events / bin", 100, 0, 10);
  hdrgenbfromharde = book<TH1D>("drgenbfromharde", ";#DeltaR(gen b from hard proc. i, gen e j); Events / bin", 200, 0, 10);
  hdphigenbfromharde = book<TH1D>("dphigenbfromharde", ";#Delta#Phi(gen b from hard proc. i, gen e j); Events / bin", 35, 0, 3.5);
  hdetagenbfromharde = book<TH1D>("detagenbfromharde", ";#Delta#eta(gen b from hard proc. i, gen e j); Events / bin", 100, 0, 10);

  hgenbsnotfromhardtotal = book<TH1D>("genbsnotfromhardtotal", ";binc = total N_{gen b not from hard proc.}; Events / bin", 1, -0.5, 0.5);
  hgenbsnotfromhardmatched = book<TH1D>("genbsnotfromhardmatched", ";binc = total N_{matched gen b not from hard proc.}; Events / bin", 1, -0.5, 0.5);
  hptgenbnotfromhard = book<TH1D>("ptgenbnotfromhard", ";p_{T}^{gen b not from hard proc.} [GeV]; Events / bin", 40, 0, 1200);
  hptgenbnotfromhard_rebin = book<TH1D>("ptgenbnotfromhard_rebin", ";p_{T}^{gen b not from hard proc.} [GeV]; Events / bin", 50, 0, 200);
  hptgenbnotfromhard_rebin2 = book<TH1D>("ptgenbnotfromhard_rebin2", ";p_{T}^{gen b not from hard proc.} [GeV]; Events / bin", 50, 0, 50);
  hptgenbnotfromhardmatched = book<TH1D>("ptgenbnotfromhardmatched", ";p_{T}^{gen and matched b not from hard proc.} [GeV]; Events / bin", 40, 0, 1200);
  hptgenbnotfromhardmatched_rebin = book<TH1D>("ptgenbnotfromhardmatched_rebin", ";p_{T}^{gen b not from hard proc.} [GeV]; Events / bin", 50, 0, 200);
  hptgenbnotfromhardmatched_rebin2 = book<TH1D>("ptgenbnotfromhardmatched_rebin2", ";p_{T}^{gen b not from hard proc.} [GeV]; Events / bin", 50, 0, 50);
  hdrgendibnotfromhard = book<TH1D>("drgendibnotfromhard", ";#DeltaR(gen b not from hard proc. i, gen b not from hard proc. j); Events / bin", 200, 0, 10);
  hdphigendibnotfromhard = book<TH1D>("dphigendibnotfromhard", ";#Delta#Phi(gen b not from hard proc. i, gen b not from hard proc. j); Events / bin", 35, 0, 3.5);
  hdetagendibnotfromhard = book<TH1D>("detagendibnotfromhard", ";#Delta#eta(gen b not from hard proc. i, gen b not from hard proc. j); Events / bin", 100, 0, 10);
  hdrgenbnotfromhardvistau = book<TH1D>("drgenbnotfromhardvistau", ";#DeltaR(gen b not from hard proc. i, gen vis #tau j); Events / bin", 200, 0, 10);
  hdphigenbnotfromhardvistau = book<TH1D>("dphigenbnotfromhardvistau", ";#Delta#Phi(gen b not from hard proc. i, gen vis #tau j); Events / bin", 35, 0, 3.5);
  hdetagenbnotfromhardvistau = book<TH1D>("detagenbnotfromhardvistau", ";#Delta#eta(gen b not from hard proc. i, gen vis #tau j); Events / bin", 100, 0, 10);
  hdrgenbnotfromhardmu = book<TH1D>("drgenbnotfromhardmu", ";#DeltaR(gen b not from hard proc. i, gen #mu j); Events / bin", 200, 0, 10);
  hdphigenbnotfromhardmu = book<TH1D>("dphigenbnotfromhardmu", ";#Delta#Phi(gen b not from hard proc. i, gen #mu j); Events / bin", 35, 0, 3.5);
  hdetagenbnotfromhardmu = book<TH1D>("detagenbnotfromhardmu", ";#Delta#eta(gen b not from hard proc. i, gen #mu j); Events / bin", 100, 0, 10);
  hdrgenbnotfromharde = book<TH1D>("drgenbnotfromharde", ";#DeltaR(gen b not from hard proc. i, gen e j); Events / bin", 200, 0, 10);
  hdphigenbnotfromharde = book<TH1D>("dphigenbnotfromharde", ";#Delta#Phi(gen b not from hard proc. i, gen e j); Events / bin", 35, 0, 3.5);
  hdetagenbnotfromharde = book<TH1D>("detagenbnotfromharde", ";#Delta#eta(gen b not from hard proc. i, gen e j); Events / bin", 100, 0, 10);


}

void LQDMGenParticleOriginHists::fill(const LQDMEvent & event){
  if(event.is_data) return;
  double weight = event.weight;



  // loop through all hard genparticles
  int n_genmus = 0;
  int n_genels = 0;
  int n_genbs = 0;
  int gpidx = -1;
  for(const GenParticle & gp : *event.genparticles_all){
    gpidx++;
    int id = abs(gp.pdgid());
    if(id != 11 && id != 13 && id != 15 && id != 5 && id != 12 && id != 14 && id != 16) continue;
    if(!gp.get_statusflag(GenParticle::isLastCopy)) continue;

    if(id == 13){ // muons
      n_genmus++;

      auto closest_mu = closestParticle(gp, *event.muons);
      float dr_min = -1.;
      if(closest_mu != nullptr) dr_min = deltaR(gp, *closest_mu);
      int origin = 0.;
      if(gp.get_statusflag(GenParticle::isDirectHardProcessTauDecayProduct)) origin = 1.;
      else if(gp.get_statusflag(GenParticle::isDirectHadronDecayProduct)) origin = 2.;

      if(gp.get_statusflag(GenParticle::isDirectHardProcessTauDecayProduct)){
        hgenmusfromtautotal->Fill(0., weight);
        hptgenmufromtau->Fill(gp.pt(), weight);
        hptgenmufromtau_rebin->Fill(gp.pt(), weight);
        hptgenmufromtau_rebin2->Fill(gp.pt(), weight);

        for(size_t i=0; i<event.genparticles_all->size(); i++){
          GenParticle gpmu = event.genparticles_all->at(i);
          if (abs(gpmu.pdgid()) != 13 || !gpmu.get_statusflag(GenParticle::isLastCopy)) continue;
          if ((int)i == gpidx) continue;
          hdrgendimufromtau->Fill(deltaR(gp, gpmu), weight);
          hdphigendimufromtau->Fill(deltaPhi(gp, gpmu), weight);
          hdetagendimufromtau->Fill(deltaEta(gp, gpmu), weight);
        }

        for(size_t i=0; i<event.genparticles_all->size(); i++){
          GenParticle gpe = event.genparticles_all->at(i);
          if (abs(gpe.pdgid()) != 11 || !gpe.get_statusflag(GenParticle::isLastCopy)) continue;
          if ((int)i == gpidx) continue;
          hdrgenmufromtaue->Fill(deltaR(gp, gpe), weight);
          hdphigenmufromtaue->Fill(deltaPhi(gp, gpe), weight);
          hdetagenmufromtaue->Fill(deltaEta(gp, gpe), weight);
        }

        for(size_t i=0; i<event.genparticles_visibletaus->size(); i++){
          GenParticle gvt = event.genparticles_visibletaus->at(i);
          hdrgenmufromtauvistau->Fill(deltaR(gp, gvt), weight);
          hdphigenmufromtauvistau->Fill(deltaPhi(gp, gvt), weight);
          hdetagenmufromtauvistau->Fill(deltaEta(gp, gvt), weight);
        }


        if(dr_min < 0.2 && dr_min > 0.){
          hgenmusfromtaumatched->Fill(0., weight);
          hptgenmufromtaumatched->Fill(gp.pt(), weight);
          hptgenmufromtaumatched_rebin->Fill(gp.pt(), weight);
          hptgenmufromtaumatched_rebin2->Fill(gp.pt(), weight);
        }
      }
      else if(gp.get_statusflag(GenParticle::isDirectHadronDecayProduct)){
        hgenmusfromhadtotal->Fill(0., weight);
        hptgenmufromhad->Fill(gp.pt(), weight);
        hptgenmufromhad_rebin->Fill(gp.pt(), weight);
        hptgenmufromhad_rebin2->Fill(gp.pt(), weight);

        for(size_t i=0; i<event.genparticles_all->size(); i++){
          GenParticle gpmu = event.genparticles_all->at(i);
          if (abs(gpmu.pdgid()) != 13 || !gpmu.get_statusflag(GenParticle::isLastCopy)) continue;
          if ((int)i == gpidx) continue;
          hdrgendimufromhad->Fill(deltaR(gp, gpmu), weight);
          hdphigendimufromhad->Fill(deltaPhi(gp, gpmu), weight);
          hdetagendimufromhad->Fill(deltaEta(gp, gpmu), weight);
        }

        for(size_t i=0; i<event.genparticles_all->size(); i++){
          GenParticle gpe = event.genparticles_all->at(i);
          if (abs(gpe.pdgid()) != 11 || !gpe.get_statusflag(GenParticle::isLastCopy)) continue;
          if ((int)i == gpidx) continue;
          hdrgenmufromhade->Fill(deltaR(gp, gpe), weight);
          hdphigenmufromhade->Fill(deltaPhi(gp, gpe), weight);
          hdetagenmufromhade->Fill(deltaEta(gp, gpe), weight);
        }

        for(size_t i=0; i<event.genparticles_visibletaus->size(); i++){
          GenParticle gvt = event.genparticles_visibletaus->at(i);
          hdrgenmufromhadvistau->Fill(deltaR(gp, gvt), weight);
          hdphigenmufromhadvistau->Fill(deltaPhi(gp, gvt), weight);
          hdetagenmufromhadvistau->Fill(deltaEta(gp, gvt), weight);
        }

        if(dr_min < 0.2 && dr_min > 0.){
          hgenmusfromhadmatched->Fill(0., weight);
          hptgenmufromhadmatched->Fill(gp.pt(), weight);
          hptgenmufromhadmatched_rebin->Fill(gp.pt(), weight);
          hptgenmufromhadmatched_rebin2->Fill(gp.pt(), weight);
        }
      }
      hgenmuorigin->Fill(origin, weight);

      if(n_genmus == 1){
        hgenmu1origin->Fill(origin, weight);
      }
      else if(n_genmus == 2){
        hgenmu2origin->Fill(origin, weight);
      }
    }
    else if(id == 11){ // electrons
      n_genels++;

      const Electron* closest_el = closestParticle(gp, *event.electrons);
      float dr_min = -1.;
      if(closest_el != nullptr) dr_min = deltaR(gp, *closest_el);
      int origin = 0.;
      if(gp.get_statusflag(GenParticle::isDirectHardProcessTauDecayProduct)) origin = 1.;
      else if(gp.get_statusflag(GenParticle::isDirectHadronDecayProduct)) origin = 2.;

      if(gp.get_statusflag(GenParticle::isDirectHardProcessTauDecayProduct)){
        hgenelsfromtautotal->Fill(0., weight);
        hptgenelfromtau->Fill(gp.pt(), weight);
        hptgenelfromtau_rebin->Fill(gp.pt(), weight);
        hptgenelfromtau_rebin2->Fill(gp.pt(), weight);

        for(size_t i=0; i<event.genparticles_all->size(); i++){
          GenParticle gpmu = event.genparticles_all->at(i);
          if (abs(gpmu.pdgid()) != 13 || !gpmu.get_statusflag(GenParticle::isLastCopy)) continue;
          if ((int)i == gpidx) continue;
          hdrgenefromtaumu->Fill(deltaR(gp, gpmu), weight);
          hdphigenefromtaumu->Fill(deltaPhi(gp, gpmu), weight);
          hdetagenefromtaumu->Fill(deltaEta(gp, gpmu), weight);
        }

        for(size_t i=0; i<event.genparticles_all->size(); i++){
          GenParticle gpe = event.genparticles_all->at(i);
          if (abs(gpe.pdgid()) != 11 || !gpe.get_statusflag(GenParticle::isLastCopy)) continue;
          if ((int)i == gpidx) continue;
          hdrgendiefromtau->Fill(deltaR(gp, gpe), weight);
          hdphigendiefromtau->Fill(deltaPhi(gp, gpe), weight);
          hdetagendiefromtau->Fill(deltaEta(gp, gpe), weight);
        }

        for(size_t i=0; i<event.genparticles_visibletaus->size(); i++){
          GenParticle gvt = event.genparticles_visibletaus->at(i);
          hdrgenefromtauvistau->Fill(deltaR(gp, gvt), weight);
          hdphigenefromtauvistau->Fill(deltaPhi(gp, gvt), weight);
          hdetagenefromtauvistau->Fill(deltaEta(gp, gvt), weight);
        }

        if(dr_min < 0.2 && dr_min > 0.){
          hgenelsfromtaumatched->Fill(0., weight);
          hptgenelfromtaumatched->Fill(gp.pt(), weight);
          hptgenelfromtaumatched_rebin->Fill(gp.pt(), weight);
          hptgenelfromtaumatched_rebin2->Fill(gp.pt(), weight);
        }
      }
      else if(gp.get_statusflag(GenParticle::isDirectHadronDecayProduct)){
        hgenelsfromhadtotal->Fill(0., weight);
        hptgenelfromhad->Fill(gp.pt(), weight);
        hptgenelfromhad_rebin->Fill(gp.pt(), weight);
        hptgenelfromhad_rebin2->Fill(gp.pt(), weight);

        for(size_t i=0; i<event.genparticles_all->size(); i++){
          GenParticle gpmu = event.genparticles_all->at(i);
          if (abs(gpmu.pdgid()) != 13 || !gpmu.get_statusflag(GenParticle::isLastCopy)) continue;
          if ((int)i == gpidx) continue;
          hdrgendiefromhad->Fill(deltaR(gp, gpmu), weight);
          hdphigendiefromhad->Fill(deltaPhi(gp, gpmu), weight);
          hdetagendiefromhad->Fill(deltaEta(gp, gpmu), weight);
        }

        for(size_t i=0; i<event.genparticles_all->size(); i++){
          GenParticle gpe = event.genparticles_all->at(i);
          if (abs(gpe.pdgid()) != 11 || !gpe.get_statusflag(GenParticle::isLastCopy)) continue;
          if ((int)i == gpidx) continue;
          hdrgenefromhadmu->Fill(deltaR(gp, gpe), weight);
          hdphigenefromhadmu->Fill(deltaPhi(gp, gpe), weight);
          hdetagenefromhadmu->Fill(deltaEta(gp, gpe), weight);
        }

        for(size_t i=0; i<event.genparticles_visibletaus->size(); i++){
          GenParticle gvt = event.genparticles_visibletaus->at(i);
          hdrgenefromhadvistau->Fill(deltaR(gp, gvt), weight);
          hdphigenefromhadvistau->Fill(deltaPhi(gp, gvt), weight);
          hdetagenefromhadvistau->Fill(deltaEta(gp, gvt), weight);
        }

        if(dr_min < 0.2 && dr_min > 0.){
          hgenelsfromhadmatched->Fill(0., weight);
          hptgenelfromhadmatched->Fill(gp.pt(), weight);
          hptgenelfromhadmatched_rebin->Fill(gp.pt(), weight);
          hptgenelfromhadmatched_rebin2->Fill(gp.pt(), weight);
        }
      }

      hgenelorigin->Fill(origin, weight);

      if(n_genels == 1){
        hgenel1origin->Fill(origin, weight);
      }
      else if(n_genels == 2){
        hgenel2origin->Fill(origin, weight);
      }
    }
    else if(id == 5){ // b quarks
      n_genbs++;



      const Jet* closest_jet = closestParticle(gp, *event.jets);
      float dr_min = -1.;
      if(closest_jet != nullptr) dr_min = deltaR(gp, *closest_jet);
      int origin = 0.;
      if(gp.get_statusflag(GenParticle::fromHardProcess)){
        origin = 1.;
      }
      else origin = 2.;

      if(gp.get_statusflag(GenParticle::fromHardProcess)){
        hgenbsfromhardtotal->Fill(0., weight);
        hptgenbfromhard->Fill(gp.pt(), weight);
        hptgenbfromhard_rebin->Fill(gp.pt(), weight);
        hptgenbfromhard_rebin2->Fill(gp.pt(), weight);

        for(size_t i=0; i<event.genparticles_all->size(); i++){
          GenParticle gpmu = event.genparticles_all->at(i);
          if (abs(gpmu.pdgid()) != 13 || !gpmu.get_statusflag(GenParticle::isLastCopy)) continue;
          if ((int)i == gpidx) continue;
          hdrgenbfromhardmu->Fill(deltaR(gp, gpmu), weight);
          hdphigenbfromhardmu->Fill(deltaPhi(gp, gpmu), weight);
          hdetagenbfromhardmu->Fill(deltaEta(gp, gpmu), weight);
        }

        for(size_t i=0; i<event.genparticles_all->size(); i++){
          GenParticle gpe = event.genparticles_all->at(i);
          if (abs(gpe.pdgid()) != 11 || !gpe.get_statusflag(GenParticle::isLastCopy)) continue;
          if ((int)i == gpidx) continue;
          hdrgenbfromharde->Fill(deltaR(gp, gpe), weight);
          hdphigenbfromharde->Fill(deltaPhi(gp, gpe), weight);
          hdetagenbfromharde->Fill(deltaEta(gp, gpe), weight);
        }

        for(size_t i=0; i<event.genparticles_visibletaus->size(); i++){
          GenParticle gvt = event.genparticles_visibletaus->at(i);
          hdrgenbfromhardvistau->Fill(deltaR(gp, gvt), weight);
          hdphigenbfromhardvistau->Fill(deltaPhi(gp, gvt), weight);
          hdetagenbfromhardvistau->Fill(deltaEta(gp, gvt), weight);
        }

        for(size_t i=gpidx+1; i<event.genparticles_all->size(); i++){
          GenParticle gpb = event.genparticles_all->at(i);
          if (abs(gpb.pdgid()) != 5 || !gpb.get_statusflag(GenParticle::isLastCopy) || !gpb.get_statusflag(GenParticle::fromHardProcess)) continue;
          hdrgendibfromhard->Fill(deltaR(gp, gpb), weight);
          hdphigendibfromhard->Fill(deltaPhi(gp, gpb), weight);
          hdetagendibfromhard->Fill(deltaEta(gp, gpb), weight);
        }

        if(dr_min < 0.4 && dr_min > 0.){
          hgenbsfromhardmatched->Fill(0., weight);
          hptgenbfromhardmatched->Fill(gp.pt(), weight);
          hptgenbfromhardmatched_rebin->Fill(gp.pt(), weight);
          hptgenbfromhardmatched_rebin2->Fill(gp.pt(), weight);
        }
      }
      else{
        hgenbsnotfromhardtotal->Fill(0., weight);
        hptgenbnotfromhard->Fill(gp.pt(), weight);
        hptgenbnotfromhard_rebin->Fill(gp.pt(), weight);
        hptgenbnotfromhard_rebin2->Fill(gp.pt(), weight);

        for(size_t i=0; i<event.genparticles_all->size(); i++){
          GenParticle gpmu = event.genparticles_all->at(i);
          if (abs(gpmu.pdgid()) != 13 || !gpmu.get_statusflag(GenParticle::isLastCopy)) continue;
          if ((int)i == gpidx) continue;
          hdrgenbnotfromhardmu->Fill(deltaR(gp, gpmu), weight);
          hdphigenbnotfromhardmu->Fill(deltaPhi(gp, gpmu), weight);
          hdetagenbnotfromhardmu->Fill(deltaEta(gp, gpmu), weight);
        }

        for(size_t i=0; i<event.genparticles_all->size(); i++){
          GenParticle gpe = event.genparticles_all->at(i);
          if (abs(gpe.pdgid()) != 11 || !gpe.get_statusflag(GenParticle::isLastCopy)) continue;
          if ((int)i == gpidx) continue;
          hdrgenbnotfromharde->Fill(deltaR(gp, gpe), weight);
          hdphigenbnotfromharde->Fill(deltaPhi(gp, gpe), weight);
          hdetagenbnotfromharde->Fill(deltaEta(gp, gpe), weight);
        }

        for(size_t i=0; i<event.genparticles_visibletaus->size(); i++){
          GenParticle gvt = event.genparticles_visibletaus->at(i);
          hdrgenbnotfromhardvistau->Fill(deltaR(gp, gvt), weight);
          hdphigenbnotfromhardvistau->Fill(deltaPhi(gp, gvt), weight);
          hdetagenbnotfromhardvistau->Fill(deltaEta(gp, gvt), weight);
        }

        for(size_t i=gpidx+1; i<event.genparticles_all->size(); i++){
          GenParticle gpb = event.genparticles_all->at(i);
          if (abs(gpb.pdgid()) != 5 || !gpb.get_statusflag(GenParticle::isLastCopy) || gpb.get_statusflag(GenParticle::fromHardProcess)) continue;
          hdrgendibnotfromhard->Fill(deltaR(gp, gpb), weight);
          hdphigendibnotfromhard->Fill(deltaPhi(gp, gpb), weight);
          hdetagendibnotfromhard->Fill(deltaEta(gp, gpb), weight);
        }

        if(dr_min < 0.4 && dr_min > 0.){
          hgenbsnotfromhardmatched->Fill(0., weight);
          hptgenbnotfromhardmatched->Fill(gp.pt(), weight);
          hptgenbnotfromhardmatched_rebin->Fill(gp.pt(), weight);
          hptgenbnotfromhardmatched_rebin2->Fill(gp.pt(), weight);
        }
      }

      hgenborigin->Fill(origin, weight);

      if(n_genbs == 1){
        hgenb1origin->Fill(origin, weight);
      }
      else if(n_genbs == 2){
        hgenb2origin->Fill(origin, weight);
      }

    }

  }

}
