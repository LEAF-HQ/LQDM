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

class LQDMFullselectionTool : public BaseTool {

public:
  // Constructors, destructor
  LQDMFullselectionTool(const Config & cfg);
  ~LQDMFullselectionTool() = default;
  void ProcessDataset(const Config & cfg) override {LoopEvents<LQDMFullselectionTool, LQDMEvent>(cfg, event, *this);};
  virtual bool Process() override;
  void book_histograms(vector<TString>);
  void fill_histograms(TString);


private:
  LQDMEvent* event;

  // Modules used in the analysis

  unique_ptr<ElectronScaleFactorApplicator> ele_reco_sf_applicator;
  unique_ptr<ElectronScaleFactorApplicator> ele_id_sf_applicator;
  unique_ptr<ElectronScaleFactorApplicator> ele_trigger_sf_applicator;
  unique_ptr<MuonScaleFactorApplicator> muon_id_sf_applicator;
  unique_ptr<MuonScaleFactorApplicator> muon_iso_sf_applicator;
  unique_ptr<MuonScaleFactorApplicator> muon_trigger_sf_applicator;
  unique_ptr<BTaggingScaleFactorApplicator> btag_sf_applicator;

  // selections
  unique_ptr<NElectronSelection> nelectron_selection;
  unique_ptr<NElectronSelection> ptelectron_selection;
  unique_ptr<NElectronSelection> electronveto_selection;
  unique_ptr<NMuonSelection>     nmuon_selection;
  unique_ptr<NMuonSelection>     ptmuon_selection;
  unique_ptr<NMuonSelection>     muonveto_selection;

  unique_ptr<NTauSelection>      ntau_selection, nditau_selection;
  unique_ptr<NTauSelection>      pttau_selection, ptditau_selection;

  unique_ptr<NJetSelection>      nbtag_selection, nbtag2_selection, ptbtag_selection, njet_selection, ptjet_selection;

  unique_ptr<MemuSelection>      memu_selection;
  unique_ptr<MetauSelection>     metau_selection;
  unique_ptr<MmutauSelection>    mmutau_selection;
  unique_ptr<MtautauSelection>   mtautau_selection;

  unique_ptr<STMETSelection>     stmet_selection;
  unique_ptr<METSelection>       met_selection;

  unique_ptr<FlagSelection>      trigger_mu_selection;
  unique_ptr<FlagSelection>      trigger_ele_selection1;
  unique_ptr<FlagSelection>      trigger_ele_selection2;
  unique_ptr<FlagSelection>      trigger_tau_selection1;
  unique_ptr<FlagSelection>      trigger_tau_selection2;
  unique_ptr<FlagSelection>      trigger_tau_selection3;

  // constants
  TString year;
};



