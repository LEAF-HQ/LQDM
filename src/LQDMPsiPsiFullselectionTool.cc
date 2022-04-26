#include <TString.h>
#include <TFile.h>
#include <TChain.h>
#include <iostream>

#include "LEAF/Analyzer/include/BaseTool.h"
#include "LEAF/Analyzer/include/useful_functions.h"
#include "LEAF/Analyzer/include/constants.h"
#include <sys/stat.h>
#include "LEAF/Analyzer/include/Registry.h"
#include "LEAF/Analyzer/include/EventHists.h"
#include "LEAF/Analyzer/include/JetHists.h"
#include "LEAF/Analyzer/include/BJetHists.h"
#include "LEAF/Analyzer/include/MuonHists.h"
#include "LEAF/Analyzer/include/ElectronHists.h"
#include "LEAF/Analyzer/include/TauHists.h"
#include "LEAF/Analyzer/include/GenParticleHists.h"
#include "LEAF/Analyzer/include/GenInfoHists.h"
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
#include "LEAF/LQDM/include/LQDMGenParticleOriginHists.h"
#include "LEAF/LQDM/include/LQDMElectronOriginHists.h"
#include "LEAF/LQDM/include/LQDMMuonOriginHists.h"

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

  // cleaner
  unique_ptr<JetCleaner> cleaner_jettauoverlap;
  unique_ptr<MuonCleaner> cleaner_muon_pteta, cleaner_muon_id;
  unique_ptr<ElectronCleaner> cleaner_electron_pteta, cleaner_electron_id;
  unique_ptr<TauCleaner> cleaner_tau_pteta, cleaner_tau_id;

  // selections
  unique_ptr<NElectronSelection> nelectron_selection;
  unique_ptr<NElectronSelection> electronveto_selection;
  unique_ptr<NMuonSelection>     nmuon_selection;
  unique_ptr<NMuonSelection>     muonveto_selection;
  unique_ptr<NJetSelection>      njet1_selection, njet2_selection;
  unique_ptr<NJetSelection>      nbjetloose1_selection, nbjetloose2_selection, nbjetmedium1_selection, nbjetmedium2_selection, nbjettight1_selection, nbjettight2_selection;

  // constants
  TString year;
};



