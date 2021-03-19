#include <cmath>
#include <iostream>
#include <TString.h>

#include "Analyzer/LQDM/PostAnalyzer/include/PostAnalysisTool.h"

using namespace std;

int main(){

  cout << green << "--> Starting the PostAnalysisTool." << reset << endl;




  PostAnalysisTool PostAnalyzer(2017, "TauTau_ElTau_MuTau_ElMu_2Jets_12LooseBTag_met50");

  
  PostAnalyzer.ProduceCombineHistograms();









  cout << green << "--> PostAnalysisTool finished." << reset << endl;

}
