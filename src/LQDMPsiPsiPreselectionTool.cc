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
#include "LEAF/Analyzer/include/FlagHists.h"
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
#include "LEAF/Analyzer/include/FlagSelection.h"
#include "LEAF/Analyzer/include/METSelection.h"
#include "LEAF/Analyzer/include/MinvSelections.h"

#include "LEAF/Analyzer/include/LumiWeightApplicator.h"


#include "LEAF/LQDM/include/LQDMEvent.h"
#include "LEAF/LQDM/include/LQDMPreselectionHists.h"

using namespace std;

class LQDMPsiPsiPreselectionTool : public BaseTool {

public:
  // Constructors, destructor
  LQDMPsiPsiPreselectionTool(const Config & cfg);
  ~LQDMPsiPsiPreselectionTool() = default;
  void ProcessDataset(const Config & cfg) override {LoopEvents<LQDMPsiPsiPreselectionTool, LQDMEvent>(cfg, event, *this);};
  virtual bool Process() override;
  void book_histograms(vector<TString>);
  void fill_histograms(TString);


private:
  LQDMEvent* event;

  // Modules used in the analysis
  unique_ptr<LumiWeightApplicator> lumiweight_applicator;

  // correctors
  unique_ptr<JetLeptonCleaner> jet_lepton_cleaner;
  unique_ptr<JECCorrector> jec_corrector;
  unique_ptr<JERCorrector> jer_corrector;

  // cleaners
  unique_ptr<JetCleaner> cleaner_jet;

  // selections
  unique_ptr<LumiblockSelection> lumiblock_selection;
  unique_ptr<METSelection> met_selection;

  unique_ptr<FlagSelection> trigger_selection_met1, trigger_selection_met2,  trigger_selection_met3,  trigger_selection_met4,  trigger_selection_met5,  trigger_selection_met6,  trigger_selection_met7,  trigger_selection_met8,  trigger_selection_met9,  trigger_selection_met10, trigger_selection_met11, trigger_selection_met12, trigger_selection_met13, trigger_selection_met14, trigger_selection_met15;

  // constants
  TString year;
};



LQDMPsiPsiPreselectionTool::LQDMPsiPsiPreselectionTool(const Config & cfg) : BaseTool(cfg){

  event = new LQDMEvent();
  event->reset();

  year = cfg.dataset_year();

  MultiID<Jet> jet_id = {PtEtaId(15, 2.4), JetID(JetID::WP_TIGHT), JetPUID(JetPUID::WP_TIGHT)};
  // MultiID<Jet> jet_overlapid = {JetTauOverlapID(0.5)};


  cleaner_jet.reset(new JetCleaner(jet_id));
  // cleaner_jettauoverlap.reset(new JetCleaner(jet_overlapid));



  lumiweight_applicator.reset(new LumiWeightApplicator(cfg));
  met_selection.reset(new METSelection(cfg, 130, -1));

  trigger_selection_met1.reset(new FlagSelection(cfg, "HLT_PFMET120_PFMHT120_IDTight_PFHT60_v*"));
  trigger_selection_met2.reset(new FlagSelection(cfg, "HLT_PFMET120_PFMHT120_IDTight_v*"));
  trigger_selection_met3.reset(new FlagSelection(cfg, "HLT_PFMET130_PFMHT130_IDTight_v*"));
  trigger_selection_met4.reset(new FlagSelection(cfg, "HLT_PFMET140_PFMHT140_IDTight_v*"));
  trigger_selection_met5.reset(new FlagSelection(cfg, "HLT_PFMET250_HBHECleaned_v*"));
  trigger_selection_met6.reset(new FlagSelection(cfg, "HLT_PFMET300_HBHECleaned_v*"));
  trigger_selection_met7.reset(new FlagSelection(cfg, "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60_v*"));
  trigger_selection_met8.reset(new FlagSelection(cfg, "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_v*"));
  trigger_selection_met9.reset(new FlagSelection(cfg, "HLT_PFMETNoMu130_PFMHTNoMu130_IDTight_v*"));
  trigger_selection_met10.reset(new FlagSelection(cfg, "HLT_PFMETNoMu140_PFMHTNoMu140_IDTight_v*"));
  trigger_selection_met11.reset(new FlagSelection(cfg, "HLT_PFMETTypeOne120_PFMHT120_IDTight_PFHT60_v*"));
  trigger_selection_met12.reset(new FlagSelection(cfg, "HLT_PFMETTypeOne120_PFMHT120_IDTight_v*"));
  trigger_selection_met13.reset(new FlagSelection(cfg, "HLT_PFMETTypeOne130_PFMHT130_IDTight_v*"));
  trigger_selection_met14.reset(new FlagSelection(cfg, "HLT_PFMETTypeOne140_PFMHT140_IDTight_v*"));
  trigger_selection_met15.reset(new FlagSelection(cfg, "HLT_PFMETTypeOne200_HBHE_BeamHaloCleaned_v*"));

  // histfolders
  vector<TString> histtags = {"input", "corrector", "jetcleaner", "trigger", "plateau", "final"};
  book_histograms(histtags);



  jet_lepton_cleaner.reset(new JetLeptonCleaner(cfg, year, "AK4PFchs"));
  jet_lepton_cleaner->set_muon_id(MuonID(Muon::IDCutBasedLoose)); // something very loose to get away most OK muons
  jet_lepton_cleaner->set_electron_id(ElectronID(Electron::IDMVANonIsoLoose)); // something very loose to get away most OK muons
  jec_corrector.reset(new JECCorrector(cfg, year, "AK4PFchs"));
  jer_corrector.reset(new JERCorrector(cfg, JERCFiles("JER", "MC", JERC_Info.at((string)year).at("JER_Version"), "AK4PFchs").at(0), JERCFiles("JER", "MC", JERC_Info.at((string)year).at("JER_Version"), "AK4PFchs").at(1)));

  lumiblock_selection.reset(new LumiblockSelection(cfg));
}