LQDMFullselectionTool::LQDMFullselectionTool(const Config & cfg) : BaseTool(cfg){

  event = new LQDMEvent();
  event->reset();

  year = cfg.dataset_year();

  ele_reco_sf_applicator.reset(new ElectronScaleFactorApplicator(cfg, year, "ElectronRECO.root", "EGamma_SF2D"));
  ele_id_sf_applicator.reset(new ElectronScaleFactorApplicator(cfg, year, "ElectronID_MVA90iso.root", "EGamma_SF2D"));
  ele_trigger_sf_applicator.reset(new ElectronScaleFactorApplicator(cfg, year, "ElectronTRIGGER.root", "ScaleFactors"));
  muon_id_sf_applicator.reset(new MuonScaleFactorApplicator(cfg, year, "MuonID.root", "NUM_TightID_DEN_TrackerMuons_abseta_pt"));
  muon_iso_sf_applicator.reset(new MuonScaleFactorApplicator(cfg, year, "MuonISO.root", "NUM_TightRelIso_DEN_TightIDandIPCut_abseta_pt"));
  muon_trigger_sf_applicator.reset(new MuonScaleFactorApplicator(cfg, year, "MuonTRIGGER.root", "ScaleFactors"));


  // JetBTag::wp btag_wp = JetBTag::DeepCSV_Medium;
  JetBTag::wp btag_wp = JetBTag::DeepCSV_Loose;
  btag_sf_applicator.reset(new BTaggingScaleFactorApplicator(cfg, btag_wp));

  // histfolders
  vector<TString> histtags = {"input", "njet", "ptjet", "met", "much", "much_trigger", "much_ptlep", "much_ntau", "much_pttau", "much_mvis", "much_btag", "much_btag_cat2", "much_btag_cat1", "elch", "elch_trigger", "elch_ptlep", "elch_ntau", "elch_pttau", "elch_mvis", "elch_btag", "elch_btag_cat2", "elch_btag_cat1", "elmuch", "elmuch_trigger", "elmuch_ptlep", "elmuch_mvis", "elmuch_btag", "elmuch_btag_cat2", "elmuch_btag_cat1", "tach", "tach_trigger", "tach_pttau", "tach_mvis", "tach_btag", "tach_btag_cat2", "tach_btag_cat1"};
  book_histograms(histtags);
  book_HistFolder("BTaggingMCEfficiencies", new BTaggingMCEfficiencyHists("BTaggingMCEfficiencies", btag_wp));


  MultiID<Jet> btag_id = {JetBTag(btag_wp)};
  MultiID<Jet> pt_btag_id = {PtEtaId(50, -1.), JetBTag(btag_wp)};
  MultiID<Jet> pt_jet_id  = {PtEtaId(50, -1.)};

  nmuon_selection.reset(new NMuonSelection(cfg, 1, 1));
  muonveto_selection.reset(new NMuonSelection(cfg, 0, 0));
  ptmuon_selection.reset(new NMuonSelection(cfg, 1, -1, PtEtaId(50, -1.)));
  nelectron_selection.reset(new NElectronSelection(cfg, 1, 1));
  electronveto_selection.reset(new NElectronSelection(cfg, 0, 0));
  ptelectron_selection.reset(new NElectronSelection(cfg, 1, -1, PtEtaId(50, -1.)));
  ntau_selection.reset(new NTauSelection(cfg, 1, -1));
  nditau_selection.reset(new NTauSelection(cfg, 2, 2));
  pttau_selection.reset(new NTauSelection(cfg, 1, -1, PtEtaId(50, -1.)));
  ptditau_selection.reset(new NTauSelection(cfg, 2, 2, PtEtaId(50, -1.)));

  nbtag_selection.reset(new NJetSelection(cfg, 1, -1, btag_id));
  ptbtag_selection.reset(new NJetSelection(cfg, 1, -1, pt_btag_id));
  nbtag2_selection.reset(new NJetSelection(cfg, 2, -1, btag_id));

  njet_selection.reset(new NJetSelection(cfg, 2, -1));
  ptjet_selection.reset(new NJetSelection(cfg, 1, -1, pt_jet_id));

  memu_selection.reset(new MemuSelection(cfg, 100, -1));
  metau_selection.reset(new MetauSelection(cfg, 100, -1));
  mmutau_selection.reset(new MmutauSelection(cfg, 100, -1));
  mtautau_selection.reset(new MtautauSelection(cfg, 100, -1));

  stmet_selection.reset(new STMETSelection(cfg, 600., -1));
  met_selection.reset(new METSelection(cfg, 50., -1));

  trigger_mu_selection.reset(new FlagSelection(cfg, (TString)"HLT_IsoMu27"));
  trigger_ele_selection1.reset(new FlagSelection(cfg, (TString)"HLT_Ele35_WPTight_Gsf"));
  trigger_ele_selection2.reset(new FlagSelection(cfg, (TString)"HLT_Photon200"));
  trigger_tau_selection1.reset(new FlagSelection(cfg, (TString)"HLT_DoubleTightChargedIsoPFTau35_Trk1_TightID_eta2p1_Reg"));
  trigger_tau_selection2.reset(new FlagSelection(cfg, (TString)"HLT_DoubleTightChargedIsoPFTau40_Trk1_eta2p1_Reg"));
  trigger_tau_selection3.reset(new FlagSelection(cfg, (TString)"HLT_DoubleMediumChargedIsoPFTau40_Trk1_TightID_eta2p1_Reg"));
}




