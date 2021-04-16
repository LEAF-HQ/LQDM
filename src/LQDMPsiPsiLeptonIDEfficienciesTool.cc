#include <TString.h>
#include <TFile.h>
#include <TChain.h>
#include <iostream>

#include "Analyzer/include/BaseTool.h"
#include "Analyzer/include/useful_functions.h"
#include "Analyzer/include/constants.h"
#include <sys/stat.h>
#include "Analyzer/include/Registry.h"
#include "Analyzer/include/JetHists.h"
#include "Analyzer/include/MuonHists.h"
#include "Analyzer/include/ElectronHists.h"
#include "Analyzer/include/TauHists.h"
#include "Analyzer/include/GenParticleHists.h"
#include "Analyzer/include/JetIds.h"
#include "Analyzer/include/MuonIds.h"
#include "Analyzer/include/ElectronIds.h"
#include "Analyzer/include/TauIds.h"
#include "Analyzer/include/JetCorrections.h"
#include "Analyzer/include/LumiblockSelection.h"
#include "Analyzer/include/NMuonSelection.h"
#include "Analyzer/include/NElectronSelection.h"
#include "Analyzer/include/NTauSelection.h"
#include "Analyzer/include/NJetSelection.h"

#include "Analyzer/include/LumiWeightApplicator.h"


#include "Analyzer/LQDM/include/LQDMEvent.h"
#include "Analyzer/LQDM/include/LQDMPreselectionHists.h"

using namespace std;

class LQDMPsiPsiLeptonIDEfficienciesTool : public BaseTool {

public:
  // Constructors, destructor
  LQDMPsiPsiLeptonIDEfficienciesTool(const Config & cfg);
  ~LQDMPsiPsiLeptonIDEfficienciesTool() = default;
  void ProcessDataset(const Config & cfg) override {LoopEvents<LQDMPsiPsiLeptonIDEfficienciesTool, LQDMEvent>(cfg, event, *this);};
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
  unique_ptr<TauCleaner> cleaner_tau;
  unique_ptr<MuonCleaner> cleaner_muon_cutsoft, cleaner_muon_cutloose, cleaner_muon_cutmedium, cleaner_muon_cuttight, cleaner_muon_mvasoft, cleaner_muon_mvaloose, cleaner_muon_mvamedium, cleaner_muon_mvatight;
  unique_ptr<ElectronCleaner> cleaner_electron_cutveto, cleaner_electron_cutloose, cleaner_electron_cutmedium, cleaner_electron_cuttight, cleaner_electron_cutheep, cleaner_electron_mvaisoloose, cleaner_electron_mvaiso90, cleaner_electron_mvaiso80, cleaner_electron_mvanonisoloose, cleaner_electron_mvanoniso90, cleaner_electron_mvanoniso80;

  // selections
  unique_ptr<LumiblockSelection> lumiblock_selection;

  // constants
  TString year;
};



