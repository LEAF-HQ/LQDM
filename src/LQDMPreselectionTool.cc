#include <TString.h>
#include <TFile.h>
#include <TChain.h>
#include <iostream>

#include "include/BaseTool.h"
#include "include/useful_functions.h"
#include "include/constants.h"
#include <sys/stat.h>
#include "include/Registry.h"
#include "include/JetHists.h"
#include "include/MuonHists.h"
#include "include/ElectronHists.h"
#include "include/TauHists.h"
#include "include/JetIds.h"
#include "include/MuonIds.h"
#include "include/ElectronIds.h"
#include "include/TauIds.h"
#include "include/JetCorrections.h"
#include "include/LumiblockSelection.h"
#include "include/NMuonSelection.h"
#include "include/NElectronSelection.h"
#include "include/NTauSelection.h"
#include "include/NJetSelection.h"

#include "include/LumiWeightApplicator.h"


#include "LQDM/include/LQDMEvent.h"
#include "LQDM/include/LQDMPreselectionHists.h"

using namespace std;

class LQDMPreselectionTool : public BaseTool {

public:
  // Constructors, destructor
  LQDMPreselectionTool(const Config & cfg);
  ~LQDMPreselectionTool() = default;
  void ProcessDataset(const Config & cfg) override {LoopEvents<LQDMPreselectionTool, LQDMEvent>(cfg, event, *this);};
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
  unique_ptr<JetCleaner> cleaner_jettauoverlap;
  unique_ptr<MuonCleaner> cleaner_muon;
  unique_ptr<ElectronCleaner> cleaner_electron;
  unique_ptr<TauCleaner> cleaner_tau;

  // selections
  unique_ptr<LumiblockSelection> lumiblock_selection;
  unique_ptr<NElectronSelection> nelectron_selection, nelectron_veto_selection;
  unique_ptr<NMuonSelection>     nmuon_selection, nmuon_veto_selection;
  unique_ptr<NTauSelection>      ntau_selection;
  unique_ptr<NJetSelection>      njet_selection;

  // constants
  TString year;
};



LQDMPreselectionTool::LQDMPreselectionTool(const Config & cfg) : BaseTool(cfg){

  event = new LQDMEvent();
  event->reset();

  year = cfg.dataset_year();

  MultiID<Muon> muon_id = {PtEtaId(30, 2.4), MuonID(Muon::IDCutBasedTight), MuonID(Muon::IsoPFTight)};
  MultiID<Electron> electron_id = {PtEtaId(30, 2.4), ElectronID(Electron::IDMVAIsoEff90)};
  MultiID<Tau> tau_id = {PtEtaId(20, 2.1), TauID(Tau::DeepTauVsJetMedium), TauID(Tau::DeepTauVsEleVVLoose), TauID(Tau::DeepTauVsMuLoose)};
  MultiID<Jet> jet_id = {PtEtaId(30, 2.5), JetID(JetID::WP_TIGHT), JetPUID(JetPUID::WP_TIGHT)};
  MultiID<Jet> jet_overlapid = {JetTauOverlapID(0.5)};

  cleaner_muon.reset(new MuonCleaner(muon_id));
  cleaner_electron.reset(new ElectronCleaner(electron_id));
  cleaner_tau.reset(new TauCleaner(tau_id));
  cleaner_jet.reset(new JetCleaner(jet_id));
  cleaner_jettauoverlap.reset(new JetCleaner(jet_overlapid));

  lumiweight_applicator.reset(new LumiWeightApplicator(cfg));


  // histfolders
  vector<TString> histtags = {"input", "corrector", "cleaner", "jettaucleaner", "nleptons", "njets"};
  book_histograms(histtags);


  jet_lepton_cleaner.reset(new JetLeptonCleaner(cfg, year, "AK4PFchs"));
  jet_lepton_cleaner->set_muon_id(MuonID(Muon::IDCutBasedLoose)); // something very loose to get away most OK muons
  jet_lepton_cleaner->set_electron_id(ElectronID(Electron::IDCutBasedLoose)); // something very loose to get away most OK muons
  jec_corrector.reset(new JECCorrector(cfg, year, "AK4PFchs"));
  jer_corrector.reset(new JERCorrector(cfg, JERCFiles("JER", "MC", JERC.at((string)year).at("JER"), "AK4PFchs").at(0), JERCFiles("JER", "MC", JERC.at((string)year).at("JER"), "AK4PFchs").at(1)));

  lumiblock_selection.reset(new LumiblockSelection(cfg));
  nmuon_selection.reset(new NMuonSelection(cfg, 1, 1));
  nelectron_selection.reset(new NElectronSelection(cfg, 1, 1));
  nmuon_veto_selection.reset(new NMuonSelection(cfg, 0, 0));
  nelectron_veto_selection.reset(new NElectronSelection(cfg, 0, 0));
  ntau_selection.reset(new NTauSelection(cfg, 2, -1));
  njet_selection.reset(new NJetSelection(cfg, 1, -1));
}




bool LQDMPreselectionTool::Process(){
  if(!lumiblock_selection->passes(*event)) return false;
  lumiweight_applicator->process(*event);


  // order all objecs in pT
  sort_by_pt<GenParticle>(*event->genparticles_hard);
  sort_by_pt<GenParticle>(*event->genparticles_visibletaus);
  sort_by_pt<GenParticle>(*event->genparticles_final);
  sort_by_pt<GenJet>(*event->genjets);
  sort_by_pt<Jet>(*event->jets);
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
  cleaner_muon->process(*event);
  cleaner_electron->process(*event);
  cleaner_tau->process(*event);
  cleaner_jet->process(*event);
  fill_histograms("cleaner");

  cleaner_jettauoverlap->process(*event);
  fill_histograms("jettaucleaner");

  // ==1 ele || ==1 muon || (0e & 0mu & ==2tau)
  if(!nmuon_selection->passes(*event) && !nelectron_selection->passes(*event) && !(nmuon_veto_selection->passes(*event) && nelectron_veto_selection->passes(*event) && ntau_selection->passes(*event))) return false;
  fill_histograms("nleptons");

  if(!njet_selection->passes(*event)) return false;
  fill_histograms("njets");




  return true;
}











void LQDMPreselectionTool::book_histograms(vector<TString> tags){
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
  }
}

void LQDMPreselectionTool::fill_histograms(TString tag){
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
}




REGISTER_TOOL(LQDMPreselectionTool)
