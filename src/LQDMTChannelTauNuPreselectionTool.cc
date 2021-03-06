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

class LQDMTChannelTauNuPreselectionTool : public BaseTool {

public:
  // Constructors, destructor
  LQDMTChannelTauNuPreselectionTool(const Config & cfg);
  ~LQDMTChannelTauNuPreselectionTool() = default;
  void ProcessDataset(const Config & cfg) override {LoopEvents<LQDMTChannelTauNuPreselectionTool, LQDMEvent>(cfg, event, *this);};
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
  unique_ptr<JetCleaner> cleaner_jet, cleaner_jettauoverlap;
  unique_ptr<TauCleaner> cleaner_tau_tautrack, cleaner_tau_taumet, cleaner_tau_base;
  unique_ptr<MuonCleaner> cleaner_muon_base, cleaner_muon_veto;
  unique_ptr<ElectronCleaner> cleaner_electron_base, cleaner_electron_veto;

  // selections
  unique_ptr<LumiblockSelection> lumiblock_selection;
  unique_ptr<NElectronSelection> nelectron_selection;
  unique_ptr<NMuonSelection> nmuon_selection, nmuon_veto_selection;
  unique_ptr<NTauSelection> ntau_selection_base, ntau_selection_tautrack, ntau_selection_taumet;
  unique_ptr<METSelection> met_selection_met, met_selection_taumet, met_selection_tach;

  unique_ptr<FlagSelection> trigger_selection_met1, trigger_selection_met2,  trigger_selection_met3,  trigger_selection_met4,  trigger_selection_met5,  trigger_selection_met6,  trigger_selection_met7,  trigger_selection_met8,  trigger_selection_met9,  trigger_selection_met10, trigger_selection_met11, trigger_selection_met12, trigger_selection_met13, trigger_selection_met14, trigger_selection_met15;
  unique_ptr<FlagSelection> trigger_selection_tautrack1, trigger_selection_tautrack2;
  unique_ptr<FlagSelection> trigger_selection_taumet1, trigger_selection_taumet2;
  unique_ptr<FlagSelection> trigger_selection_singleelectron1, trigger_selection_singleelectron2, trigger_selection_singleelectron3;
  unique_ptr<FlagSelection> trigger_selection_singlemuon1;

  // constants
  TString year;
  TString channel;
};



