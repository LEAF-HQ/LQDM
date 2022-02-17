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

#include "LEAF/Analyzer/include/LumiWeightApplicator.h"


#include "LEAF/LQDM/include/LQDMEvent.h"
#include "LEAF/LQDM/include/LQDMPreselectionHists.h"

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
  unique_ptr<TauCleaner> cleaner_tau_pteta, cleaner_tau_vsjetvvvloose, cleaner_tau_vsjetvvloose, cleaner_tau_vsjetvloose, cleaner_tau_vsjetloose, cleaner_tau_vsjetmedium, cleaner_tau_vsjettight, cleaner_tau_vsjetvtight, cleaner_tau_vsjetvvtight, cleaner_tau_vselevvvloose, cleaner_tau_vselevvloose, cleaner_tau_vselevloose, cleaner_tau_vseleloose, cleaner_tau_vselemedium, cleaner_tau_vseletight, cleaner_tau_vselevtight, cleaner_tau_vselevvtight, cleaner_tau_vsmuvloose, cleaner_tau_vsmuloose, cleaner_tau_vsmumedium, cleaner_tau_vsmutight;
  unique_ptr<MuonCleaner> cleaner_muon_cutsoft, cleaner_muon_pteta, cleaner_muon_cutloose, cleaner_muon_cutmedium, cleaner_muon_cuttight, cleaner_muon_mvasoft, cleaner_muon_mvaloose, cleaner_muon_mvamedium, cleaner_muon_mvatight;
  unique_ptr<ElectronCleaner> cleaner_electron_cutveto, cleaner_electron_pteta, cleaner_electron_cutloose, cleaner_electron_cutmedium, cleaner_electron_cuttight, cleaner_electron_cutheep, cleaner_electron_mvaisoloose, cleaner_electron_mvaiso90, cleaner_electron_mvaiso80, cleaner_electron_mvanonisoloose, cleaner_electron_mvanoniso90, cleaner_electron_mvanoniso80;

  // selections
  unique_ptr<LumiblockSelection> lumiblock_selection;

  // constants
  TString year;
};