LQDMPsiPsiFullselectionTool::LQDMPsiPsiFullselectionTool(const Config & cfg) : BaseTool(cfg){

  event = new LQDMEvent();
  event->reset();

  year = cfg.dataset_year();


  MultiID<Jet> jet_overlapid = {JetTauOverlapID(0.5)};
  cleaner_jettauoverlap.reset(new JetCleaner(jet_overlapid));

  MultiID<Jet> jet_btaglooseid = {JetBTag(JetBTag::DeepCSV_Loose)};
  MultiID<Jet> jet_btagmediumid = {JetBTag(JetBTag::DeepCSV_Medium)};
  MultiID<Jet> jet_btagtightid = {JetBTag(JetBTag::DeepCSV_Tight)};
  nbjetloose1_selection.reset(new NJetSelection(cfg, 1, -1, jet_btaglooseid));
  nbjetloose2_selection.reset(new NJetSelection(cfg, 2, -1, jet_btaglooseid));
  nbjetmedium1_selection.reset(new NJetSelection(cfg, 1, -1, jet_btagmediumid));
  nbjetmedium2_selection.reset(new NJetSelection(cfg, 2, -1, jet_btagmediumid));
  nbjettight1_selection.reset(new NJetSelection(cfg, 1, -1, jet_btagtightid));
  nbjettight2_selection.reset(new NJetSelection(cfg, 2, -1, jet_btagtightid));


  MultiID<Muon> muon_id_pteta = {PtEtaId(3, 2.4)};
  MultiID<Muon> muon_id_cutsoft = {MuonID(Muon::IDCutBasedSoft)};
  cleaner_muon_pteta.reset(new MuonCleaner(muon_id_pteta));
  cleaner_muon_id.reset(new    MuonCleaner(muon_id_cutsoft));

  MultiID<Electron> electron_id_pteta = {PtEtaId(5, 2.4)};
  MultiID<Electron> electron_id_mvanoniso90 = {ElectronID(Electron::IDMVANonIsoEff90)};
  cleaner_electron_pteta.reset(new ElectronCleaner(electron_id_pteta));
  cleaner_electron_id.reset(new    ElectronCleaner(electron_id_mvanoniso90));

  MultiID<Tau> tau_id_pteta = {PtEtaId(20, 2.3)};
  MultiID<Tau> tau_id_loosest = {TauID(Tau::DeepTauVsJetVVVLoose), TauID(Tau::DeepTauVsEleVVVLoose), TauID(Tau::DeepTauVsMuVLoose)};
  cleaner_tau_pteta.reset(new TauCleaner(tau_id_pteta));
  cleaner_tau_id.reset(new    TauCleaner(tau_id_loosest));

  nmuon_selection.reset(new NMuonSelection(cfg, 1, -1));
  muonveto_selection.reset(new NMuonSelection(cfg, 0, 0));
  nelectron_selection.reset(new NElectronSelection(cfg, 1, -1));
  electronveto_selection.reset(new NElectronSelection(cfg, 0, 0));
  njet1_selection.reset(new NJetSelection(cfg, 1, -1));
  njet2_selection.reset(new NJetSelection(cfg, 2, -1));

  // histfolders
  vector<TString> histtags = {"input", "cleanerpteta", "cleanerid", "jettaucleaner", "jets1_nominal", "jets2_nominal", "bjetloose1_nominal", "bjetloose2_nominal", "bjetloose2medium1_nominal", "bjetmedium1_nominal", "bjetmedium2_nominal", "bjetmedium2tight1_nominal", "bjettight1_nominal", "bjettight2_nominal", "much_nominal", "elch_nominal", "noch_nominal", "nominal"};
  book_histograms(histtags);
}




bool LQDMPsiPsiFullselectionTool::Process(){
  // cout << endl << "+++++ NEW EVENT ++++++" << endl;

  // order all objecs in pT
  sort_by_pt<GenParticle>(*event->genparticles_visibletaus);
  sort_by_pt<GenParticle>(*event->genparticles_pruned);
  sort_by_pt<GenJet>(*event->genjets);
  sort_by_pt<Jet>(*event->jets_ak4chs);
  sort_by_pt<Muon>(*event->muons);
  sort_by_pt<Electron>(*event->electrons);
  sort_by_pt<Tau>(*event->taus);
  fill_histograms("input");

  cleaner_muon_pteta->process(*event);
  cleaner_electron_pteta->process(*event);
  cleaner_tau_pteta->process(*event);
  fill_histograms("cleanerpteta");

  cleaner_muon_id->process(*event);
  cleaner_electron_id->process(*event);
  cleaner_tau_id->process(*event);
  fill_histograms("cleanerid");

  cleaner_jettauoverlap->process(*event);
  fill_histograms("jettaucleaner");


  double stmet = event->met->pt();
  for (Jet & jet : *event->jets_ak4chs) stmet += jet.pt();
  for (Electron & e : *event->electrons) stmet += e.pt();
  for (Muon & mu : *event->muons) stmet += mu.pt();
  for (Tau & tau : *event->taus) stmet += tau.pt();

  if(!njet1_selection->passes(*event)) return false;
  fill_histograms("jets1_nominal");

  if(!njet2_selection->passes(*event)) return false;
  fill_histograms("jets2_nominal");

  if(!nbjetloose1_selection->passes(*event)) return false;
  fill_histograms("bjetloose1_nominal");

  if(nbjetloose2_selection->passes(*event)){
    fill_histograms("bjetloose2_nominal");

    if(nbjetmedium1_selection->passes(*event)){
      fill_histograms("bjetloose2medium1_nominal");
    }
  }
  if(nbjetmedium1_selection->passes(*event)){
    fill_histograms("bjetmedium1_nominal");
  }
  if(nbjetmedium2_selection->passes(*event)){
    fill_histograms("bjetmedium2_nominal");

    if(nbjettight1_selection->passes(*event)){
      fill_histograms("bjetmedium2tight1_nominal");
    }
  }
  if(nbjettight1_selection->passes(*event)){
    fill_histograms("bjettight1_nominal");
  }
  if(nbjettight2_selection->passes(*event)){
    fill_histograms("bjettight2_nominal");
  }

  if(nmuon_selection->passes(*event) && !nelectron_selection->passes(*event)){
    fill_histograms("much_nominal");
  }
  else if(nelectron_selection->passes(*event) && !nmuon_selection->passes(*event)){
    fill_histograms("elch_nominal");
  }
  else{
    fill_histograms("noch_nominal");
  }

  fill_histograms("nominal");

  return false;
}