LQDMPsiPsiLeptonIDEfficienciesTool::LQDMPsiPsiLeptonIDEfficienciesTool(const Config & cfg) : BaseTool(cfg){

  event = new LQDMEvent();
  event->reset();

  year = cfg.dataset_year();

  MultiID<Muon> muon_id_cutsoft = {PtEtaId(3, 2.4), MuonID(Muon::IDCutBasedSoft)};
  MultiID<Muon> muon_id_cutloose = {PtEtaId(3, 2.4), MuonID(Muon::IDCutBasedLoose)};
  MultiID<Muon> muon_id_cutmedium = {PtEtaId(3, 2.4), MuonID(Muon::IDCutBasedMedium)};
  MultiID<Muon> muon_id_cuttight = {PtEtaId(3, 2.4), MuonID(Muon::IDCutBasedTight)};
  MultiID<Muon> muon_id_mvasoft = {PtEtaId(3, 2.4), MuonID(Muon::IDMvaSoft)};
  MultiID<Muon> muon_id_mvaloose = {PtEtaId(3, 2.4), MuonID(Muon::IDMvaLoose)};
  MultiID<Muon> muon_id_mvamedium = {PtEtaId(3, 2.4), MuonID(Muon::IDMvaMedium)};
  MultiID<Muon> muon_id_mvatight = {PtEtaId(3, 2.4), MuonID(Muon::IDMvaTight)};

  MultiID<Electron> electron_id_cutveto = {PtEtaId(5, 2.4), ElectronID(Electron::IDCutBasedVeto)};
  MultiID<Electron> electron_id_cutloose = {PtEtaId(5, 2.4), ElectronID(Electron::IDCutBasedLoose)};
  MultiID<Electron> electron_id_cutmedium = {PtEtaId(5, 2.4), ElectronID(Electron::IDCutBasedMedium)};
  MultiID<Electron> electron_id_cuttight = {PtEtaId(5, 2.4), ElectronID(Electron::IDCutBasedTight)};
  MultiID<Electron> electron_id_cutheep = {PtEtaId(5, 2.4), ElectronID(Electron::IDCutBasedHEEP)};
  MultiID<Electron> electron_id_mvaisoloose = {PtEtaId(5, 2.4), ElectronID(Electron::IDMVAIsoLoose)};
  MultiID<Electron> electron_id_mvaiso90 = {PtEtaId(5, 2.4), ElectronID(Electron::IDMVAIsoEff90)};
  MultiID<Electron> electron_id_mvaiso80 = {PtEtaId(5, 2.4), ElectronID(Electron::IDMVAIsoEff80)};
  MultiID<Electron> electron_id_mvanonisoloose = {PtEtaId(5, 2.4), ElectronID(Electron::IDMVANonIsoLoose)};
  MultiID<Electron> electron_id_mvanoniso90 = {PtEtaId(5, 2.4), ElectronID(Electron::IDMVANonIsoEff90)};
  MultiID<Electron> electron_id_mvanoniso80 = {PtEtaId(5, 2.4), ElectronID(Electron::IDMVANonIsoEff80)};

  MultiID<Tau> tau_id = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsJetVVLoose), TauID(Tau::DeepTauVsEleVVLoose), TauID(Tau::DeepTauVsMuLoose)};
  MultiID<Jet> jet_id = {PtEtaId(15, 2.5), JetID(JetID::WP_TIGHT), JetPUID(JetPUID::WP_TIGHT)};
  MultiID<Jet> jet_overlapid = {JetTauOverlapID(0.5)};

  cleaner_muon_cutsoft.reset(new MuonCleaner(muon_id_cutsoft));
  cleaner_muon_cutloose.reset(new MuonCleaner(muon_id_cutloose));
  cleaner_muon_cutmedium.reset(new MuonCleaner(muon_id_cutmedium));
  cleaner_muon_cuttight.reset(new MuonCleaner(muon_id_cuttight));
  cleaner_muon_mvasoft.reset(new MuonCleaner(muon_id_mvasoft));
  cleaner_muon_mvaloose.reset(new MuonCleaner(muon_id_mvaloose));
  cleaner_muon_mvamedium.reset(new MuonCleaner(muon_id_mvamedium));
  cleaner_muon_mvatight.reset(new MuonCleaner(muon_id_mvatight));

  cleaner_electron_cutveto.reset(new ElectronCleaner(electron_id_cutveto));
  cleaner_electron_cutloose.reset(new ElectronCleaner(electron_id_cutloose));
  cleaner_electron_cutmedium.reset(new ElectronCleaner(electron_id_cutmedium));
  cleaner_electron_cuttight.reset(new ElectronCleaner(electron_id_cuttight));
  cleaner_electron_cutheep.reset(new ElectronCleaner(electron_id_cutheep));
  cleaner_electron_mvaisoloose.reset(new ElectronCleaner(electron_id_mvaisoloose));
  cleaner_electron_mvaiso90.reset(new ElectronCleaner(electron_id_mvaiso90));
  cleaner_electron_mvaiso80.reset(new ElectronCleaner(electron_id_mvaiso80));
  cleaner_electron_mvanonisoloose.reset(new ElectronCleaner(electron_id_mvanonisoloose));
  cleaner_electron_mvanoniso90.reset(new ElectronCleaner(electron_id_mvanoniso90));
  cleaner_electron_mvanoniso80.reset(new ElectronCleaner(electron_id_mvanoniso80));

  cleaner_tau.reset(new TauCleaner(tau_id));
  cleaner_jet.reset(new JetCleaner(jet_id));
  cleaner_jettauoverlap.reset(new JetCleaner(jet_overlapid));

  lumiweight_applicator.reset(new LumiWeightApplicator(cfg));


  // histfolders
  vector<TString> histtags = {"input", "corrector", "hadcleaner", "jettaucleaner", "iddenominator"};
  book_histograms(histtags);


  book_HistFolder("muon_cutsoft_Muons", new MuonHists("muon_cutsoft_Muons"));
  book_HistFolder("muon_cutloose_Muons", new MuonHists("muon_cutloose_Muons"));
  book_HistFolder("muon_cutmedium_Muons", new MuonHists("muon_cutmedium_Muons"));
  book_HistFolder("muon_cuttight_Muons", new MuonHists("muon_cuttight_Muons"));
  book_HistFolder("muon_mvasoft_Muons", new MuonHists("muon_mvasoft_Muons"));
  book_HistFolder("muon_mvaloose_Muons", new MuonHists("muon_mvaloose_Muons"));
  book_HistFolder("muon_mvamedium_Muons", new MuonHists("muon_mvamedium_Muons"));
  book_HistFolder("muon_mvatight_Muons", new MuonHists("muon_mvatight_Muons"));
  book_HistFolder("muon_cutsoft_GenParticles", new GenParticleHists("muon_cutsoft_GenParticles"));
  book_HistFolder("muon_cutloose_GenParticles", new GenParticleHists("muon_cutloose_GenParticles"));
  book_HistFolder("muon_cutmedium_GenParticles", new GenParticleHists("muon_cutmedium_GenParticles"));
  book_HistFolder("muon_cuttight_GenParticles", new GenParticleHists("muon_cuttight_GenParticles"));
  book_HistFolder("muon_mvasoft_GenParticles", new GenParticleHists("muon_mvasoft_GenParticles"));
  book_HistFolder("muon_mvaloose_GenParticles", new GenParticleHists("muon_mvaloose_GenParticles"));
  book_HistFolder("muon_mvamedium_GenParticles", new GenParticleHists("muon_mvamedium_GenParticles"));
  book_HistFolder("muon_mvatight_GenParticles", new GenParticleHists("muon_mvatight_GenParticles"));

  book_HistFolder("electron_cutveto_Electrons", new ElectronHists("electron_cutveto_Electrons"));
  book_HistFolder("electron_cutloose_Electrons", new ElectronHists("electron_cutloose_Electrons"));
  book_HistFolder("electron_cutmedium_Electrons", new ElectronHists("electron_cutmedium_Electrons"));
  book_HistFolder("electron_cuttight_Electrons", new ElectronHists("electron_cuttight_Electrons"));
  book_HistFolder("electron_cutheep_Electrons", new ElectronHists("electron_cutheep_Electrons"));
  book_HistFolder("electron_mvaisoloose_Electrons", new ElectronHists("electron_mvaisoloose_Electrons"));
  book_HistFolder("electron_mvaiso90_Electrons", new ElectronHists("electron_mvaiso90_Electrons"));
  book_HistFolder("electron_mvaiso80_Electrons", new ElectronHists("electron_mvaiso80_Electrons"));
  book_HistFolder("electron_mvanonisoloose_Electrons", new ElectronHists("electron_mvanonisoloose_Electrons"));
  book_HistFolder("electron_mvanoniso90_Electrons", new ElectronHists("electron_mvanoniso90_Electrons"));
  book_HistFolder("electron_mvanoniso80_Electrons", new ElectronHists("electron_mvanoniso80_Electrons"));
  book_HistFolder("electron_cutveto_GenParticles", new GenParticleHists("electron_cutveto_GenParticles"));
  book_HistFolder("electron_cutloose_GenParticles", new GenParticleHists("electron_cutloose_GenParticles"));
  book_HistFolder("electron_cutmedium_GenParticles", new GenParticleHists("electron_cutmedium_GenParticles"));
  book_HistFolder("electron_cuttight_GenParticles", new GenParticleHists("electron_cuttight_GenParticles"));
  book_HistFolder("electron_cutheep_GenParticles", new GenParticleHists("electron_cutheep_GenParticles"));
  book_HistFolder("electron_mvaisoloose_GenParticles", new GenParticleHists("electron_mvaisoloose_GenParticles"));
  book_HistFolder("electron_mvaiso90_GenParticles", new GenParticleHists("electron_mvaiso90_GenParticles"));
  book_HistFolder("electron_mvaiso80_GenParticles", new GenParticleHists("electron_mvaiso80_GenParticles"));
  book_HistFolder("electron_mvanonisoloose_GenParticles", new GenParticleHists("electron_mvanonisoloose_GenParticles"));
  book_HistFolder("electron_mvanoniso90_GenParticles", new GenParticleHists("electron_mvanoniso90_GenParticles"));
  book_HistFolder("electron_mvanoniso80_GenParticles", new GenParticleHists("electron_mvanoniso80_GenParticles"));


  jet_lepton_cleaner.reset(new JetLeptonCleaner(cfg, year, "AK4PFchs"));
  jet_lepton_cleaner->set_muon_id(MuonID(Muon::IDCutBasedSoft)); // something very loose to get away most OK muons
  jet_lepton_cleaner->set_electron_id(ElectronID(Electron::IDMVANonIsoEff90)); // something very loose to get away most OK muons
  jec_corrector.reset(new JECCorrector(cfg, year, "AK4PFchs"));
  jer_corrector.reset(new JERCorrector(cfg, JERCFiles("JER", "MC", JERC.at((string)year).at("JER"), "AK4PFchs").at(0), JERCFiles("JER", "MC", JERC.at((string)year).at("JER"), "AK4PFchs").at(1)));

  lumiblock_selection.reset(new LumiblockSelection(cfg));
}