LQDMPsiPsiLeptonIDEfficienciesTool::LQDMPsiPsiLeptonIDEfficienciesTool(const Config & cfg) : BaseTool(cfg){

  event = new LQDMEvent();
  event->reset();

  year = cfg.dataset_year();

  MultiID<Muon> muon_id_pteta = {PtEtaId(3, 2.4)};
  MultiID<Muon> muon_id_cutsoft = {PtEtaId(3, 2.4), MuonID(Muon::IDCutBasedSoft)};
  MultiID<Muon> muon_id_cutloose = {PtEtaId(3, 2.4), MuonID(Muon::IDCutBasedLoose)};
  MultiID<Muon> muon_id_cutmedium = {PtEtaId(3, 2.4), MuonID(Muon::IDCutBasedMedium)};
  MultiID<Muon> muon_id_cuttight = {PtEtaId(3, 2.4), MuonID(Muon::IDCutBasedTight)};
  MultiID<Muon> muon_id_mvasoft = {PtEtaId(3, 2.4), MuonID(Muon::IDMvaSoft)};
  MultiID<Muon> muon_id_mvaloose = {PtEtaId(3, 2.4), MuonID(Muon::IDMvaLoose)};
  MultiID<Muon> muon_id_mvamedium = {PtEtaId(3, 2.4), MuonID(Muon::IDMvaMedium)};
  MultiID<Muon> muon_id_mvatight = {PtEtaId(3, 2.4), MuonID(Muon::IDMvaTight)};


  MultiID<Electron> electron_id_pteta = {PtEtaId(5, 2.4)};
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

  MultiID<Tau> tau_id_pteta = {PtEtaId(18, 2.1)};
  MultiID<Tau> tau_id_vsjetvvvloose = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsJetVVVLoose)};
  MultiID<Tau> tau_id_vsjetvvloose = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsJetVVLoose)};
  MultiID<Tau> tau_id_vsjetvloose = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsJetVLoose)};
  MultiID<Tau> tau_id_vsjetloose = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsJetLoose)};
  MultiID<Tau> tau_id_vsjetmedium = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsJetMedium)};
  MultiID<Tau> tau_id_vsjettight = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsJetTight)};
  MultiID<Tau> tau_id_vsjetvtight = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsJetVTight)};
  MultiID<Tau> tau_id_vsjetvvtight = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsJetVVTight)};

  MultiID<Tau> tau_id_vselevvvloose = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsEleVVVLoose)};
  MultiID<Tau> tau_id_vselevvloose = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsEleVVLoose)};
  MultiID<Tau> tau_id_vselevloose = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsEleVLoose)};
  MultiID<Tau> tau_id_vseleloose = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsEleLoose)};
  MultiID<Tau> tau_id_vselemedium = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsEleMedium)};
  MultiID<Tau> tau_id_vseletight = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsEleTight)};
  MultiID<Tau> tau_id_vselevtight = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsEleVTight)};
  MultiID<Tau> tau_id_vselevvtight = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsEleVVTight)};
  MultiID<Tau> tau_id_vsmuvloose = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsMuVLoose)};
  MultiID<Tau> tau_id_vsmuloose = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsMuLoose)};
  MultiID<Tau> tau_id_vsmumedium = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsMuMedium)};
  MultiID<Tau> tau_id_vsmutight = {PtEtaId(18, 2.1), TauID(Tau::DeepTauVsMuTight)};




  MultiID<Jet> jet_id = {PtEtaId(15, 2.5), JetID(JetID::WP_TIGHT), JetPUID(JetPUID::WP_TIGHT)};
  MultiID<Jet> jet_overlapid = {JetTauOverlapID(0.5)};

  cleaner_muon_pteta.reset(new MuonCleaner(muon_id_pteta));
  cleaner_muon_cutsoft.reset(new MuonCleaner(muon_id_cutsoft));
  cleaner_muon_cutloose.reset(new MuonCleaner(muon_id_cutloose));
  cleaner_muon_cutmedium.reset(new MuonCleaner(muon_id_cutmedium));
  cleaner_muon_cuttight.reset(new MuonCleaner(muon_id_cuttight));
  cleaner_muon_mvasoft.reset(new MuonCleaner(muon_id_mvasoft));
  cleaner_muon_mvaloose.reset(new MuonCleaner(muon_id_mvaloose));
  cleaner_muon_mvamedium.reset(new MuonCleaner(muon_id_mvamedium));
  cleaner_muon_mvatight.reset(new MuonCleaner(muon_id_mvatight));

  cleaner_electron_pteta.reset(new ElectronCleaner(electron_id_pteta));
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

  cleaner_tau_pteta.reset(new TauCleaner(tau_id_pteta));
  cleaner_tau_vsjetvvvloose.reset(new TauCleaner(tau_id_vsjetvvvloose));
  cleaner_tau_vsjetvvloose.reset(new TauCleaner(tau_id_vsjetvvloose));
  cleaner_tau_vsjetvloose.reset(new TauCleaner(tau_id_vsjetvloose));
  cleaner_tau_vsjetloose.reset(new TauCleaner(tau_id_vsjetloose));
  cleaner_tau_vsjetmedium.reset(new TauCleaner(tau_id_vsjetmedium));
  cleaner_tau_vsjettight.reset(new TauCleaner(tau_id_vsjettight));
  cleaner_tau_vsjetvtight.reset(new TauCleaner(tau_id_vsjetvtight));
  cleaner_tau_vsjetvvtight.reset(new TauCleaner(tau_id_vsjetvvtight));
  cleaner_tau_vselevvvloose.reset(new TauCleaner(tau_id_vselevvvloose));
  cleaner_tau_vselevvloose.reset(new TauCleaner(tau_id_vselevvloose));
  cleaner_tau_vselevloose.reset(new TauCleaner(tau_id_vselevloose));
  cleaner_tau_vseleloose.reset(new TauCleaner(tau_id_vseleloose));
  cleaner_tau_vselemedium.reset(new TauCleaner(tau_id_vselemedium));
  cleaner_tau_vseletight.reset(new TauCleaner(tau_id_vseletight));
  cleaner_tau_vselevtight.reset(new TauCleaner(tau_id_vselevtight));
  cleaner_tau_vselevvtight.reset(new TauCleaner(tau_id_vselevvtight));
  cleaner_tau_vsmuvloose.reset(new TauCleaner(tau_id_vsmuvloose));
  cleaner_tau_vsmuloose.reset(new TauCleaner(tau_id_vsmuloose));
  cleaner_tau_vsmumedium.reset(new TauCleaner(tau_id_vsmumedium));
  cleaner_tau_vsmutight.reset(new TauCleaner(tau_id_vsmutight));

  cleaner_jet.reset(new JetCleaner(jet_id));
  cleaner_jettauoverlap.reset(new JetCleaner(jet_overlapid));

  lumiweight_applicator.reset(new LumiWeightApplicator(cfg));


  // histfolders
  vector<TString> histtags = {"input", "corrector", "hadcleaner", "jettaucleaner", "iddenominator", "pteta"};
  book_histograms(histtags);


  book_HistFolder("mu_pteta_Muons", new MuonHists("mu_pteta_Muons"));
  book_HistFolder("mu_cutsoft_Muons", new MuonHists("mu_cutsoft_Muons"));
  book_HistFolder("mu_cutloose_Muons", new MuonHists("mu_cutloose_Muons"));
  book_HistFolder("mu_cutmedium_Muons", new MuonHists("mu_cutmedium_Muons"));
  book_HistFolder("mu_cuttight_Muons", new MuonHists("mu_cuttight_Muons"));
  book_HistFolder("mu_mvasoft_Muons", new MuonHists("mu_mvasoft_Muons"));
  book_HistFolder("mu_mvaloose_Muons", new MuonHists("mu_mvaloose_Muons"));
  book_HistFolder("mu_mvamedium_Muons", new MuonHists("mu_mvamedium_Muons"));
  book_HistFolder("mu_mvatight_Muons", new MuonHists("mu_mvatight_Muons"));
  book_HistFolder("mu_pteta_GenParticles", new GenParticleHists("mu_pteta_GenParticles"));
  book_HistFolder("mu_cutsoft_GenParticles", new GenParticleHists("mu_cutsoft_GenParticles"));
  book_HistFolder("mu_cutloose_GenParticles", new GenParticleHists("mu_cutloose_GenParticles"));
  book_HistFolder("mu_cutmedium_GenParticles", new GenParticleHists("mu_cutmedium_GenParticles"));
  book_HistFolder("mu_cuttight_GenParticles", new GenParticleHists("mu_cuttight_GenParticles"));
  book_HistFolder("mu_mvasoft_GenParticles", new GenParticleHists("mu_mvasoft_GenParticles"));
  book_HistFolder("mu_mvaloose_GenParticles", new GenParticleHists("mu_mvaloose_GenParticles"));
  book_HistFolder("mu_mvamedium_GenParticles", new GenParticleHists("mu_mvamedium_GenParticles"));
  book_HistFolder("mu_mvatight_GenParticles", new GenParticleHists("mu_mvatight_GenParticles"));

  book_HistFolder("el_pteta_Electrons", new ElectronHists("el_pteta_Electrons"));
  book_HistFolder("el_cutveto_Electrons", new ElectronHists("el_cutveto_Electrons"));
  book_HistFolder("el_cutloose_Electrons", new ElectronHists("el_cutloose_Electrons"));
  book_HistFolder("el_cutmedium_Electrons", new ElectronHists("el_cutmedium_Electrons"));
  book_HistFolder("el_cuttight_Electrons", new ElectronHists("el_cuttight_Electrons"));
  book_HistFolder("el_cutheep_Electrons", new ElectronHists("el_cutheep_Electrons"));
  book_HistFolder("el_mvaisoloose_Electrons", new ElectronHists("el_mvaisoloose_Electrons"));
  book_HistFolder("el_mvaiso90_Electrons", new ElectronHists("el_mvaiso90_Electrons"));
  book_HistFolder("el_mvaiso80_Electrons", new ElectronHists("el_mvaiso80_Electrons"));
  book_HistFolder("el_mvanonisoloose_Electrons", new ElectronHists("el_mvanonisoloose_Electrons"));
  book_HistFolder("el_mvanoniso90_Electrons", new ElectronHists("el_mvanoniso90_Electrons"));
  book_HistFolder("el_mvanoniso80_Electrons", new ElectronHists("el_mvanoniso80_Electrons"));
  book_HistFolder("el_pteta_GenParticles", new GenParticleHists("el_pteta_GenParticles"));
  book_HistFolder("el_cutveto_GenParticles", new GenParticleHists("el_cutveto_GenParticles"));
  book_HistFolder("el_cutloose_GenParticles", new GenParticleHists("el_cutloose_GenParticles"));
  book_HistFolder("el_cutmedium_GenParticles", new GenParticleHists("el_cutmedium_GenParticles"));
  book_HistFolder("el_cuttight_GenParticles", new GenParticleHists("el_cuttight_GenParticles"));
  book_HistFolder("el_cutheep_GenParticles", new GenParticleHists("el_cutheep_GenParticles"));
  book_HistFolder("el_mvaisoloose_GenParticles", new GenParticleHists("el_mvaisoloose_GenParticles"));
  book_HistFolder("el_mvaiso90_GenParticles", new GenParticleHists("el_mvaiso90_GenParticles"));
  book_HistFolder("el_mvaiso80_GenParticles", new GenParticleHists("el_mvaiso80_GenParticles"));
  book_HistFolder("el_mvanonisoloose_GenParticles", new GenParticleHists("el_mvanonisoloose_GenParticles"));
  book_HistFolder("el_mvanoniso90_GenParticles", new GenParticleHists("el_mvanoniso90_GenParticles"));
  book_HistFolder("el_mvanoniso80_GenParticles", new GenParticleHists("el_mvanoniso80_GenParticles"));

  book_HistFolder("tau_pteta_Taus", new TauHists("tau_pteta_Taus"));
  book_HistFolder("tau_vsjetvvvloose_Taus", new TauHists("tau_vsjetvvvloose_Taus"));
  book_HistFolder("tau_vsjetvvloose_Taus", new TauHists("tau_vsjetvvloose_Taus"));
  book_HistFolder("tau_vsjetvloose_Taus", new TauHists("tau_vsjetvloose_Taus"));
  book_HistFolder("tau_vsjetloose_Taus", new TauHists("tau_vsjetloose_Taus"));
  book_HistFolder("tau_vsjetmedium_Taus", new TauHists("tau_vsjetmedium_Taus"));
  book_HistFolder("tau_vsjettight_Taus", new TauHists("tau_vsjettight_Taus"));
  book_HistFolder("tau_vsjetvtight_Taus", new TauHists("tau_vsjetvtight_Taus"));
  book_HistFolder("tau_vsjetvvtight_Taus", new TauHists("tau_vsjetvvtight_Taus"));
  book_HistFolder("tau_vselevvvloose_Taus", new TauHists("tau_vselevvvloose_Taus"));
  book_HistFolder("tau_vselevvloose_Taus", new TauHists("tau_vselevvloose_Taus"));
  book_HistFolder("tau_vselevloose_Taus", new TauHists("tau_vselevloose_Taus"));
  book_HistFolder("tau_vseleloose_Taus", new TauHists("tau_vseleloose_Taus"));
  book_HistFolder("tau_vselemedium_Taus", new TauHists("tau_vselemedium_Taus"));
  book_HistFolder("tau_vseletight_Taus", new TauHists("tau_vseletight_Taus"));
  book_HistFolder("tau_vselevtight_Taus", new TauHists("tau_vselevtight_Taus"));
  book_HistFolder("tau_vselevvtight_Taus", new TauHists("tau_vselevvtight_Taus"));
  book_HistFolder("tau_vsmuvloose_Taus", new TauHists("tau_vsmuvloose_Taus"));
  book_HistFolder("tau_vsmuloose_Taus", new TauHists("tau_vsmuloose_Taus"));
  book_HistFolder("tau_vsmumedium_Taus", new TauHists("tau_vsmumedium_Taus"));
  book_HistFolder("tau_vsmutight_Taus", new TauHists("tau_vsmutight_Taus"));
  book_HistFolder("tau_pteta_GenParticles", new GenParticleHists("tau_pteta_GenParticles"));
  book_HistFolder("tau_vsjetvvvloose_GenParticles", new GenParticleHists("tau_vsjetvvvloose_GenParticles"));
  book_HistFolder("tau_vsjetvvloose_GenParticles", new GenParticleHists("tau_vsjetvvloose_GenParticles"));
  book_HistFolder("tau_vsjetvloose_GenParticles", new GenParticleHists("tau_vsjetvloose_GenParticles"));
  book_HistFolder("tau_vsjetloose_GenParticles", new GenParticleHists("tau_vsjetloose_GenParticles"));
  book_HistFolder("tau_vsjetmedium_GenParticles", new GenParticleHists("tau_vsjetmedium_GenParticles"));
  book_HistFolder("tau_vsjettight_GenParticles", new GenParticleHists("tau_vsjettight_GenParticles"));
  book_HistFolder("tau_vsjetvtight_GenParticles", new GenParticleHists("tau_vsjetvtight_GenParticles"));
  book_HistFolder("tau_vsjetvvtight_GenParticles", new GenParticleHists("tau_vsjetvvtight_GenParticles"));
  book_HistFolder("tau_vselevvvloose_GenParticles", new GenParticleHists("tau_vselevvvloose_GenParticles"));
  book_HistFolder("tau_vselevvloose_GenParticles", new GenParticleHists("tau_vselevvloose_GenParticles"));
  book_HistFolder("tau_vselevloose_GenParticles", new GenParticleHists("tau_vselevloose_GenParticles"));
  book_HistFolder("tau_vseleloose_GenParticles", new GenParticleHists("tau_vseleloose_GenParticles"));
  book_HistFolder("tau_vselemedium_GenParticles", new GenParticleHists("tau_vselemedium_GenParticles"));
  book_HistFolder("tau_vseletight_GenParticles", new GenParticleHists("tau_vseletight_GenParticles"));
  book_HistFolder("tau_vselevtight_GenParticles", new GenParticleHists("tau_vselevtight_GenParticles"));
  book_HistFolder("tau_vselevvtight_GenParticles", new GenParticleHists("tau_vselevvtight_GenParticles"));
  book_HistFolder("tau_vsmuvloose_GenParticles", new GenParticleHists("tau_vsmuvloose_GenParticles"));
  book_HistFolder("tau_vsmuloose_GenParticles", new GenParticleHists("tau_vsmuloose_GenParticles"));
  book_HistFolder("tau_vsmumedium_GenParticles", new GenParticleHists("tau_vsmumedium_GenParticles"));
  book_HistFolder("tau_vsmutight_GenParticles", new GenParticleHists("tau_vsmutight_GenParticles"));


  jet_lepton_cleaner.reset(new JetLeptonCleaner(cfg, year, "AK4PFchs"));
  jet_lepton_cleaner->set_muon_id(MuonID(Muon::IDCutBasedSoft)); // something very loose to get away most OK muons
  jet_lepton_cleaner->set_electron_id(ElectronID(Electron::IDMVANonIsoLoose)); // something very loose to get away most OK muons
  jec_corrector.reset(new JECCorrector(cfg, year, "AK4PFchs"));
  jer_corrector.reset(new JERCorrector(cfg, JERCFiles("JER", "MC", JERC_Info.at((string)year).at("JER_Version"), "AK4PFchs").at(0), JERCFiles("JER", "MC", JERC_Info.at((string)year).at("JER_Version"), "AK4PFchs").at(1)));

  lumiblock_selection.reset(new LumiblockSelection(cfg));
}




