#pragma once

#include "LEAF/Analyzer/include/Config.h"
#include "LEAF/Analyzer/include/Selection.h"
#include "LEAF/LQDM/include/LQDMEvent.h"
#include "LEAF/Analyzer/include/ObjectIdUtils.h"
#include <TMatrixDSym.h>
#include <TMatrixDSymEigen.h>
#include <TVectorD.h>

#include <vector>
// #include <pair>

class SphericitySelection: public Selection<LQDMEvent>{
public:
  explicit SphericitySelection(const Config & cfg, double min_, double max_);
  virtual bool passes(LQDMEvent & event) override;

private:
  double min, max;

};

class AplanaritySelection: public Selection<LQDMEvent>{
public:
  explicit AplanaritySelection(const Config & cfg, double min_, double max_);
  virtual bool passes(LQDMEvent & event) override;

private:
  double min, max;

};
