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
#include "Analyzer/include/BJetHists.h"
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
#include "Analyzer/include/METSelection.h"
#include "Analyzer/include/MinvSelections.h"
#include "Analyzer/include/GenParticlePrinter.h"

#include "Analyzer/include/LumiWeightApplicator.h"


#include "Analyzer/LQDM/include/LQDMEvent.h"
#include "Analyzer/LQDM/include/LQDMPreselectionHists.h"
#include "Analyzer/LQDM/include/NeutrinoReconstruction.h"

using namespace std;

class LQDMTChannelTauNuFullselectionTool : public BaseTool {

public:
  // Constructors, destructor
  LQDMTChannelTauNuFullselectionTool(const Config & cfg);
  ~LQDMTChannelTauNuFullselectionTool() = default;
  void ProcessDataset(const Config & cfg) override {LoopEvents<LQDMTChannelTauNuFullselectionTool, LQDMEvent>(cfg, event, *this);};
  virtual bool Process() override;
  void book_histograms(vector<TString>);
  void fill_histograms(TString);


private:
  LQDMEvent* event;


  // for Tau channel
  unique_ptr<FlagSelection> trigger_selection_met, trigger_selection_tautrack;
  unique_ptr<TauCleaner> cleaner_tau;
  unique_ptr<NTauSelection> ntau_selection;
  unique_ptr<MuonCleaner> cleaner_muon_iso;

  // For Muon channel
  unique_ptr<NJetSelection> njet_selection, njet_selection2, njet_selection3, njet_selection4, nbjet_selection_loose, nbjet_selection_medium, nbjet_selection_tight;
  unique_ptr<NMuonSelection> nmuon_selection;
  ID<Jet> deepb_tight;



  // Printers
  unique_ptr<GenParticlePrinter> gp_printer;



  // constants
  TString year;
  TString channel;
};



LQDMTChannelTauNuFullselectionTool::LQDMTChannelTauNuFullselectionTool(const Config & cfg) : BaseTool(cfg){

  event = new LQDMEvent();
  event->reset();

  year = cfg.dataset_year();
  channel = TString(cfg.get("Channel"));
  gp_printer.reset(new GenParticlePrinter(cfg));

  // Triggers
  // HLT MediumChargedIsoPFTau180HighPtRelaxedIso Trk50 eta2p1
  // HLT PFMETNoMu120 PFMHTNoMu120 IDTight
  trigger_selection_met.reset(new FlagSelection(cfg, "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_v*"));
  trigger_selection_tautrack.reset(new FlagSelection(cfg, "HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v*"));

  MultiID<Jet> btag_id_loose = {JetBTag(JetBTag::DeepCSV_Loose)};
  MultiID<Jet> btag_id_medium = {JetBTag(JetBTag::DeepCSV_Medium)};
  MultiID<Jet> btag_id_tight = {JetBTag(JetBTag::DeepCSV_Tight)};

  // Tau:
  // pt > 190
  // veto decay modes 5, 6
  // All DeepTauIDs WPTight
  MultiID<Tau> tau_id = {PtEtaId(190, 2.1), TauID(Tau::DeepTauVsJetTight), TauID(Tau::DeepTauVsEleTight), TauID(Tau::DeepTauVsMuTight), TauDecaymodeVetoID({5, 6})};
  cleaner_tau.reset(new TauCleaner(tau_id));
  ntau_selection.reset(new NTauSelection(cfg, 1, -1));


  MultiID<Muon> muon_id_iso = {PtEtaId(30, 2.4), MuonID(Muon::IDCutBasedTight), MuonID(Muon::IsoPFVVTight)};
  cleaner_muon_iso.reset(new MuonCleaner(muon_id_iso));

  // Tau channel (W' -> tau nu)

  // MET > 200
  // --> already done in pre-selection

  // dPhi (tau, met) > 2.4

  // 0.7 < ptTau / MET < 1.3



  // Muon:

  // exactly one muon
  // nmuon_selection.reset(new NMuonSelection(cfg, 1, 1));

  // not more than 5 jets (going to be inverted)
  njet_selection.reset(new NJetSelection(cfg, 6, -1));

  // leading jet not tight-b-tagged
  deepb_tight = JetBTag(JetBTag::DeepCSV_Tight);

  // not more than 2 jets (going to be inverted)
  njet_selection2.reset(new NJetSelection(cfg, 3, -1));

  // not more than 1 jets (going to be inverted)
  njet_selection3.reset(new NJetSelection(cfg, 2, -1));

  // not more than 0 jets (going to be inverted)
  njet_selection4.reset(new NJetSelection(cfg, 1, -1));


  nbjet_selection_loose.reset(new NJetSelection(cfg, 1, -1, btag_id_loose));
  nbjet_selection_medium.reset(new NJetSelection(cfg, 1, -1, btag_id_medium));
  nbjet_selection_tight.reset(new NJetSelection(cfg, 1, -1, btag_id_tight));

  // histfolders
  // vector<TString> histtags = {"input", "trigger", "taucleaner", "tauselection", "dphi", "ptratio", "tach_cat1_nominal", "much_mu1", "much_jetveto5", "much_btagveto1", "much_dphi28", "much_ptratio", "much_jetveto3", "much_jetveto2", "much_jetveto1", "much_dphi30", "much_ptratioleq1", "much_ptmu100", "much_cat1_nominal"};
  vector<TString> histtags = {"input", "tach_trigger", "tach_taucleaner", "tach_tauselection", "tach_dphi", "tach_ptratio", "tach_cat1_nominal", "much_mu1", "much_jetveto5", "much_btagveto1", "much_ptmu100", "much_ptratio", "much_ptmu150", "much_cat1_nominal"};
  // vector<TString> histtags = {"input", "tach_trigger", "tach_taucleaner", "tach_tauselection", "tach_cat0jets_initial", "tach_cat0jets_nominal", "tach_cat1jets_initial", "tach_cat1jets_1btagloose_nominal", "tach_cat1jets_0btagloose_initial", "tach_cat1jets_0btagloose_1jet30", "tach_cat1jets_0btagloose_nominal", "tach_cat1jets_nominal", "tach_cat2jets_input", "tach_cat2jets_1btagloose_nominal", "tach_cat2jets_0btagloose_initial", "tach_cat2jets_0btagloose_1jet30", "tach_cat2jets_0btagloose_2jet30", "tach_cat2jets_0btagloose_nominal", "tach_cat2jets_nominal", "tach_cat1_nominal", "much_jetveto5", "much_btagveto1", "much_dphi25", "much_ptratio0415", "much_ptmu50", "much_cat1_nominal"};
  book_histograms(histtags);



}




