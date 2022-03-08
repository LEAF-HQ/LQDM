#include <TString.h>
#include <TFile.h>
#include <TChain.h>
#include <iostream>

#include "LEAF/Analyzer/include/BaseTool.h"
#include "LEAF/Analyzer/include/useful_functions.h"
#include "LEAF/Analyzer/include/constants.h"
#include <sys/stat.h>
#include "LEAF/Analyzer/include/Registry.h"
#include "LEAF/Analyzer/include/JetHists.h"
#include "LEAF/Analyzer/include/MuonHists.h"
#include "LEAF/Analyzer/include/ElectronHists.h"
#include "LEAF/Analyzer/include/TauHists.h"
#include "LEAF/Analyzer/include/GenParticleHists.h"
#include "LEAF/Analyzer/include/JetIds.h"
#include "LEAF/Analyzer/include/MuonIds.h"
#include "LEAF/Analyzer/include/ElectronIds.h"
#include "LEAF/Analyzer/include/TauIds.h"
#include "LEAF/Analyzer/include/JetCorrections.h"
#include "LEAF/Analyzer/include/LumiblockSelection.h"
#include "LEAF/Analyzer/include/NMuonSelection.h"
#include "LEAF/Analyzer/include/NElectronSelection.h"
#include "LEAF/Analyzer/include/NTauSelection.h"
#include "LEAF/Analyzer/include/NJetSelection.h"
#include "LEAF/Analyzer/include/MinvSelections.h"
#include "LEAF/Analyzer/include/FlagSelection.h"
#include "LEAF/Analyzer/include/STMETSelection.h"
#include "LEAF/Analyzer/include/METSelection.h"


#include "LEAF/Analyzer/include/ElectronScaleFactorApplicator.h"
#include "LEAF/Analyzer/include/MuonScaleFactorApplicator.h"
#include "LEAF/Analyzer/include/BTaggingScaleFactorApplicator.h"
#include "LEAF/Analyzer/include/BTaggingMCEfficiencyHists.h"


#include "LEAF/LQDM/include/LQDMEvent.h"
#include "LEAF/LQDM/include/LQDMPreselectionHists.h"

using namespace std;

class LQDMPsiPsiFullselectionTool : public BaseTool {

public:
  // Constructors, destructor
  LQDMPsiPsiFullselectionTool(const Config & cfg);
  ~LQDMPsiPsiFullselectionTool() = default;
  void ProcessDataset(const Config & cfg) override {LoopEvents<LQDMPsiPsiFullselectionTool, LQDMEvent>(cfg, event, *this);};
  virtual bool Process() override;
  void book_histograms(vector<TString>);
  void fill_histograms(TString);


private:
  LQDMEvent* event;

  // selections
  unique_ptr<NElectronSelection> nelectron_selection;
  unique_ptr<NElectronSelection> electronveto_selection;
  unique_ptr<NMuonSelection>     nmuon_selection;
  unique_ptr<NMuonSelection>     muonveto_selection;
  unique_ptr<NTauSelection>      nditau_selection;

  // constants
  TString year;
};



LQDMPsiPsiFullselectionTool::LQDMPsiPsiFullselectionTool(const Config & cfg) : BaseTool(cfg){

  event = new LQDMEvent();
  event->reset();

  year = cfg.dataset_year();

  // histfolders
  vector<TString> histtags = {"input", "much", "elch", "elmuch", "tach", "noch"};
  book_histograms(histtags);

  nmuon_selection.reset(new NMuonSelection(cfg, 1, 1));
  muonveto_selection.reset(new NMuonSelection(cfg, 0, 0));
  nelectron_selection.reset(new NElectronSelection(cfg, 1, 1));
  electronveto_selection.reset(new NElectronSelection(cfg, 0, 0));
  nditau_selection.reset(new NTauSelection(cfg, 2, 2));

}




bool LQDMPsiPsiFullselectionTool::Process(){
  // cout << endl << "+++++ NEW EVENT ++++++" << endl;

  // order all objecs in pT
  // sort_by_pt<GenParticle>(*event->genparticles_hard);
  sort_by_pt<GenParticle>(*event->genparticles_visibletaus);
  // sort_by_pt<GenParticle>(*event->genparticles_final);
  sort_by_pt<GenParticle>(*event->genparticles_all);
  sort_by_pt<GenJet>(*event->genjets);
  sort_by_pt<Jet>(*event->jets_ak4chs);
  sort_by_pt<Muon>(*event->muons);
  sort_by_pt<Electron>(*event->electrons);
  sort_by_pt<Tau>(*event->taus);
  fill_histograms("input");


  double stmet = event->met->pt();
  for (Jet & jet : *event->jets_ak4chs) stmet += jet.pt();
  for (Electron & e : *event->electrons) stmet += e.pt();
  for (Muon & mu : *event->muons) stmet += mu.pt();
  for (Tau & tau : *event->taus) stmet += tau.pt();


  if(nmuon_selection->passes(*event) && electronveto_selection->passes(*event)){
    fill_histograms("much");
  }
  else if(nelectron_selection->passes(*event) && muonveto_selection->passes(*event)){
    fill_histograms("elch");
  }
  else if(nelectron_selection->passes(*event) && nmuon_selection->passes(*event)){
    fill_histograms("elmuch");
  }
  else if(nditau_selection->passes(*event)){
    fill_histograms("tach");
  }
  else{
    fill_histograms("noch");
  }




  return false;
}











void LQDMPsiPsiFullselectionTool::book_histograms(vector<TString> tags){
  for(const TString & tag : tags){
    TString mytag = tag+"_General";
    book_HistFolder(mytag, new LQDMPreselectionHists(mytag));
    mytag = tag+"_Jets";
    book_HistFolder(mytag, new JetHists(mytag));
    mytag = tag+"_Muons";
    book_HistFolder(mytag, new MuonHists(mytag));
    mytag = tag+"_Electrons";
    book_HistFolder(mytag, new ElectronHists(mytag));
    mytag = tag+"_Taus";
    book_HistFolder(mytag, new TauHists(mytag));
    mytag = tag+"_GenParticles";
    book_HistFolder(mytag, new GenParticleHists(mytag));
  }
}

void LQDMPsiPsiFullselectionTool::fill_histograms(TString tag){
  TString mytag = tag+"_General";
  HistFolder<LQDMPreselectionHists>(mytag)->fill(*event);
  mytag = tag+"_Jets";
  HistFolder<JetHists>(mytag)->fill(*event);
  mytag = tag+"_Muons";
  HistFolder<MuonHists>(mytag)->fill(*event);
  mytag = tag+"_Electrons";
  HistFolder<ElectronHists>(mytag)->fill(*event);
  mytag = tag+"_Taus";
  HistFolder<TauHists>(mytag)->fill(*event);
  mytag = tag+"_GenParticles";
  HistFolder<GenParticleHists>(mytag)->fill(*event);
}




REGISTER_TOOL(LQDMPsiPsiFullselectionTool)