LQDMTChannelTauNuPreselectionTool::LQDMTChannelTauNuPreselectionTool(const Config & cfg) : BaseTool(cfg){

  event = new LQDMEvent();
  event->reset();

  year = cfg.dataset_year();
  channel = TString(cfg.get("Channel"));

  MultiID<Jet> jet_id = {PtEtaId(15, 2.4), JetID(JetID::WP_TIGHT), JetPUID(JetPUID::WP_TIGHT)};
  MultiID<Jet> jet_overlapid = {JetTauOverlapID(0.5)};

  MultiID<Muon> muon_id_veto = {PtEtaId(20, 2.4), MuonID(Muon::IDCutBasedLoose), MuonID(Muon::IsoPFLoose)};
  MultiID<Electron> electron_id_veto = {PtEtaId(20, 2.4), ElectronID(Electron::IDCutBasedLoose)};
  MultiID<Muon> muon_id_base = {PtEtaId(30, 2.4), MuonID(Muon::IDCutBasedTight), MuonID(Muon::IsoPFTight)};
  MultiID<Electron> electron_id_base = {PtEtaId(40, 2.4), ElectronID(Electron::IDCutBasedTight)};
  MultiID<Tau> tau_id_base = {PtEtaId(30, 2.1), TauID(Tau::DeepTauVsJetLoose), TauID(Tau::DeepTauVsEleLoose), TauID(Tau::DeepTauVsMuTight)};
  MultiID<Tau> tau_id_tautrack = {PtEtaId(185, 2.1), TauID(Tau::DeepTauVsJetLoose), TauID(Tau::DeepTauVsEleLoose), TauID(Tau::DeepTauVsMuTight)};
  MultiID<Tau> tau_id_taumet = {PtEtaId(55, 2.1), TauID(Tau::DeepTauVsJetLoose), TauID(Tau::DeepTauVsEleLoose), TauID(Tau::DeepTauVsMuTight)};



  cleaner_jet.reset(new JetCleaner(jet_id));
  cleaner_jettauoverlap.reset(new JetCleaner(jet_overlapid));

  cleaner_muon_veto.reset(new MuonCleaner(muon_id_veto));
  cleaner_electron_veto.reset(new ElectronCleaner(electron_id_veto));
  cleaner_muon_base.reset(new MuonCleaner(muon_id_base));
  cleaner_electron_base.reset(new ElectronCleaner(electron_id_base));
  cleaner_tau_base.reset(new TauCleaner(tau_id_base));
  cleaner_tau_tautrack.reset(new TauCleaner(tau_id_tautrack));
  cleaner_tau_taumet.reset(new TauCleaner(tau_id_taumet));



  lumiweight_applicator.reset(new LumiWeightApplicator(cfg));

  met_selection_met.reset(new METSelection(cfg, 120, -1));
  met_selection_taumet.reset(new METSelection(cfg, 95, -1));
  met_selection_tach.reset(new METSelection(cfg, 200, -1));
  ntau_selection_base.reset(new NTauSelection(cfg, 1, -1));
  ntau_selection_tautrack.reset(new NTauSelection(cfg, 1, -1, tau_id_tautrack));
  ntau_selection_taumet.reset(new NTauSelection(cfg, 1, -1, tau_id_taumet));
  nelectron_selection.reset(new NElectronSelection(cfg, 1, -1));
  nmuon_selection.reset(new NMuonSelection(cfg, 1, -1));
  nmuon_veto_selection.reset(new NMuonSelection(cfg, 2, -1));

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

  trigger_selection_tautrack1.reset(new FlagSelection(cfg, "HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_1pr_v*"));
  trigger_selection_tautrack2.reset(new FlagSelection(cfg, "HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v*"));

  trigger_selection_taumet1.reset(new FlagSelection(cfg, "HLT_MediumChargedIsoPFTau50_Trk30_eta2p1_1pr_MET100_v*"));
  trigger_selection_taumet2.reset(new FlagSelection(cfg, "HLT_MediumChargedIsoPFTau50_Trk30_eta2p1_1pr_MET90_v*"));

  trigger_selection_singleelectron1.reset(new FlagSelection(cfg, "HLT_Ele35_WPTight_Gsf_v*"));
  trigger_selection_singleelectron2.reset(new FlagSelection(cfg, "HLT_Ele115_CaloIdVT_GsfTrkIdT_v*"));
  trigger_selection_singleelectron3.reset(new FlagSelection(cfg, "HLT_Photon200_v*"));

  trigger_selection_singlemuon1.reset(new FlagSelection(cfg, "HLT_IsoMu27_v*"));






  // histfolders
  vector<TString> histtags = {"input", "corrector", "trigger", "jetcleaner", "leptonvetocleaner", "plateau", "base", "tach_leptonveto", "tach_leptoncleaner", "tach_plateau", "tach_tau", "tach_met", "much_leptonveto", "much_leptoncleaner", "much_lepton", "much_plateau", "much_dphi", "much_ptratio", "final"};
  book_histograms(histtags);



  jet_lepton_cleaner.reset(new JetLeptonCleaner(cfg, year, "AK4PFchs"));
  jet_lepton_cleaner->set_muon_id(MuonID(Muon::IDCutBasedLoose)); // something very loose to get away most OK muons
  jet_lepton_cleaner->set_electron_id(ElectronID(Electron::IDMVANonIsoLoose)); // something very loose to get away most OK muons
  jec_corrector.reset(new JECCorrector(cfg, year, "AK4PFchs"));
  jer_corrector.reset(new JERCorrector(cfg, JERCFiles("JER", "MC", JERC_Info.at((string)year).at("JER_Version"), "AK4PFchs").at(0), JERCFiles("JER", "MC", JERC_Info.at((string)year).at("JER_Version"), "AK4PFchs").at(1)));

  lumiblock_selection.reset(new LumiblockSelection(cfg));
}