bool LQDMTChannelTauNuFullselectionTool::Process(){
  // cout << "++++++++++ NEW EVENT ++++++++++" << endl;

  fill_histograms("input");


  if(channel == "tau"){
    // Triggers
    bool pass_hlt_met = trigger_selection_met->passes(*event);
    bool pass_hlt_tautrack = trigger_selection_tautrack->passes(*event);

    bool pass_hlt = pass_hlt_met || pass_hlt_tautrack;
    if(!pass_hlt) return false;
    fill_histograms("tach_trigger");


    // Tau
    cleaner_tau->process(*event);
    fill_histograms("tach_taucleaner");

    if(!ntau_selection->passes(*event)) return false;
    fill_histograms("tach_tauselection");


    // DeltaPhi (tau, MET)
    double dphi = deltaPhi(event->taus->at(0), *event->met);
    if(dphi < 2.4) return false;
    fill_histograms("tach_dphi");


    // 0.7 < ptTau / MET < 1.3
    double ratio_pttau_met = event->taus->at(0).pt() / event->met->pt();
    if(ratio_pttau_met < 0.7 || ratio_pttau_met > 1.3) return false;
    fill_histograms("tach_ptratio");

    // if(!njet_selection4->passes(*event)){ // == 0 jets
    //   fill_histograms("tach_cat0jets_initial");
    //   fill_histograms("tach_cat0jets_nominal");
    //
    // }
    // else if(njet_selection4->passes(*event) && !njet_selection3->passes(*event)){ // == 1 jet
    //   fill_histograms("tach_cat1jets_initial");
    //   // gp_printer->process(*event);



    // DeltaPhi (tau, MET)
    // double dphi = deltaPhi(event->taus->at(0), *event->met);
    // if(dphi < 2.4) return false;
    // fill_histograms("tach_cat1jets_dphi");
    //
    //
    // // 0.7 < ptTau / MET < 1.3
    // double ratio_pttau_met = event->taus->at(0).pt() / event->met->pt();
    // if(ratio_pttau_met < 0.7 || ratio_pttau_met > 1.3) return false;
    // fill_histograms("tach_cat1jets_ptratio");

    //   if(nbjet_selection_loose->passes(*event)){
    //     fill_histograms("tach_cat1jets_1btagloose_nominal");
    //   }
    //   else{
    //     fill_histograms("tach_cat1jets_0btagloose_initial");
    //
    //     if(event->jets->at(0).pt() < 30) return false;
    //     fill_histograms("tach_cat1jets_0btagloose_1jet30");
    //     fill_histograms("tach_cat1jets_0btagloose_nominal");
    //   }
    //   fill_histograms("tach_cat1jets_nominal");
    // }
    // else if(njet_selection3->passes(*event)){ // >= 2 jets
    //   fill_histograms("tach_cat2jets_input");
    //
    //   if(nbjet_selection_loose->passes(*event)){
    //     fill_histograms("tach_cat2jets_1btagloose_nominal");
    //   }
    //   else{
    //     fill_histograms("tach_cat2jets_0btagloose_nominal");
    //
    //     if(event->jets->at(0).pt() < 30) return false;
    //     fill_histograms("tach_cat2jets_0btagloose_1jet30");
    //
    //     if(event->jets->at(1).pt() < 30) return false;
    //     fill_histograms("tach_cat2jets_0btagloose_2jet30");
    //
    //     fill_histograms("tach_cat2jets_0btagloose_nominal");
    //   }
    //
    //   fill_histograms("tach_cat2jets_nominal");
    // }




    fill_histograms("tach_cat1_nominal");
  }
  else if(channel == "muon"){


    // if(!nmuon_selection->passes(*event)) return false;
    // fill_histograms("much_mu1");

    if(njet_selection->passes(*event)) return false;
    fill_histograms("much_jetveto5");

    if(event->jets->size() > 0){
      if(deepb_tight(event->jets->at(0), *event)) return false;
    }
    fill_histograms("much_btagveto1");


    double dphi = deltaPhi(event->muons->at(0), *event->met);
    if(dphi < 2.5) return false;
    fill_histograms("much_dphi25");

    // 0.4 < ptTau / MET < 1.5 (tightened from 1.3)
    double ratio_ptmu_met = event->muons->at(0).pt() / event->met->pt();
    if(ratio_ptmu_met < 0.4 || ratio_ptmu_met > 1.5) return false;
    fill_histograms("much_ptratio0415");

    if(event->muons->at(0).pt() < 50) return false;
    fill_histograms("much_ptmu50");
    //
    // if(njet_selection2->passes(*event)) return false;
    // fill_histograms("much_jetveto3");
    //
    // if(njet_selection3->passes(*event)) return false;
    // fill_histograms("much_jetveto2");
    //
    // if(njet_selection4->passes(*event)) return false;
    // fill_histograms("much_jetveto1");
    //
    //
    // if(dphi < 3) return false;
    // fill_histograms("much_dphi30");
    //
    // // 0.7 < ptTau / MET < 1.0 (tightened from 1.1)
    // if(ratio_ptmu_met < 0.7 || ratio_ptmu_met > 1.0) return false;
    // fill_histograms("much_ptratioleq1");



    // cleaner_muon_iso->process(*event);
    // if(!nmuon_selection->passes(*event)) return false;
    // fill_histograms("much_isomuVVT");

    // 0.7 < ptTau / MET < 1.1 (tightened from 1.3)
    // double ratio_ptmu_met = event->muons->at(0).pt() / event->met->pt();
    // if(ratio_ptmu_met < 0.7 || ratio_ptmu_met > 1.1) return false;
    // fill_histograms("much_ptratio");
    //
    // if(event->muons->at(0).pt() < 150) return false;
    // fill_histograms("much_ptmu150");



    fill_histograms("much_cat1_nominal");


    // vector<TLorentzVector> solutions = NeutrinoReconstruction(event->muons->at(0).p4(), event->met->p4());
    // cout << "number solutions: " << solutions.size() << endl;

  }



  return true;
}











