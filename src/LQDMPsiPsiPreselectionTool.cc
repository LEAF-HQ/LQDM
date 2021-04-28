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
  unique_ptr<JetCleaner> cleaner_jettauoverlap;
  unique_ptr<TauCleaner> cleaner_tau_pteta, cleaner_tau_vsjet, cleaner_tau_vse, cleaner_tau_vsmu;
  unique_ptr<MuonCleaner> cleaner_muon_pteta, cleaner_muon_mvasoft, cleaner_muon_mvasoft_ip;
  unique_ptr<ElectronCleaner> cleaner_electron_pteta, cleaner_electron_mvanonisoloose, cleaner_electron_mvanonisoloose_ip;

  // selections
  unique_ptr<LumiblockSelection> lumiblock_selection;
  unique_ptr<NMuonSelection> nmuon_selection;
  unique_ptr<NElectronSelection> nelectron_selection;
  unique_ptr<NTauSelection> ntau_selection;

  // constants
  TString year;
};



LQDMPsiPsiPreselectionTool::LQDMPsiPsiPreselectionTool(const Config & cfg) : BaseTool(cfg){

  event = new LQDMEvent();
  event->reset();

  year = cfg.dataset_year();

  MultiID<Muon> muon_id_pteta = {PtEtaId(3, 2.4)};
  MultiID<Muon> muon_id_mvasoft = {PtEtaId(3, 2.4), MuonID(Muon::IDMvaSoft)};
  MultiID<Muon> muon_id_mvasoft_ip = {PtEtaId(3, 2.4), MuonID(Muon::IDMvaSoft), MuonIPID(0., 0.01)};


  MultiID<Electron> electron_id_pteta = {PtEtaId(5, 2.4)};
  MultiID<Electron> electron_id_mvanonisoloose = {PtEtaId(5, 2.4), ElectronID(Electron::IDMVANonIsoLoose)};
  MultiID<Electron> electron_id_mvanonisoloose_ip = {PtEtaId(5, 2.4), ElectronID(Electron::IDMVANonIsoLoose), ElectronIPID(0., 0.01)};

  MultiID<Tau> tau_id_pteta = {PtEtaId(18, 2.1)};
  MultiID<Tau> tau_id_vsjet = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsJetVVVLoose)};
  MultiID<Tau> tau_id_vse = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsEleVLoose)};
  MultiID<Tau> tau_id_vsmu = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsMuTight)};
  MultiID<Jet> jet_id = {PtEtaId(15, 2.5), JetID(JetID::WP_TIGHT), JetPUID(JetPUID::WP_TIGHT)};
  MultiID<Jet> jet_overlapid = {JetTauOverlapID(0.5)};


  // cleaner_tau.reset(new TauCleaner(tau_id));
  cleaner_jet.reset(new JetCleaner(jet_id));
  cleaner_jettauoverlap.reset(new JetCleaner(jet_overlapid));

  cleaner_muon_pteta.reset(new MuonCleaner(muon_id_pteta));
  cleaner_electron_pteta.reset(new ElectronCleaner(electron_id_pteta));
  cleaner_tau_pteta.reset(new TauCleaner(tau_id_pteta));

  cleaner_muon_mvasoft.reset(new MuonCleaner(muon_id_mvasoft));
  cleaner_electron_mvanonisoloose.reset(new ElectronCleaner(electron_id_mvanonisoloose));

  cleaner_muon_mvasoft_ip.reset(new MuonCleaner(muon_id_mvasoft_ip));
  cleaner_electron_mvanonisoloose_ip.reset(new ElectronCleaner(electron_id_mvanonisoloose_ip));

  cleaner_tau_vsjet.reset(new TauCleaner(tau_id_vsjet));
  cleaner_tau_vse.reset(new TauCleaner(tau_id_vse));
  cleaner_tau_vsmu.reset(new TauCleaner(tau_id_vsmu));


  lumiweight_applicator.reset(new LumiWeightApplicator(cfg));

  nmuon_selection.reset(new NMuonSelection(cfg, 1, -1));
  nelectron_selection.reset(new NElectronSelection(cfg, 1, -1));
  ntau_selection.reset(new NTauSelection(cfg, 1, -1));


  // histfolders
  vector<TString> histtags = {"input", "corrector", "hadcleaner", "jettaucleaner", "iddenominator", "ptetacleaner", "pteta1muon", "pteta1electron", "pteta1tau", "pteta1lepton", "mvacleaner", "mva1muon", "mva1electron", "mva1lepton", "mvaipcleaner", "mvaip1muon", "mvaip1electron", "mvaip1lepton", "tauvsmucleaner", "tauvsmu1tau", "tauvsmuvsecleaner", "tauvsmuvse1tau", "tauvsmuvsevsjetcleaner", "tauvsmuvsevsjet1tau", "final"};
  book_histograms(histtags);




  jet_lepton_cleaner.reset(new JetLeptonCleaner(cfg, year, "AK4PFchs"));
  jet_lepton_cleaner->set_muon_id(MuonID(Muon::IDMvaSoft)); // something very loose to get away most OK muons
  jet_lepton_cleaner->set_electron_id(ElectronID(Electron::IDMVANonIsoLoose)); // something very loose to get away most OK muons
  jec_corrector.reset(new JECCorrector(cfg, year, "AK4PFchs"));
  jer_corrector.reset(new JERCorrector(cfg, JERCFiles("JER", "MC", JERC.at((string)year).at("JER"), "AK4PFchs").at(0), JERCFiles("JER", "MC", JERC.at((string)year).at("JER"), "AK4PFchs").at(1)));

  lumiblock_selection.reset(new LumiblockSelection(cfg));
}