bool LQDMTChannelTauNuPreselectionTool::Process(){
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





  bool pass_hlt_met = (trigger_selection_met1->passes(*event) || trigger_selection_met2->passes(*event) || trigger_selection_met3->passes(*event) || trigger_selection_met4->passes(*event) || trigger_selection_met5->passes(*event) || trigger_selection_met6->passes(*event) || trigger_selection_met7->passes(*event) || trigger_selection_met8->passes(*event) || trigger_selection_met9->passes(*event) || trigger_selection_met10->passes(*event) || trigger_selection_met11->passes(*event) || trigger_selection_met12->passes(*event) || trigger_selection_met13->passes(*event) || trigger_selection_met14->passes(*event) || trigger_selection_met15->passes(*event));
  bool pass_hlt_tautrack = (trigger_selection_tautrack1->passes(*event) || trigger_selection_tautrack2->passes(*event));
  bool pass_hlt_taumet = (trigger_selection_taumet1->passes(*event) || trigger_selection_taumet2->passes(*event));
  bool pass_hlt_ele = (trigger_selection_singleelectron1->passes(*event) || trigger_selection_singleelectron2->passes(*event) || trigger_selection_singleelectron2->passes(*event));
  bool pass_hlt_mu = (trigger_selection_singlemuon1->passes(*event));



  bool pass_hlt = pass_hlt_met || pass_hlt_tautrack || pass_hlt_taumet || pass_hlt_ele || pass_hlt_mu;


  if(!pass_hlt) return false;
  fill_histograms("trigger");


  // cleaners
  cleaner_jet->process(*event);
  fill_histograms("jetcleaner");


  cleaner_muon_veto->process(*event);
  cleaner_electron_veto->process(*event);
  cleaner_tau_base->process(*event);
  fill_histograms("leptonvetocleaner");

  if(!(met_selection_met->passes(*event) || ntau_selection_tautrack->passes(*event) || nelectron_selection->passes(*event) || nmuon_selection->passes(*event) || (ntau_selection_taumet->passes(*event) && met_selection_taumet->passes(*event)))) return false;
  fill_histograms("plateau");

  if(channel == "tau"){
    // veto lepton channels
    if(nelectron_selection->passes(*event) || nmuon_selection->passes(*event)) return false;
    fill_histograms("tach_leptonveto");



    cleaner_muon_base->process(*event);
    cleaner_electron_base->process(*event);
    cleaner_jettauoverlap->process(*event);
    fill_histograms("tach_leptoncleaner");

    // plateau for this channel
    if(!(pass_hlt_tautrack || pass_hlt_taumet || pass_hlt_met)) return false;
    if(!(met_selection_met->passes(*event) || ntau_selection_tautrack->passes(*event) || (ntau_selection_taumet->passes(*event) && met_selection_taumet->passes(*event)))) return false;
    fill_histograms("tach_plateau");

    if(!ntau_selection_tautrack->passes(*event)) return false;
    fill_histograms("tach_tau");

    if(!met_selection_tach->passes(*event)) return false;
    fill_histograms("tach_met");
  }
  else if(channel == "muon"){

    // veto tau channel
    if(ntau_selection_base->passes(*event) || nelectron_selection->passes(*event) || nmuon_veto_selection->passes(*event)) return false;
    fill_histograms("much_leptonveto");

    cleaner_muon_base->process(*event);
    cleaner_electron_base->process(*event);
    cleaner_jettauoverlap->process(*event);
    fill_histograms("much_leptoncleaner");

    if(!nmuon_selection->passes(*event)) return false;
    fill_histograms("much_lepton");

    // // plateau for this channel
    // if(!(event->muons->at(0).pt() > 55)) return false;
    // fill_histograms("much_mupt");

    // plateau for this channel
    if(!((pass_hlt_mu && nmuon_selection->passes(*event)) || (pass_hlt_met && met_selection_met->passes(*event)))) return false;
    fill_histograms("much_plateau");

    double dphi = deltaPhi(event->muons->at(0), *event->met);
    if(dphi < 2.0) return false;
    fill_histograms("much_dphi");

    // 0.7 < ptTau / MET < 1.3
    double ratio_ptmu_met = event->muons->at(0).pt() / event->met->pt();
    if(ratio_ptmu_met < 0.3 || ratio_ptmu_met > 1.6) return false;
    fill_histograms("much_ptratio");

  }
  else return false;


  fill_histograms("final");


  return true;
}











void LQDMTChannelTauNuPreselectionTool::book_histograms(vector<TString> tags){
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

void LQDMTChannelTauNuPreselectionTool::fill_histograms(TString tag){
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




REGISTER_TOOL(LQDMTChannelTauNuPreselectionTool)
