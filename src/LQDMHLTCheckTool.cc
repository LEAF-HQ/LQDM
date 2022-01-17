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
#include "Analyzer/include/METSelection.h"
#include "Analyzer/include/MinvSelections.h"
#include "Analyzer/include/GenInfoHists.h"

#include "Analyzer/include/LumiWeightApplicator.h"


#include "LQDM/Analyzer/include/LQDMEvent.h"
#include "LQDM/Analyzer/include/LQDMPreselectionHists.h"
#include "LQDM/Analyzer/include/XPDFScaleFactorApplicator.h"


#include <LHAPDF/LHAPDF.h>

using namespace std;

class LQDMHLTCheckTool : public BaseTool {

public:
  // Constructors, destructor
  LQDMHLTCheckTool(const Config & cfg);
  ~LQDMHLTCheckTool() = default;
  void ProcessDataset(const Config & cfg) override {LoopEvents<LQDMHLTCheckTool, LQDMEvent>(cfg, event, *this);};
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
  unique_ptr<JetCleaner> cleaner_jet, cleaner_jet_elejet, cleaner_jettauoverlap;
  unique_ptr<TauCleaner> cleaner_tau_mutau, cleaner_tau_eletau, cleaner_tau_ditau;
  unique_ptr<MuonCleaner> cleaner_muon_singlemu, cleaner_muon_mutau, cleaner_muon_dimu1, cleaner_muon_dimu2;
  unique_ptr<ElectronCleaner> cleaner_electron_singleele, cleaner_electron_elejet, cleaner_electron_eletau;

  // selections
  unique_ptr<LumiblockSelection> lumiblock_selection;

  // scale factors
  unique_ptr<XPDFScaleFactorApplicator> xpdf_sf_applicator;

  // constants
  TString year, name;

  // LHAPDF object
  LHAPDF::PDF* pdf;
  LHAPDF::PDF* pdf_new;
};



LQDMHLTCheckTool::LQDMHLTCheckTool(const Config & cfg) : BaseTool(cfg){

  event = new LQDMEvent();
  event->reset();

  year = cfg.dataset_year();
  name = cfg.dataset_name();

  MultiID<Jet> jet_id = {PtEtaId(30, 2.4), JetID(JetID::WP_TIGHT), JetPUID(JetPUID::WP_TIGHT)};
  MultiID<Jet> jet_overlapid = {JetTauOverlapID(0.5)};

  MultiID<Muon> muon_id_singlemu = {PtEtaId(30, 2.4), MuonID(Muon::IDCutBasedTight), MuonID(Muon::IsoPFTight)};
  MultiID<Electron> electron_id_singleele = {PtEtaId(40, 2.4), ElectronID(Electron::IDMVANonIsoEff90)};
  MultiID<Tau> tau_id_ditau = {PtEtaId(40, 2.1), TauID(Tau::DeepTauVsJetLoose), TauID(Tau::DeepTauVsEleLoose), TauID(Tau::DeepTauVsMuTight)};



  cleaner_jet.reset(new JetCleaner(jet_id));
  cleaner_jettauoverlap.reset(new JetCleaner(jet_overlapid));
  cleaner_muon_singlemu.reset(new MuonCleaner(muon_id_singlemu));
  cleaner_electron_singleele.reset(new ElectronCleaner(electron_id_singleele));
  cleaner_tau_ditau.reset(new TauCleaner(tau_id_ditau));

  lumiweight_applicator.reset(new LumiWeightApplicator(cfg));


  string pdfname = "NNPDF31_lo_as_0130";
  LHAPDF::initPDFSet(1, (std::string)(pdfname+".LHgrid"));
  pdf = LHAPDF::mkPDF( (std::string) pdfname, 0);

  // string pdfname_new = "NNPDF31_nnlo_hessian_pdfas";
  // LHAPDF::initPDFSet(1, (std::string)(pdfname_new+".LHgrid"));
  // pdf_new = LHAPDF::mkPDF( (std::string) pdfname_new, 0);

  if(name.Contains("LQTChannelTauNu_MLQ2980_MPS117_MC1100") || name.Contains("LQTChannelTauNu_MLQ2620_MPS117_MC1100")) xpdf_sf_applicator.reset(new XPDFScaleFactorApplicator(cfg, pdfname, year, "../../LQDM/data/2017/XPDFScaleFactors.root", name));


  // histfolders
  vector<TString> histtags = {"input", "input_reweight", "genmet", "genmet_reweight", "corrector", "cleaner", "final"};
  book_histograms(histtags);
  book_HistFolder("input_Flags", new FlagHists("input_Flags"));




  jet_lepton_cleaner.reset(new JetLeptonCleaner(cfg, year, "AK4PFchs"));
  jet_lepton_cleaner->set_muon_id(MuonID(Muon::IDCutBasedLoose)); // something very loose to get away most OK muons
  jet_lepton_cleaner->set_electron_id(ElectronID(Electron::IDMVANonIsoLoose)); // something very loose to get away most OK muons
  jec_corrector.reset(new JECCorrector(cfg, year, "AK4PFchs"));
  jer_corrector.reset(new JERCorrector(cfg, JERCFiles("JER", "MC", JERC.at((string)year).at("JER"), "AK4PFchs").at(0), JERCFiles("JER", "MC", JERC.at((string)year).at("JER"), "AK4PFchs").at(1)));

  lumiblock_selection.reset(new LumiblockSelection(cfg));
}




