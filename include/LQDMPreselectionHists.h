#pragma once

#include <TString.h>
#include <TH1F.h>
#include <map>
#include <TTreeReader.h>
#include <TROOT.h>
#include "LEAF/LQDM/include/LQDMEvent.h"
#include "LEAF/Analyzer/include/BaseHists.h"
#include "LEAF/LQDM/include/NeutrinoReconstruction.h"

using namespace std;

class LQDMPreselectionHists : public BaseHists{

public:
  // Constructors, destructor
  LQDMPreselectionHists(TString dir_);
  LQDMPreselectionHists(const LQDMPreselectionHists &) = default;
  LQDMPreselectionHists & operator = (const LQDMPreselectionHists &) = default;
  ~LQDMPreselectionHists() = default;

  // Main functions
  void fill(const LQDMEvent & event);


protected:

  shared_ptr<TH1D> hmetpt, hmetphi, hstmet, hstmet_rebin, hstvecpt, hstvecpt_rebin, hstvecphi, hstveceta, hdphi_stvec_met, hptratio_stvec_met, hht, hht_rebin, hhtvecpt, hhtvecpt_rebin, hhtvecphi, hhtveceta, hdphi_htvec_met, hptratio_htvec_met, hlt, hlt_rebin, hltvecpt, hltvecpt_rebin, hltvecphi, hltveceta, hdphi_ltvec_met, hptratio_ltvec_met, hrt, hrt_rebin, hrt_rebin2, hrt_rebin3, hmt_taumet, hmt_taumet_rebin, hmt_taumet_rebinlimit, hmt_taumet_rebinlimit2, hmt_mumet, hmt_mumet_rebin, hmt_mumet_rebinlimit, hmt_mumet_rebinlimit2, hmt_emet, hmt_emet_rebin, hmt_emet_rebinlimit, hmt_emet_rebinlimit2, hmemu, hmetau, hmmutau, hmtautau, hdphi_tau1_met, hptratio_tau1_met, hdphi_mu1_met, hptratio_mu1_met, hdphi_e1_met, hptratio_e1_met, hdphi_j1_met, hptratio_j1_met, hdphi_j2_met, hptratio_j2_met, hdphi_j3_met, hptratio_j3_met, hptratio_j2_j1, hptratio_j3_j1, hptratio_j3_j2, hreco_wmass, hreco_wpt, hnpv, hnpvgood, hsumweights;

  std::vector<double> bins2, bins3;

};
