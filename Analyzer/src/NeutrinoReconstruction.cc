#include "Analyzer/LQDM/include/NeutrinoReconstruction.h"

using namespace std;


vector<TLorentzVector> NeutrinoReconstruction(const TLorentzVector & lepton, const TLorentzVector & met) {

  TLorentzVector lepton_ = lepton;
  lepton_.SetE(lepton_.P()); // set mass of lepton to 0
  TVector3 ptl = lepton_.Vect();
  ptl.SetZ(0);
  TVector3 ptn = met.Vect();
  ptn.SetZ(0);

  const float mw = 80.399;
  float mu = mw * mw / 2 + ptl * ptn;
  float A = - (ptl * ptl);
  float B = mu * lepton_.Pz();
  float C = mu * mu - lepton_.E() * lepton_.E() * (ptn * ptn);
  float discriminant = B * B - A * C;


  vector<TLorentzVector> solutions;
  if (0 >= discriminant) {
    // Take only real part of the solution for pz:
    TLorentzVector solution;
    solution.SetPxPyPzE(met.Px(),met.Py(),-B / A,0);
    solution.SetE(solution.P());
    solutions.emplace_back(solution);
  }
  else {
    discriminant = sqrt(discriminant);
    TLorentzVector solution;
    solution.SetPxPyPzE(met.Px(),met.Py(),(-B - discriminant) / A, 0);
    solution.SetE(solution.P());
    solutions.emplace_back(solution);
    TLorentzVector solution2;
    solution2.SetPxPyPzE(met.Px(),met.Py(),(-B + discriminant) / A, 0);
    solution2.SetE(solution2.P());
    solutions.emplace_back(solution2);
  }


  return solutions;
}