bool LQDMHLTCheckTool::Process(){
  // cout << endl << endl << "++++++++++ NEW EVENT ++++++++++" << endl;

  if(!lumiblock_selection->passes(*event)) return false;
  lumiweight_applicator->process(*event);

  if(name.Contains("LQTChannelTauMu")){
    int n_hardmu = 0;
    int n_hardtau = 0;
    for (const auto & gp : *event->genparticles_all){
      if(abs(gp.pdgid()) == 13 && gp.get_statusflag(GenParticle::isHardProcess)) n_hardmu++;
      if(abs(gp.pdgid()) == 15 && gp.get_statusflag(GenParticle::isHardProcess)) n_hardtau++;
    }
    if(n_hardmu != 1) return false;
  }



  // double xpdf1 = pdf->xfxQ(event->geninfo->id1(), event->geninfo->x1(), event->geninfo->scale_pdf());
  // double xpdf2 = pdf->xfxQ(event->geninfo->id2(), event->geninfo->x2(), event->geninfo->scale_pdf());
  // double xpdf1_new = pdf_new->xfxQ(event->geninfo->id1(), event->geninfo->x1(), event->geninfo->scale_pdf());
  // double xpdf2_new = pdf_new->xfxQ(event->geninfo->id2(), event->geninfo->x2(), event->geninfo->scale_pdf());
  //
  // double sf = xpdf1_new * xpdf2_new  / (xpdf1 * xpdf2);


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

  double eventweight_original = event->weight;
  if(name.Contains("LQTChannelTauNu_MLQ2980_MPS117_MC1100") || name.Contains("LQTChannelTauNu_MLQ2620_MPS117_MC1100")) xpdf_sf_applicator->process(*event);
  // event->weight *= sf;
  fill_histograms("input_reweight");
  event->weight = eventweight_original;

  if(event->genmet->pt() < 100) return false;
  fill_histograms("genmet");

  if(name.Contains("LQTChannelTauNu_MLQ2980_MPS117_MC1100") || name.Contains("LQTChannelTauNu_MLQ2620_MPS117_MC1100")) xpdf_sf_applicator->process(*event);
  // event->weight *= sf;
  fill_histograms("genmet_reweight");
  event->weight = eventweight_original;



  // correctors
  jet_lepton_cleaner->process(*event);
  jec_corrector->process(*event);
  jec_corrector->correct_met(*event);
  jer_corrector->process(*event);
  fill_histograms("corrector");

  cleaner_jet->process(*event);
  cleaner_jettauoverlap->process(*event);
  cleaner_muon_singlemu->process(*event);
  cleaner_electron_singleele->process(*event);
  cleaner_tau_ditau->process(*event);
  fill_histograms("cleaner");


  fill_histograms("final");
  return false;
}











void LQDMHLTCheckTool::book_histograms(vector<TString> tags){
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
    mytag = tag+"_GenInfo";
    book_HistFolder(mytag, new GenInfoHists(mytag));
  }
}

void LQDMHLTCheckTool::fill_histograms(TString tag){
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
  mytag = tag+"_GenInfo";
  HistFolder<GenInfoHists>(mytag)->fill(*event);
}




REGISTER_TOOL(LQDMHLTCheckTool)