bool LQDMPsiPsiPreselectionTool::Process(){
  // cout << "++++++++++ NEW EVENT ++++++++++" << endl;
  if(!lumiblock_selection->passes(*event)) return false;
  lumiweight_applicator->process(*event);


  // order all objecs in pT
  sort_by_pt<GenParticle>(*event->genparticles_hard);
  sort_by_pt<GenParticle>(*event->genparticles_visibletaus);
  sort_by_pt<GenParticle>(*event->genparticles_final);
  sort_by_pt<GenParticle>(*event->genparticles_all);
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
  cleaner_jet->process(*event);
  fill_histograms("hadcleaner");

  cleaner_jettauoverlap->process(*event);
  fill_histograms("jettaucleaner");
  fill_histograms("iddenominator");


  cleaner_muon_pteta->process(*event);
  cleaner_electron_pteta->process(*event);
  cleaner_tau_pteta->process(*event);
  fill_histograms("ptetacleaner");

  if(nmuon_selection->passes(*event)){
    fill_histograms("pteta1muon");
  }

  if(nelectron_selection->passes(*event)){
    fill_histograms("pteta1electron");
  }

  if(ntau_selection->passes(*event)){
    fill_histograms("pteta1tau");
  }

  if(nelectron_selection->passes(*event) || nmuon_selection->passes(*event)){
    fill_histograms("pteta1lepton");
  }


  cleaner_muon_mvasoft->process(*event);
  cleaner_electron_mvanonisoloose->process(*event);
  fill_histograms("mvacleaner");

  if(nmuon_selection->passes(*event)){
    fill_histograms("mva1muon");
  }

  if(nelectron_selection->passes(*event)){
    fill_histograms("mva1electron");
  }

  if(nelectron_selection->passes(*event) || nmuon_selection->passes(*event)){
    fill_histograms("mva1lepton");
  }


  cleaner_muon_mvasoft_ip->process(*event);
  cleaner_electron_mvanonisoloose_ip->process(*event);
  fill_histograms("mvaipcleaner");

  if(nmuon_selection->passes(*event)){
    fill_histograms("mvaip1muon");
  }

  if(nelectron_selection->passes(*event)){
    fill_histograms("mvaip1electron");
  }

  if(nelectron_selection->passes(*event) || nmuon_selection->passes(*event)){
    fill_histograms("mvaip1lepton");
  }


  cleaner_tau_vsmu->process(*event);
  fill_histograms("tauvsmucleaner");

  if(ntau_selection->passes(*event)){
    fill_histograms("tauvsmu1tau");
  }

  cleaner_tau_vse->process(*event);
  fill_histograms("tauvsmuvsecleaner");

  if(ntau_selection->passes(*event)){
    fill_histograms("tauvsmuvse1tau");
  }

  cleaner_tau_vsjet->process(*event);
  fill_histograms("tauvsmuvsevsjetcleaner");

  if(ntau_selection->passes(*event)){
    fill_histograms("tauvsmuvsevsjet1tau");
  }

  if(!(nelectron_selection->passes(*event) || nmuon_selection->passes(*event) || ntau_selection->passes(*event))) return false;









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
