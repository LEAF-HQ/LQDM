#pragma once

#include <TString.h>
#include <TH1F.h>
#include <map>
#include <TTreeReader.h>
#include <TROOT.h>
#include "LQDM/Analyzer/include/LQDMEvent.h"
#include "Analyzer/include/BaseHists.h"

using namespace std;

class LQDMLimitHists : public BaseHists{

public:
  // Constructors, destructor
  LQDMLimitHists(TString dir_);
  LQDMLimitHists(const LQDMLimitHists &) = default;
  LQDMLimitHists & operator = (const LQDMLimitHists &) = default;
  ~LQDMLimitHists() = default;

  // Main functions
  void fill(const LQDMEvent & event);


protected:

  shared_ptr<TH1D> hstmet1, hstmet2, hstmet3;
  // , hstmet4, hstmet5;

  vector<double> bins2, bins3;
  // , bins4, bins5;

};
