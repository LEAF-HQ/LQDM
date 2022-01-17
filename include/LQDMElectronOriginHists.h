#pragma once

#include <TString.h>
#include <TH1F.h>
#include <TH2F.h>
#include <map>
#include <TTreeReader.h>
#include <TROOT.h>
#include "LEAF/LQDM/include/LQDMEvent.h"
#include "LEAF/Analyzer/include/BaseHists.h"

using namespace std;

class LQDMElectronOriginHists : public BaseHists{

public:
  // Constructors, destructor
  LQDMElectronOriginHists(TString dir_);
  LQDMElectronOriginHists(const LQDMElectronOriginHists &) = default;
  LQDMElectronOriginHists & operator = (const LQDMElectronOriginHists &) = default;
  ~LQDMElectronOriginHists() = default;

  // Main functions
  void fill(const LQDMEvent & event);


protected:

  shared_ptr<TH1D> helectrongenorigin, hnelectronsgenfromtau, hnelectronsgenfromhadron, hnelectronsgenfromelse, hnelectronsgenunmatched,

  helectronclosestorigin, helectronclosestd,

  hnelectronsfromtau, helectronfromtaupt, helectronfromtaupt_rebin, helectronfromtaupt_rebin2, helectronfromtaueta, helectronfromtauphi, helectronfromtaumass, helectronfromtauenergy, helectronfromtaupfiso, helectronfromtaudxy, helectronfromtaudz, helectronfromtaud, helectronfromtaugendrmin,

  hnelectronsfromhad, helectronfromhadpt, helectronfromhadpt_rebin, helectronfromhadpt_rebin2, helectronfromhadeta, helectronfromhadphi, helectronfromhadmass, helectronfromhadenergy, helectronfromhadpfiso, helectronfromhaddxy, helectronfromhaddz, helectronfromhadd, helectronfromhadgendrmin,

  hnelectronsunmatched, helectronunmatchedpt, helectronunmatchedpt_rebin, helectronunmatchedpt_rebin2, helectronunmatchedeta, helectronunmatchedphi, helectronunmatchedmass, helectronunmatchedenergy, helectronunmatchedpfiso, helectronunmatcheddxy, helectronunmatcheddz, helectronunmatchedd, helectronunmatchedgendrmin,


  helectron1genorigin, helectron2genorigin, helectron3genorigin, helectron4genorigin;


  shared_ptr<TH2D> helectronfromtau_dxy_vs_dz, helectronfromhad_dxy_vs_dz, helectronunmatched_dxy_vs_dz;





};