bool LQDMFullselectionTool::Process(){
  // cout << endl << "+++++ NEW EVENT ++++++" << endl;

  // order all objecs in pT
  // sort_by_pt<GenParticle>(*event->genparticles_hard);
  sort_by_pt<GenParticle>(*event->genparticles_visibletaus);
  // sort_by_pt<GenParticle>(*event->genparticles_final);
  sort_by_pt<GenParticle>(*event->genparticles_all);
  sort_by_pt<GenJet>(*event->genjets);
  sort_by_pt<Jet>(*event->jets);
  sort_by_pt<Muon>(*event->muons);
  sort_by_pt<Electron>(*event->electrons);
  sort_by_pt<Tau>(*event->taus);
  fill_histograms("input");


  double stmet = event->met->pt();
  for (Jet & jet : *event->jets) stmet += jet.pt();
  for (Electron & e : *event->electrons) stmet += e.pt();
  for (Muon & mu : *event->muons) stmet += mu.pt();
  for (Tau & tau : *event->taus) stmet += tau.pt();
  // if(stmet > 600) return false;


  ele_reco_sf_applicator->process(*event);
  ele_id_sf_applicator->process(*event);
  muon_id_sf_applicator->process(*event);
  muon_iso_sf_applicator->process(*event);

  if(!njet_selection->passes(*event)) return false;
  fill_histograms("njet");

  if(!ptjet_selection->passes(*event)) return false;
  fill_histograms("ptjet");

  if(!met_selection->passes(*event)) return false;
  fill_histograms("met");



  if(nmuon_selection->passes(*event) && electronveto_selection->passes(*event)){
    fill_histograms("much");

    if(!trigger_mu_selection->passes(*event)) return false;
    muon_trigger_sf_applicator->process_trigger(*event);
    fill_histograms("much_trigger");

    if(!ptmuon_selection->passes(*event)) return false;
    fill_histograms("much_ptlep");

    if(!ntau_selection->passes(*event)) return false;
    fill_histograms("much_ntau");

    if(!pttau_selection->passes(*event)) return false;
    fill_histograms("much_pttau");

    if(!mmutau_selection->passes(*event)) return false;
    fill_histograms("much_mvis");
    HistFolder<BTaggingMCEfficiencyHists>("BTaggingMCEfficiencies")->fill(*event);
    btag_sf_applicator->process(*event);

    if(!(nbtag_selection->passes(*event) && ptbtag_selection->passes(*event))) return false;
    fill_histograms("much_btag");

    if(nbtag2_selection->passes(*event)) fill_histograms("much_btag_cat2");
    else fill_histograms("much_btag_cat1");

  }

  else if(nelectron_selection->passes(*event) && muonveto_selection->passes(*event)){
    fill_histograms("elch");

    if(!(trigger_ele_selection1->passes(*event) || trigger_ele_selection2->passes(*event))) return false;
    ele_trigger_sf_applicator->process_trigger(*event);
    fill_histograms("elch_trigger");

    if(!ptelectron_selection->passes(*event)) return false;
    fill_histograms("elch_ptlep");

    if(!ntau_selection->passes(*event)) return false;
    fill_histograms("elch_ntau");

    if(!pttau_selection->passes(*event)) return false;
    fill_histograms("elch_pttau");

    if(!metau_selection->passes(*event)) return false;
    fill_histograms("elch_mvis");
    HistFolder<BTaggingMCEfficiencyHists>("BTaggingMCEfficiencies")->fill(*event);
    btag_sf_applicator->process(*event);

    if(!(nbtag_selection->passes(*event) && ptbtag_selection->passes(*event))) return false;
    fill_histograms("elch_btag");

    if(nbtag2_selection->passes(*event)) fill_histograms("elch_btag_cat2");
    else fill_histograms("elch_btag_cat1");
  }

  else if(nelectron_selection->passes(*event) && nmuon_selection->passes(*event)){
    fill_histograms("elmuch");

    if(!trigger_mu_selection->passes(*event)) return false;
    muon_trigger_sf_applicator->process_trigger(*event);
    fill_histograms("elmuch_trigger");

    if(!(ptmuon_selection->passes(*event) && ptelectron_selection->passes(*event))) return false;
    fill_histograms("elmuch_ptlep");

    if(!memu_selection->passes(*event)) return false;
    fill_histograms("elmuch_mvis");
    HistFolder<BTaggingMCEfficiencyHists>("BTaggingMCEfficiencies")->fill(*event);
    btag_sf_applicator->process(*event);

    if(!(nbtag_selection->passes(*event) && ptbtag_selection->passes(*event))) return false;
    fill_histograms("elmuch_btag");

    if(nbtag2_selection->passes(*event)) fill_histograms("elmuch_btag_cat2");
    else fill_histograms("elmuch_btag_cat1");

  }

  else if(nditau_selection->passes(*event)){
    fill_histograms("tach");

    if(!(trigger_tau_selection1->passes(*event) || trigger_tau_selection2->passes(*event) || trigger_tau_selection3->passes(*event))) return false;
    fill_histograms("tach_trigger");

    if(!ptditau_selection->passes(*event)) return false;
    fill_histograms("tach_pttau");

    if(!mtautau_selection->passes(*event)) return false;
    fill_histograms("tach_mvis");
    HistFolder<BTaggingMCEfficiencyHists>("BTaggingMCEfficiencies")->fill(*event);
    btag_sf_applicator->process(*event);

    if(!(nbtag_selection->passes(*event) && ptbtag_selection->passes(*event))) return false;
    fill_histograms("tach_btag");

    if(nbtag2_selection->passes(*event)) fill_histograms("tach_btag_cat2");
    else fill_histograms("tach_btag_cat1");
  }
  else return false;




  return true;
}











void LQDMFullselectionTool::book_histograms(vector<TString> tags){
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

void LQDMFullselectionTool::fill_histograms(TString tag){
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




REGISTER_TOOL(LQDMFullselectionTool)