bool LQDMPsiPsiLeptonIDEfficienciesTool::Process(){
  // cout << endl << "++++++++++ NEW EVENT ++++++++++" << endl;
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


  vector<Muon> original_muons = *event->muons;
  vector<Electron> original_electrons = *event->electrons;
  vector<Tau> original_taus = *event->taus;

  cleaner_muon_pteta->process(*event);
  cleaner_electron_pteta->process(*event);
  cleaner_tau_pteta->process(*event);
  fill_histograms("pteta");
  *event->muons = original_muons;
  *event->electrons = original_electrons;
  *event->taus = original_taus;

  cleaner_muon_pteta->process(*event);
  HistFolder<MuonHists>("mu_pteta_Muons")->fill(*event);
  HistFolder<GenParticleHists>("mu_pteta_GenParticles")->fill(*event);
  *event->muons = original_muons;

  cleaner_muon_cutsoft->process(*event);
  HistFolder<MuonHists>("mu_cutsoft_Muons")->fill(*event);
  HistFolder<GenParticleHists>("mu_cutsoft_GenParticles")->fill(*event);
  *event->muons = original_muons;

  cleaner_muon_cutloose->process(*event);
  HistFolder<MuonHists>("mu_cutloose_Muons")->fill(*event);
  HistFolder<GenParticleHists>("mu_cutloose_GenParticles")->fill(*event);
  *event->muons = original_muons;

  cleaner_muon_cutmedium->process(*event);
  HistFolder<MuonHists>("mu_cutmedium_Muons")->fill(*event);
  HistFolder<GenParticleHists>("mu_cutmedium_GenParticles")->fill(*event);
  *event->muons = original_muons;

  cleaner_muon_cuttight->process(*event);
  HistFolder<MuonHists>("mu_cuttight_Muons")->fill(*event);
  HistFolder<GenParticleHists>("mu_cuttight_GenParticles")->fill(*event);
  *event->muons = original_muons;

  cleaner_muon_mvasoft->process(*event);
  HistFolder<MuonHists>("mu_mvasoft_Muons")->fill(*event);
  HistFolder<GenParticleHists>("mu_mvasoft_GenParticles")->fill(*event);
  *event->muons = original_muons;

  cleaner_muon_mvaloose->process(*event);
  HistFolder<MuonHists>("mu_mvaloose_Muons")->fill(*event);
  HistFolder<GenParticleHists>("mu_mvaloose_GenParticles")->fill(*event);
  *event->muons = original_muons;

  cleaner_muon_mvamedium->process(*event);
  HistFolder<MuonHists>("mu_mvamedium_Muons")->fill(*event);
  HistFolder<GenParticleHists>("mu_mvamedium_GenParticles")->fill(*event);
  *event->muons = original_muons;

  cleaner_muon_mvatight->process(*event);
  HistFolder<MuonHists>("mu_mvatight_Muons")->fill(*event);
  HistFolder<GenParticleHists>("mu_mvatight_GenParticles")->fill(*event);
  *event->muons = original_muons;





  cleaner_electron_pteta->process(*event);
  HistFolder<ElectronHists>("el_pteta_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("el_pteta_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_cutveto->process(*event);
  HistFolder<ElectronHists>("el_cutveto_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("el_cutveto_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_cutloose->process(*event);
  HistFolder<ElectronHists>("el_cutloose_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("el_cutloose_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_cutmedium->process(*event);
  HistFolder<ElectronHists>("el_cutmedium_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("el_cutmedium_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_cuttight->process(*event);
  HistFolder<ElectronHists>("el_cuttight_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("el_cuttight_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_cutheep->process(*event);
  HistFolder<ElectronHists>("el_cutheep_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("el_cutheep_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_mvaisoloose->process(*event);
  HistFolder<ElectronHists>("el_mvaisoloose_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("el_mvaisoloose_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_mvaiso90->process(*event);
  HistFolder<ElectronHists>("el_mvaiso90_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("el_mvaiso90_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_mvaiso80->process(*event);
  HistFolder<ElectronHists>("el_mvaiso80_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("el_mvaiso80_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_mvanonisoloose->process(*event);
  HistFolder<ElectronHists>("el_mvanonisoloose_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("el_mvanonisoloose_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_mvanoniso90->process(*event);
  HistFolder<ElectronHists>("el_mvanoniso90_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("el_mvanoniso90_GenParticles")->fill(*event);
  *event->electrons = original_electrons;

  cleaner_electron_mvanoniso80->process(*event);
  HistFolder<ElectronHists>("el_mvanoniso80_Electrons")->fill(*event);
  HistFolder<GenParticleHists>("el_mvanoniso80_GenParticles")->fill(*event);
  *event->electrons = original_electrons;




  cleaner_tau_pteta->process(*event);
  HistFolder<TauHists>("tau_pteta_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_pteta_GenParticles")->fill(*event);
  *event->taus = original_taus;

  cleaner_tau_vsjetvvvloose->process(*event);
  HistFolder<TauHists>("tau_vsjetvvvloose_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_vsjetvvvloose_GenParticles")->fill(*event);
  *event->taus = original_taus;

  cleaner_tau_vsjetvvloose->process(*event);
  HistFolder<TauHists>("tau_vsjetvvloose_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_vsjetvvloose_GenParticles")->fill(*event);
  *event->taus = original_taus;

  cleaner_tau_vsjetvloose->process(*event);
  HistFolder<TauHists>("tau_vsjetvloose_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_vsjetvloose_GenParticles")->fill(*event);
  *event->taus = original_taus;

  cleaner_tau_vsjetloose->process(*event);
  HistFolder<TauHists>("tau_vsjetloose_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_vsjetloose_GenParticles")->fill(*event);
  *event->taus = original_taus;

  cleaner_tau_vsjetmedium->process(*event);
  HistFolder<TauHists>("tau_vsjetmedium_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_vsjetmedium_GenParticles")->fill(*event);
  *event->taus = original_taus;

  cleaner_tau_vsjettight->process(*event);
  HistFolder<TauHists>("tau_vsjettight_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_vsjettight_GenParticles")->fill(*event);
  *event->taus = original_taus;

  cleaner_tau_vsjetvtight->process(*event);
  HistFolder<TauHists>("tau_vsjetvtight_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_vsjetvtight_GenParticles")->fill(*event);
  *event->taus = original_taus;

  cleaner_tau_vsjetvvtight->process(*event);
  HistFolder<TauHists>("tau_vsjetvvtight_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_vsjetvvtight_GenParticles")->fill(*event);
  *event->taus = original_taus;

  cleaner_tau_vselevvvloose->process(*event);
  HistFolder<TauHists>("tau_vselevvvloose_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_vselevvvloose_GenParticles")->fill(*event);
  *event->taus = original_taus;

  cleaner_tau_vselevvloose->process(*event);
  HistFolder<TauHists>("tau_vselevvloose_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_vselevvloose_GenParticles")->fill(*event);
  *event->taus = original_taus;

  cleaner_tau_vselevloose->process(*event);
  HistFolder<TauHists>("tau_vselevloose_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_vselevloose_GenParticles")->fill(*event);
  *event->taus = original_taus;

  cleaner_tau_vseleloose->process(*event);
  HistFolder<TauHists>("tau_vseleloose_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_vseleloose_GenParticles")->fill(*event);
  *event->taus = original_taus;

  cleaner_tau_vselemedium->process(*event);
  HistFolder<TauHists>("tau_vselemedium_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_vselemedium_GenParticles")->fill(*event);
  *event->taus = original_taus;

  cleaner_tau_vseletight->process(*event);
  HistFolder<TauHists>("tau_vseletight_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_vseletight_GenParticles")->fill(*event);
  *event->taus = original_taus;

  cleaner_tau_vselevtight->process(*event);
  HistFolder<TauHists>("tau_vselevtight_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_vselevtight_GenParticles")->fill(*event);
  *event->taus = original_taus;

  cleaner_tau_vselevvtight->process(*event);
  HistFolder<TauHists>("tau_vselevvtight_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_vselevvtight_GenParticles")->fill(*event);
  *event->taus = original_taus;

  cleaner_tau_vsmuvloose->process(*event);
  HistFolder<TauHists>("tau_vsmuvloose_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_vsmuvloose_GenParticles")->fill(*event);
  *event->taus = original_taus;

  cleaner_tau_vsmuloose->process(*event);
  HistFolder<TauHists>("tau_vsmuloose_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_vsmuloose_GenParticles")->fill(*event);
  *event->taus = original_taus;

  cleaner_tau_vsmumedium->process(*event);
  HistFolder<TauHists>("tau_vsmumedium_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_vsmumedium_GenParticles")->fill(*event);
  *event->taus = original_taus;

  cleaner_tau_vsmutight->process(*event);
  HistFolder<TauHists>("tau_vsmutight_Taus")->fill(*event);
  HistFolder<GenParticleHists>("tau_vsmutight_GenParticles")->fill(*event);
  *event->taus = original_taus;





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
