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

class LQDMMuonOriginHists : public BaseHists{

public:
  // Constructors, destructor
  LQDMMuonOriginHists(TString dir_);
  LQDMMuonOriginHists(const LQDMMuonOriginHists &) = default;
  LQDMMuonOriginHists & operator = (const LQDMMuonOriginHists &) = default;
  ~LQDMMuonOriginHists() = default;

  // Main functions
  void fill(const LQDMEvent & event);


protected:

  shared_ptr<TH1D> hmuongenorigin,

  hmuonclosestorigin,

  hnmuonsfromtau, hmuonfromtaupt, hmuonfromtaupt_rebin, hmuonfromtaupt_rebin2, hmuonfromtaueta, hmuonfromtauphi, hmuonfromtaumass, hmuonfromtauenergy, hmuonfromtaupfiso, hmuonfromtaudxy, hmuonfromtaudz, hmuonfromtaud, hmuonfromtaugendrmin,
  hnmuonsfromhad, hmuonfromhadpt, hmuonfromhadpt_rebin, hmuonfromhadpt_rebin2, hmuonfromhadeta, hmuonfromhadphi, hmuonfromhadmass, hmuonfromhadenergy, hmuonfromhadpfiso, hmuonfromhaddxy, hmuonfromhaddz, hmuonfromhadd, hmuonfromhadgendrmin,
  hnmuonsunmatched, hmuonunmatchedpt, hmuonunmatchedpt_rebin, hmuonunmatchedpt_rebin2, hmuonunmatchedeta, hmuonunmatchedphi, hmuonunmatchedmass, hmuonunmatchedenergy, hmuonunmatchedpfiso, hmuonunmatcheddxy, hmuonunmatcheddz, hmuonunmatchedd, hmuonunmatchedgendrmin,

  hmuon1genorigin, hmuon2genorigin, hmuon3genorigin, hmuon4genorigin;

  shared_ptr<TH2D> hmuonfromtau_dxy_vs_dz, hmuonfromhad_dxy_vs_dz, hmuonunmatched_dxy_vs_dz;





};
