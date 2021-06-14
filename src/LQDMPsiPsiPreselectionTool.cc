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
#include "Analyzer/include/FlagHists.h"
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
#include "Analyzer/include/FlagSelection.h"

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
  unique_ptr<TauCleaner> cleaner_tau_pteta, cleaner_tau_vsx;
  unique_ptr<MuonCleaner> cleaner_muon_pteta, cleaner_muon_cutloose;
  unique_ptr<ElectronCleaner> cleaner_electron_pteta, cleaner_electron_mvanonisoloose;

  // selections
  unique_ptr<LumiblockSelection> lumiblock_selection;
  unique_ptr<NMuonSelection> nmuon_selection;
  unique_ptr<NElectronSelection> nelectron_selection;
  unique_ptr<NTauSelection> ntau_selection;

  unique_ptr<FlagSelection> trigger_selection_met1, trigger_selection_met2,  trigger_selection_met3,  trigger_selection_met4,  trigger_selection_met5,  trigger_selection_met6,  trigger_selection_met7,  trigger_selection_met8,  trigger_selection_met9,  trigger_selection_met10, trigger_selection_met11, trigger_selection_met12, trigger_selection_met13, trigger_selection_met14, trigger_selection_met15;
  unique_ptr<FlagSelection> trigger_selection_singlemu1, trigger_selection_doublemu1, trigger_selection_mutau1, trigger_selection_mutau2, trigger_selection_mutau3, trigger_selection_mutau4, trigger_selection_mutau5, trigger_selection_mutau6, trigger_selection_singleele1, trigger_selection_elejet1, trigger_selection_elejet2, trigger_selection_eletau1, trigger_selection_eletau2, trigger_selection_eletau3, trigger_selection_eletau4, trigger_selection_eletau5, trigger_selection_eletau6, trigger_selection_ditau1, trigger_selection_ditau2, trigger_selection_ditau3;

  // constants
  TString year;
};



