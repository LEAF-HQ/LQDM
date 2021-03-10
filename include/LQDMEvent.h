#pragma once

#include <TString.h>
#include <TH1F.h>
#include <map>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

#include "include/RecoEvent.h"

using namespace std;

// Container class for all quantities
class LQDMEvent : public RecoEvent{

public:
  // Constructors, destructor
  LQDMEvent();
  ~LQDMEvent();

  void clear();
  void reset();

};