bool LQDMPsiPsiPreselectionTool::Process(){
  // cout << "++++++++++ NEW EVENT ++++++++++" << endl;
  if(!lumiblock_selection->passes(*event)) return false;
  lumiweight_applicator->process(*event);


  // order all objecs in pT
  sort_by_pt<GenParticle>(*event->genparticles_visibletaus);
  sort_by_pt<GenParticle>(*event->genparticles_pruned);
  sort_by_pt<GenJet>(*event->genjets);
  sort_by_pt<Jet>(*event->jets_ak4chs);
  sort_by_pt<Muon>(*event->muons);
  sort_by_pt<Electron>(*event->electrons);
  sort_by_pt<Tau>(*event->taus);
  fill_histograms("input");


  // correctors
  jet_lepton_cleaner->process(*event);
  jec_corrector->process(*event);
  jec_corrector->correct_met(*event);
  jer_corrector->process(*event);
  fill_histograms("corrector");


  // cleaners
  cleaner_jet->process(*event);
  // cleaner_jettauoverlap->process(*event);
  fill_histograms("jetcleaner");




  bool pass_hlt_met = (trigger_selection_met1->passes(*event) || trigger_selection_met2->passes(*event) || trigger_selection_met3->passes(*event) || trigger_selection_met4->passes(*event) || trigger_selection_met5->passes(*event) || trigger_selection_met6->passes(*event) || trigger_selection_met7->passes(*event) || trigger_selection_met8->passes(*event) || trigger_selection_met9->passes(*event) || trigger_selection_met10->passes(*event) || trigger_selection_met11->passes(*event) || trigger_selection_met12->passes(*event) || trigger_selection_met13->passes(*event) || trigger_selection_met14->passes(*event) || trigger_selection_met15->passes(*event));


  if(!pass_hlt_met) return false;
  fill_histograms("trigger");

  if(!met_selection->passes(*event)) return false;
  fill_histograms("plateau");

  fill_histograms("final");


  return true;
}











void LQDMPsiPsiPreselectionTool::book_histograms(vector<TString> tags){
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

void LQDMPsiPsiPreselectionTool::fill_histograms(TString tag){
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




REGISTER_TOOL(LQDMPsiPsiPreselectionTool)
