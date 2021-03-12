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
#include "include/MinvSelections.h"
#include "include/FlagSelection.h"


#include "include/ElectronScaleFactorApplicator.h"
#include "include/MuonScaleFactorApplicator.h"


#include "LQDM/include/LQDMEvent.h"
#include "LQDM/include/LQDMPreselectionHists.h"

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

  // selections
  unique_ptr<NElectronSelection> nelectron_selection;
  unique_ptr<NElectronSelection> ptelectron_selection;
  unique_ptr<NElectronSelection> electronveto_selection;
  unique_ptr<NMuonSelection>     nmuon_selection;
  unique_ptr<NMuonSelection>     ptmuon_selection;
  unique_ptr<NMuonSelection>     muonveto_selection;

  unique_ptr<NTauSelection>      ntau_selection;
  unique_ptr<NTauSelection>      pttau_selection;

  unique_ptr<NJetSelection>      btag_selection;

  unique_ptr<MemuSelection>      memu_selection;
  unique_ptr<MetauSelection>     metau_selection;
  unique_ptr<MmutauSelection>    mmutau_selection;

  unique_ptr<FlagSelection>      trigger_mu_selection;
  unique_ptr<FlagSelection>      trigger_ele_selection1;
  unique_ptr<FlagSelection>      trigger_ele_selection2;

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


  // histfolders
  vector<TString> histtags = {"input", "btag", "much", "much_trigger", "much_ptlep", "much_ntau", "much_pttau", "much_mvis", "elch", "elch_trigger", "elch_ptlep", "elch_ntau", "elch_pttau", "elch_mvis", "elmuch", "elmuch_trigger", "elmuch_ptlep", "elmuch_mvis"};
  book_histograms(histtags);

  MultiID<Jet> pt_btag_id = {PtEtaId(50, -1.), JetBTag(JetBTag::DeepCSV_Medium)};
  // MultiID<Jet> pt_btag_id = {JetBTag(JetBTag::DeepCSV_Medium)};

  nmuon_selection.reset(new NMuonSelection(cfg, 1, 1));
  muonveto_selection.reset(new NMuonSelection(cfg, 0, 0));
  ptmuon_selection.reset(new NMuonSelection(cfg, 1, -1, PtEtaId(50, -1.)));
  nelectron_selection.reset(new NElectronSelection(cfg, 1, 1));
  electronveto_selection.reset(new NElectronSelection(cfg, 0, 0));
  ptelectron_selection.reset(new NElectronSelection(cfg, 1, -1, PtEtaId(50, -1.)));
  ntau_selection.reset(new NTauSelection(cfg, 1, -1));
  pttau_selection.reset(new NTauSelection(cfg, 1, -1, PtEtaId(50, -1.)));
  btag_selection.reset(new NJetSelection(cfg, 1, -1, pt_btag_id));

  memu_selection.reset(new MemuSelection(cfg, 100, -1));
  metau_selection.reset(new MetauSelection(cfg, 100, -1));
  mmutau_selection.reset(new MmutauSelection(cfg, 100, -1));

  trigger_mu_selection.reset(new FlagSelection(cfg, (TString)"HLT_IsoMu27"));
  trigger_ele_selection1.reset(new FlagSelection(cfg, (TString)"HLT_Ele35_WPTight_Gsf"));
  trigger_ele_selection2.reset(new FlagSelection(cfg, (TString)"HLT_Photon200"));
}




bool LQDMFullselectionTool::Process(){
  // cout << endl << "+++++ NEW EVENT ++++++" << endl;

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


  // cout << "weight before: " << event->weight << endl;
  ele_reco_sf_applicator->process(*event);
  ele_id_sf_applicator->process(*event);
  // cout << "weight between: " << event->weight << endl;
  muon_id_sf_applicator->process(*event);
  muon_iso_sf_applicator->process(*event);
  // cout << "weight after: " << event->weight << endl;

  if(!btag_selection->passes(*event)) return false;
  fill_histograms("btag");



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
  }

  if(nelectron_selection->passes(*event) && muonveto_selection->passes(*event)){
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
  }

  if(nelectron_selection->passes(*event) && nmuon_selection->passes(*event)){
    fill_histograms("elmuch");

    if(!trigger_mu_selection->passes(*event)) return false;
    muon_trigger_sf_applicator->process_trigger(*event);
    fill_histograms("elmuch_trigger");

    if(!(ptmuon_selection->passes(*event) && ptelectron_selection->passes(*event))) return false;
    fill_histograms("elmuch_ptlep");

    if(!memu_selection->passes(*event)) return false;
    fill_histograms("elmuch_mvis");

  }




  return false;
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
