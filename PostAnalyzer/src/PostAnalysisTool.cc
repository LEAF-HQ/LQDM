#include <TString.h>
#include <TFile.h>
#include <iostream>

#include "Analyzer/LQDM/PostAnalyzer/include/PostAnalysisTool.h"

using namespace std;

PostAnalysisTool::PostAnalysisTool(int year_, TString foldername_) {

    m_year = "";
    m_year += year_;

    m_leaf_path =  (TString)getenv("LEAFPATH") + "/"; // set up by setup.sh
    m_analyzer_path = (TString)getenv("ANALYZERPATH") + "/"; // set up by setup.sh
    m_lqdm_path = m_analyzer_path + "LQDM/";

    TString user = (TString)getenv("USER");
    m_fullselection_path = "/pnfs/psi.ch/cms/trivcat/store/user/" + user + "/LQDM/" + m_year + "/Fullselection/" + foldername_ + "/";
    m_finalselection_path = m_fullselection_path.ReplaceAll("Fullselection", "Finalselection");
    m_output_path = "/work/" + user + "/LQDM/" + m_year + "/Finalselection/" + foldername_ + "/";

    m_datacard_path = m_output_path + "data/";


    cout << green << "--> Fullselection path: " << m_fullselection_path << reset << endl;
    cout << green << "--> Finalselection path: " << m_fullselection_path << reset << endl;
    cout << green << "--> Output path:        " << m_output_path << reset << endl;

}