LQDMPsiPsiPreselectionTool::LQDMPsiPsiPreselectionTool(const Config & cfg) : BaseTool(cfg){

  event = new LQDMEvent();
  event->reset();

  year = cfg.dataset_year();

  MultiID<Muon> muon_id_pteta = {PtEtaId(3, 2.4)};
  MultiID<Muon> muon_id_cutloose = {PtEtaId(3, 2.4), MuonID(Muon::IDCutBasedLoose)};


  MultiID<Electron> electron_id_pteta = {PtEtaId(5, 2.4)};
  MultiID<Electron> electron_id_mvanonisoloose = {PtEtaId(5, 2.4), ElectronID(Electron::IDMVANonIsoLoose)};

  MultiID<Tau> tau_id_pteta = {PtEtaId(18, 2.1)};
  MultiID<Tau> tau_id_vsx = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsJetVVVLoose), TauID(Tau::DeepTauVsEleVLoose), TauID(Tau::DeepTauVsMuTight)};

  MultiID<Jet> jet_id = {PtEtaId(15, 2.5), JetID(JetID::WP_TIGHT), JetPUID(JetPUID::WP_TIGHT)};
  MultiID<Jet> jet_overlapid = {JetTauOverlapID(0.5)};


  cleaner_jet.reset(new JetCleaner(jet_id));
  cleaner_jettauoverlap.reset(new JetCleaner(jet_overlapid));

  cleaner_muon_pteta.reset(new MuonCleaner(muon_id_pteta));
  cleaner_electron_pteta.reset(new ElectronCleaner(electron_id_pteta));
  cleaner_tau_pteta.reset(new TauCleaner(tau_id_pteta));

  cleaner_muon_cutloose.reset(new MuonCleaner(muon_id_cutloose));
  cleaner_electron_mvanonisoloose.reset(new ElectronCleaner(electron_id_mvanonisoloose));
  cleaner_tau_vsx.reset(new TauCleaner(tau_id_vsx));


  lumiweight_applicator.reset(new LumiWeightApplicator(cfg));

  nmuon_selection.reset(new NMuonSelection(cfg, 1, -1));
  nelectron_selection.reset(new NElectronSelection(cfg, 1, -1));
  ntau_selection.reset(new NTauSelection(cfg, 1, -1));

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

  trigger_selection_singlemu1.reset(new FlagSelection(cfg, "HLT_IsoMu27_v*"));

  trigger_selection_doublemu1.reset(new FlagSelection(cfg, "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v*"));

  trigger_selection_mutau1.reset(new FlagSelection(cfg, "HLT_IsoMu20_eta2p1_LooseChargedIsoPFTau27_eta2p1_CrossL1_v*"));
  trigger_selection_mutau2.reset(new FlagSelection(cfg, "HLT_IsoMu20_eta2p1_LooseChargedIsoPFTau27_eta2p1_TightID_CrossL1_v*"));
  trigger_selection_mutau3.reset(new FlagSelection(cfg, "HLT_IsoMu20_eta2p1_MediumChargedIsoPFTau27_eta2p1_CrossL1_v*"));
  trigger_selection_mutau4.reset(new FlagSelection(cfg, "HLT_IsoMu20_eta2p1_MediumChargedIsoPFTau27_eta2p1_TightID_CrossL1_v*"));
  trigger_selection_mutau5.reset(new FlagSelection(cfg, "HLT_IsoMu20_eta2p1_TightChargedIsoPFTau27_eta2p1_CrossL1_v*"));
  trigger_selection_mutau6.reset(new FlagSelection(cfg, "HLT_IsoMu20_eta2p1_TightChargedIsoPFTau27_eta2p1_TightID_CrossL1_v*"));

  trigger_selection_singleele1.reset(new FlagSelection(cfg, "HLT_Ele35_WPTight_Gsf_v*"));

  trigger_selection_elejet1.reset(new FlagSelection(cfg, "HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned_v*"));
  trigger_selection_elejet2.reset(new FlagSelection(cfg, "HLT_Ele28_eta2p1_WPTight_Gsf_HT150_v*"));

  trigger_selection_eletau1.reset(new FlagSelection(cfg, "HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTau30_eta2p1_CrossL1_v*"));
  trigger_selection_eletau2.reset(new FlagSelection(cfg, "HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTau30_eta2p1_TightID_CrossL1_v*"));
  trigger_selection_eletau3.reset(new FlagSelection(cfg, "HLT_Ele24_eta2p1_WPTight_Gsf_MediumChargedIsoPFTau30_eta2p1_CrossL1_v*"));
  trigger_selection_eletau4.reset(new FlagSelection(cfg, "HLT_Ele24_eta2p1_WPTight_Gsf_MediumChargedIsoPFTau30_eta2p1_TightID_CrossL1_v*"));
  trigger_selection_eletau5.reset(new FlagSelection(cfg, "HLT_Ele24_eta2p1_WPTight_Gsf_TightChargedIsoPFTau30_eta2p1_CrossL1_v*"));
  trigger_selection_eletau6.reset(new FlagSelection(cfg, "HLT_Ele24_eta2p1_WPTight_Gsf_TightChargedIsoPFTau30_eta2p1_TightID_CrossL1_v*"));

  trigger_selection_ditau1.reset(new FlagSelection(cfg, "HLT_DoubleTightChargedIsoPFTau35_Trk1_TightID_eta2p1_Reg_v*"));
  trigger_selection_ditau2.reset(new FlagSelection(cfg, "HLT_DoubleTightChargedIsoPFTau40_Trk1_TightID_eta2p1_Reg_v*"));
  trigger_selection_ditau3.reset(new FlagSelection(cfg, "HLT_DoubleTightChargedIsoPFTau40_Trk1_eta2p1_Reg_v*"));


  // histfolders
  vector<TString> histtags = {"input", "corrector", "jetcleaner", "ptetacleaner", "pteta1muon", "pteta1electron", "pteta1tau", "pteta1lepton", "idcleaner", "id1muon", "id1electron", "id1lepton", "tauvsxcleaner", "tauvsx1tau", "mettrigger", "mutrigger", "eletrigger", "tautrigger", "trigger", "nottrigger", "emutau1", "final"};
  book_histograms(histtags);

  book_HistFolder("input_Flags", new FlagHists("input_Flags"));
  book_HistFolder("id1muon_Flags", new FlagHists("id1muon_Flags"));
  book_HistFolder("id1electron_Flags", new FlagHists("id1electron_Flags"));
  book_HistFolder("tauvsx1tau_Flags", new FlagHists("tauvsx1tau_Flags"));
  book_HistFolder("mettrigger_Flags", new FlagHists("mettrigger_Flags"));
  book_HistFolder("mutrigger_Flags", new FlagHists("mutrigger_Flags"));
  book_HistFolder("eletrigger_Flags", new FlagHists("eletrigger_Flags"));
  book_HistFolder("tautrigger_Flags", new FlagHists("tautrigger_Flags"));
  book_HistFolder("trigger_Flags", new FlagHists("trigger_Flags"));
  book_HistFolder("nottrigger_Flags", new FlagHists("nottrigger_Flags"));
  book_HistFolder("final_Flags", new FlagHists("final_Flags"));




  jet_lepton_cleaner.reset(new JetLeptonCleaner(cfg, year, "AK4PFchs"));
  jet_lepton_cleaner->set_muon_id(MuonID(Muon::IDCutBasedLoose)); // something very loose to get away most OK muons
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
  sort_by_pt<GenParticle>(*event->genparticles_visibletaus);
  sort_by_pt<GenParticle>(*event->genparticles_all);
  sort_by_pt<GenJet>(*event->genjets);
  sort_by_pt<Jet>(*event->jets);
  sort_by_pt<Muon>(*event->muons);
  sort_by_pt<Electron>(*event->electrons);
  sort_by_pt<Tau>(*event->taus);
  fill_histograms("input");
  HistFolder<FlagHists>("input_Flags")->fill(*event);


  // correctors
  jet_lepton_cleaner->process(*event);
  jec_corrector->process(*event);
  jec_corrector->correct_met(*event);
  jer_corrector->process(*event);
  fill_histograms("corrector");


  // cleaners
  cleaner_jet->process(*event);
  cleaner_jettauoverlap->process(*event);
  fill_histograms("jetcleaner");


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




  cleaner_muon_cutloose->process(*event);
  cleaner_electron_mvanonisoloose->process(*event);
  fill_histograms("idcleaner");

  if(nmuon_selection->passes(*event)){
    fill_histograms("id1muon");
    HistFolder<FlagHists>("id1muon_Flags")->fill(*event);
  }

  if(nelectron_selection->passes(*event)){
    fill_histograms("id1electron");
    HistFolder<FlagHists>("id1electron_Flags")->fill(*event);
  }

  if(nelectron_selection->passes(*event) || nmuon_selection->passes(*event)){
    fill_histograms("id1lepton");
  }


  cleaner_tau_vsx->process(*event);
  fill_histograms("tauvsxcleaner");

  if(ntau_selection->passes(*event)){
    fill_histograms("tauvsx1tau");
    HistFolder<FlagHists>("tauvsx1tau_Flags")->fill(*event);
  }






  bool pass_hlt_met = (trigger_selection_met1->passes(*event) || trigger_selection_met2->passes(*event) || trigger_selection_met3->passes(*event) || trigger_selection_met4->passes(*event) || trigger_selection_met5->passes(*event) || trigger_selection_met6->passes(*event) || trigger_selection_met7->passes(*event) || trigger_selection_met8->passes(*event) || trigger_selection_met9->passes(*event) || trigger_selection_met10->passes(*event) || trigger_selection_met11->passes(*event) || trigger_selection_met12->passes(*event) || trigger_selection_met13->passes(*event) || trigger_selection_met14->passes(*event) || trigger_selection_met15->passes(*event));
  bool pass_hlt_singlemu = (trigger_selection_singlemu1->passes(*event));
  bool pass_hlt_doublemu = (trigger_selection_doublemu1->passes(*event));
  bool pass_hlt_mutau    = (trigger_selection_mutau1->passes(*event) || trigger_selection_mutau2->passes(*event) || trigger_selection_mutau3->passes(*event) || trigger_selection_mutau4->passes(*event) || trigger_selection_mutau5->passes(*event) || trigger_selection_mutau6->passes(*event));
  bool pass_hlt_singleele = (trigger_selection_singleele1->passes(*event));
  bool pass_hlt_elejet = (trigger_selection_elejet1->passes(*event) || trigger_selection_elejet2->passes(*event));
  bool pass_hlt_eletau    = (trigger_selection_eletau1->passes(*event) || trigger_selection_eletau2->passes(*event) || trigger_selection_eletau3->passes(*event) || trigger_selection_eletau4->passes(*event) || trigger_selection_eletau5->passes(*event) || trigger_selection_eletau6->passes(*event));
  bool pass_hlt_ditau = (trigger_selection_ditau1->passes(*event) || trigger_selection_ditau2->passes(*event) || trigger_selection_ditau3->passes(*event));

  bool pass_hlt_mu  = pass_hlt_singlemu || pass_hlt_doublemu || pass_hlt_mutau;
  bool pass_hlt_ele = pass_hlt_singleele || pass_hlt_elejet || pass_hlt_eletau;
  bool pass_hlt_tau = pass_hlt_ditau;

  bool pass_hlt = pass_hlt_met || pass_hlt_mu || pass_hlt_ele || pass_hlt_tau;



  if(pass_hlt_met){
    fill_histograms("mettrigger");
    HistFolder<FlagHists>("mettrigger_Flags")->fill(*event);
  }
  if(pass_hlt_mu){
    fill_histograms("mutrigger");
    HistFolder<FlagHists>("mutrigger_Flags")->fill(*event);
  }
  if(pass_hlt_ele){
    fill_histograms("eletrigger");
    HistFolder<FlagHists>("eletrigger_Flags")->fill(*event);
  }
  if(pass_hlt_tau){
    fill_histograms("tautrigger");
    HistFolder<FlagHists>("tautrigger_Flags")->fill(*event);
  }

  if(pass_hlt){
    fill_histograms("trigger");
    HistFolder<FlagHists>("trigger_Flags")->fill(*event);
  }
  else{
    fill_histograms("nottrigger");
    HistFolder<FlagHists>("nottrigger_Flags")->fill(*event);
    return false;
  }



  if(!(nelectron_selection->passes(*event) || nmuon_selection->passes(*event) || ntau_selection->passes(*event))) return false;
  fill_histograms("emutau1");



  fill_histograms("final");
  HistFolder<FlagHists>("final_Flags")->fill(*event);
  return false;
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