void LQDMPsiPsiFullselectionTool::book_histograms(vector<TString> tags){
  for(const TString & tag : tags){
    TString mytag = tag+"_General";
    book_HistFolder(mytag, new LQDMPreselectionHists(mytag));
    mytag = tag+"_Event";
    book_HistFolder(mytag, new EventHists(mytag));
    mytag = tag+"_Jets";
    book_HistFolder(mytag, new JetHists(mytag));
    mytag = tag+"_BJets";
    book_HistFolder(mytag, new BJetHists(mytag));
    mytag = tag+"_Muons";
    book_HistFolder(mytag, new MuonHists(mytag));
    mytag = tag+"_MuonOrigins";
    book_HistFolder(mytag, new LQDMMuonOriginHists(mytag));
    mytag = tag+"_Electrons";
    book_HistFolder(mytag, new ElectronHists(mytag));
    mytag = tag+"_ElectronOrigins";
    book_HistFolder(mytag, new LQDMElectronOriginHists(mytag));
    mytag = tag+"_Taus";
    book_HistFolder(mytag, new TauHists(mytag));
    mytag = tag+"_GenParticles";
    book_HistFolder(mytag, new GenParticleHists(mytag));
    mytag = tag+"_GenParticleOrigins";
    book_HistFolder(mytag, new LQDMGenParticleOriginHists(mytag));
    mytag = tag+"_GenInfo";
    book_HistFolder(mytag, new GenInfoHists(mytag));
  }
}

void LQDMPsiPsiFullselectionTool::fill_histograms(TString tag){
  TString mytag = tag+"_General";
  HistFolder<LQDMPreselectionHists>(mytag)->fill(*event);
  mytag = tag+"_Event";
  HistFolder<EventHists>(mytag)->fill(*event);
  mytag = tag+"_Jets";
  HistFolder<JetHists>(mytag)->fill(*event);
  mytag = tag+"_BJets";
  HistFolder<BJetHists>(mytag)->fill(*event);
  mytag = tag+"_Muons";
  HistFolder<MuonHists>(mytag)->fill(*event);
  mytag = tag+"_MuonOrigins";
  HistFolder<LQDMMuonOriginHists>(mytag)->fill(*event);
  mytag = tag+"_Electrons";
  HistFolder<ElectronHists>(mytag)->fill(*event);
  mytag = tag+"_ElectronOrigins";
  HistFolder<LQDMElectronOriginHists>(mytag)->fill(*event);
  mytag = tag+"_Taus";
  HistFolder<TauHists>(mytag)->fill(*event);
  mytag = tag+"_GenParticles";
  HistFolder<GenParticleHists>(mytag)->fill(*event);
  mytag = tag+"_GenParticleOrigins";
  HistFolder<LQDMGenParticleOriginHists>(mytag)->fill(*event);
  mytag = tag+"_GenInfo";
  HistFolder<GenInfoHists>(mytag)->fill(*event);
}




REGISTER_TOOL(LQDMPsiPsiFullselectionTool)