void LQDMTChannelTauNuFullselectionTool::book_histograms(vector<TString> tags){
  for(const TString & tag : tags){
    TString mytag = tag+"_General";
    book_HistFolder(mytag, new LQDMPreselectionHists(mytag));
    mytag = tag+"_Jets";
    book_HistFolder(mytag, new JetHists(mytag));
    mytag = tag+"_BJets";
    book_HistFolder(mytag, new BJetHists(mytag));
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

void LQDMTChannelTauNuFullselectionTool::fill_histograms(TString tag){
  TString mytag = tag+"_General";
  HistFolder<LQDMPreselectionHists>(mytag)->fill(*event);
  mytag = tag+"_Jets";
  HistFolder<JetHists>(mytag)->fill(*event);
  mytag = tag+"_BJets";
  HistFolder<BJetHists>(mytag)->fill(*event);
  mytag = tag+"_Muons";
  HistFolder<MuonHists>(mytag)->fill(*event);
  mytag = tag+"_Electrons";
  HistFolder<ElectronHists>(mytag)->fill(*event);
  mytag = tag+"_Taus";
  HistFolder<TauHists>(mytag)->fill(*event);
  mytag = tag+"_GenParticles";
  HistFolder<GenParticleHists>(mytag)->fill(*event);
}




REGISTER_TOOL(LQDMTChannelTauNuFullselectionTool)
