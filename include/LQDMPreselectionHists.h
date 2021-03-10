#pragma once

#include <TString.h>
#include <TH1F.h>
#include <map>
#include <TTreeReader.h>
#include <TROOT.h>
#include "LQDM/include/LQDMEvent.h"
#include "include/BaseHists.h"

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

  shared_ptr<TH1D> hmetpt, hmetphi, hstmet, hmemu, hmetau, hmmutau, hsumweights;

};