bool LQDMPsiPsiLeptonIDEfficienciesTool::Process(){
  // cout << "++++++++++ NEW EVENT ++++++++++" << endl;
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
  cleaner_tau->process(*event);
  cleaner_jet->process(*event);
  fill_histograms("hadcleaner");

  cleaner_jettauoverlap->process(*event);
  fill_histograms("jettaucleaner");

  fill_histograms("iddenominator");

  // Here, fill all histograms after each cleaning, compare to 'jettaucleaner' as denominator
  vector<Muon> original_muons = *event->muons;
  cleaner_muon_cutsoft->process(*event);
  HistFolder<MuonHists>("muon_cutsoft_Muons")->fill(*event);
  HistFolder<GenParticleHists>("muon_cutsoft_GenParticles")->fill(*event);
  *event->muons = original_muons;

  cleaner_muon_cutloose->process(*event);
  HistFolder<MuonHists>("muon_cutloose_Muons")->fill(*event);
  HistFolder<GenParticleHists>("muon_cutloose_GenParticles")->fill(*event);
  *event->muons = original_muons;

  cleaner_muon_cutmedium->process(*event);
  HistFolder<MuonHists>("muon_cutmedium_Muons")->fill(*event);
  HistFolder<GenParticleHists>("muon_cutmedium_GenParticles")->fill(*event);
  *event->muons = original_muons;

  cleaner_muon_cuttight->process(*event);
  HistFolder<MuonHists>("muon_cuttight_Muons")->fill(*event);
  HistFolder<GenParticleHists>("muon_cuttight_GenParticles")->fill(*event);
  *event->muons = original_muons;

  cleaner_muon_mvasoft->process(*event);
  HistFolder<MuonHists>("muon_mvasoft_Muons")->fill(*event);
  HistFolder<GenParticleHists>("muon_mvasoft_GenParticles")->fill(*event);
  *event->muons = original_muons;

  cleaner_muon_mvaloose->process(*event);
  HistFolder<MuonHists>("muon_mvaloose_Muons")->fill(*event);
  HistFolder<GenParticleHists>("muon_mvaloose_GenParticles")->fill(*event);
  *event->muons = original_muons;

  cleaner_muon_mvamedium->process(*event);
  HistFolder<MuonHists>("muon_mvamedium_Muons")->fill(*event);
  HistFolder<GenParticleHists>("muon_mvamedium_GenParticles")->fill(*event);
  *event->muons = original_muons;

  cleaner_muon_mvatight->process(*event);
  HistFolder<MuonHists>("muon_mvatight_Muons")->fill(*event);
  HistFolder<GenParticleHists>("muon_mvatight_GenParticles")->fill(*event);
  *event->muons = original_muons;






  vector<Electron> original_electrons = *event->electrons;

  cleaner_electron_cutveto->process(*event);
  HistFolder<ElectronHists>("electron_cutveto_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("electron_cutveto_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_cutloose->process(*event);
  HistFolder<ElectronHists>("electron_cutloose_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("electron_cutloose_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_cutmedium->process(*event);
  HistFolder<ElectronHists>("electron_cutmedium_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("electron_cutmedium_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_cuttight->process(*event);
  HistFolder<ElectronHists>("electron_cuttight_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("electron_cuttight_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_cutheep->process(*event);
  HistFolder<ElectronHists>("electron_cutheep_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("electron_cutheep_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_mvaisoloose->process(*event);
  HistFolder<ElectronHists>("electron_mvaisoloose_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("electron_mvaisoloose_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_mvaiso90->process(*event);
  HistFolder<ElectronHists>("electron_mvaiso90_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("electron_mvaiso90_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_mvaiso80->process(*event);
  HistFolder<ElectronHists>("electron_mvaiso80_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("electron_mvaiso80_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_mvanonisoloose->process(*event);
  HistFolder<ElectronHists>("electron_mvanonisoloose_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("electron_mvanonisoloose_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_mvanoniso90->process(*event);
  HistFolder<ElectronHists>("electron_mvanoniso90_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("electron_mvanoniso90_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_mvanoniso80->process(*event);
  HistFolder<ElectronHists>("electron_mvanoniso80_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("electron_mvanoniso80_GenParticles")->fill(*event);
  *event->electrons = original_electrons;







  return false;
}











void LQDMPsiPsiLeptonIDEfficienciesTool::book_histograms(vector<TString> tags){
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

void LQDMPsiPsiLeptonIDEfficienciesTool::fill_histograms(TString tag){
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




REGISTER_TOOL(LQDMPsiPsiLeptonIDEfficienciesTool)
