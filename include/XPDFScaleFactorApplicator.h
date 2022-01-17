#pragma once
#include "LEAF/Analyzer/include/ScaleFactorApplicator.h"
#include "LEAF/Analyzer/include/useful_functions.h"
#include "LEAF/Analyzer/include/constants.h"
#include <TH1D.h>
#include <TH2D.h>

#include <LHAPDF/LHAPDF.h>

class XPDFScaleFactorApplicator: public ScaleFactorApplicator {
public:
  explicit XPDFScaleFactorApplicator(const Config& cfg, TString pdfname, TString year, TString infilename, TString histname) : ScaleFactorApplicator(cfg, year, infilename, histname) {
    m_pdfname = (std::string)pdfname;

    LHAPDF::initPDFSet(1, (std::string)(m_pdfname+".LHgrid"));
    m_pdf = LHAPDF::mkPDF( (std::string) m_pdfname, 0);
  };
  virtual ~XPDFScaleFactorApplicator() = default;

  virtual bool process(RecoEvent & event) override;


private:
  std::string m_pdfname;
  LHAPDF::PDF* m_pdf;
};
