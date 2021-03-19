#pragma once

#include <cmath>
#include <iostream>
#include <TString.h>
#include <TFile.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

#include "Analyzer/include/useful_functions.h"

class PostAnalysisTool {

public:

  // Constructors, destructor
  PostAnalysisTool(int year_, TString foldername_);
  PostAnalysisTool(const PostAnalysisTool &) = default;
  PostAnalysisTool & operator = (const PostAnalysisTool &) = default;
  ~PostAnalysisTool() = default;

  // Main functions
  void ProduceCombineHistograms();

private:

  TString m_year;

  TString m_leaf_path;
  TString m_analyzer_path;
  TString m_lqdm_path;

  TString m_datacard_path;
  TString m_fullselection_path;
  TString m_finalselection_path;
  TString m_output_path;




};
