#include "LEAF/LQDM/include/LQDMEventShapeSelections.h"

#include "TFile.h"
#include "TTree.h"

using namespace std;


SphericitySelection::SphericitySelection(const Config & cfg, double min_, double max_) : min(min_), max(max_){}

bool SphericitySelection::passes(LQDMEvent & event){

  // Sphericity
  double s11 = -1., s12 = -1., s13 = -1., s22 = -1., s23 = -1., s33 = -1., mag = -1.;
  for(const Jet jet : *event.jets_ak4chs){
    mag += (jet.p4().Px()*jet.p4().Px()+jet.p4().Py()*jet.p4().Py()+jet.p4().Pz()*jet.p4().Pz());
    s11 += jet.p4().Px()*jet.p4().Px();
    s12 += jet.p4().Px()*jet.p4().Py();
    s13 += jet.p4().Px()*jet.p4().Pz();
    s22 += jet.p4().Py()*jet.p4().Py();
    s23 += jet.p4().Py()*jet.p4().Pz();
    s33 += jet.p4().Pz()*jet.p4().Pz();
  }
  for(const Muon mu : *event.muons){
    mag += (mu.p4().Px()*mu.p4().Px()+mu.p4().Py()*mu.p4().Py()+mu.p4().Pz()*mu.p4().Pz());
    s11 += mu.p4().Px()*mu.p4().Px();
    s12 += mu.p4().Px()*mu.p4().Py();
    s13 += mu.p4().Px()*mu.p4().Pz();
    s22 += mu.p4().Py()*mu.p4().Py();
    s23 += mu.p4().Py()*mu.p4().Pz();
    s33 += mu.p4().Pz()*mu.p4().Pz();
  }
  for(const Electron ele : *event.electrons){
    mag += (ele.p4().Px()*ele.p4().Px()+ele.p4().Py()*ele.p4().Py()+ele.p4().Pz()*ele.p4().Pz());
    s11 += ele.p4().Px()*ele.p4().Px();
    s12 += ele.p4().Px()*ele.p4().Py();
    s13 += ele.p4().Px()*ele.p4().Pz();
    s22 += ele.p4().Py()*ele.p4().Py();
    s23 += ele.p4().Py()*ele.p4().Pz();
    s33 += ele.p4().Pz()*ele.p4().Pz();
  }
  for(const Tau tau : *event.taus){
    mag += (tau.p4().Px()*tau.p4().Px()+tau.p4().Py()*tau.p4().Py()+tau.p4().Pz()*tau.p4().Pz());
    s11 += tau.p4().Px()*tau.p4().Px();
    s12 += tau.p4().Px()*tau.p4().Py();
    s13 += tau.p4().Px()*tau.p4().Pz();
    s22 += tau.p4().Py()*tau.p4().Py();
    s23 += tau.p4().Py()*tau.p4().Pz();
    s33 += tau.p4().Pz()*tau.p4().Pz();
  }

  s11 = s11 / mag;
  s12 = s12 / mag;
  s13 = s13 / mag;
  s22 = s22 / mag;
  s23 = s23 / mag;
  s33 = s33 / mag;

  vector<double> S_elements = {s11, s12, s13, s12, s22, s23, s13, s23, s33};
  TMatrixDSymEigen S = TMatrixDSymEigen(TMatrixDSym(3, &S_elements[0]));
  TVectorD S_eigenvalues = S.GetEigenValues();
  double sphericity = -1.;
  if(S_eigenvalues[0] >= S_eigenvalues[1] && S_eigenvalues[0] >= S_eigenvalues[2]){
    sphericity = 1.5*(S_eigenvalues[1] + S_eigenvalues[2]);
  }
  else if(S_eigenvalues[1] >= S_eigenvalues[0] && S_eigenvalues[1] >= S_eigenvalues[2]){
    sphericity = 1.5*(S_eigenvalues[0] + S_eigenvalues[2]);
  }
  else if(S_eigenvalues[2] >= S_eigenvalues[0] && S_eigenvalues[2] >= S_eigenvalues[1]){
    sphericity = 1.5*(S_eigenvalues[0] + S_eigenvalues[1]);
  }

  bool pass = (sphericity >= 0) && (sphericity >= min && (sphericity < max || max < 0));

  return pass;
}


AplanaritySelection::AplanaritySelection(const Config & cfg, double min_, double max_) : min(min_), max(max_){}

bool AplanaritySelection::passes(LQDMEvent & event){

  // Sphericity
  double s11 = -1., s12 = -1., s13 = -1., s22 = -1., s23 = -1., s33 = -1., mag = -1.;
  for(const Jet jet : *event.jets_ak4chs){
    mag += (jet.p4().Px()*jet.p4().Px()+jet.p4().Py()*jet.p4().Py()+jet.p4().Pz()*jet.p4().Pz());
    s11 += jet.p4().Px()*jet.p4().Px();
    s12 += jet.p4().Px()*jet.p4().Py();
    s13 += jet.p4().Px()*jet.p4().Pz();
    s22 += jet.p4().Py()*jet.p4().Py();
    s23 += jet.p4().Py()*jet.p4().Pz();
    s33 += jet.p4().Pz()*jet.p4().Pz();
  }
  for(const Muon mu : *event.muons){
    mag += (mu.p4().Px()*mu.p4().Px()+mu.p4().Py()*mu.p4().Py()+mu.p4().Pz()*mu.p4().Pz());
    s11 += mu.p4().Px()*mu.p4().Px();
    s12 += mu.p4().Px()*mu.p4().Py();
    s13 += mu.p4().Px()*mu.p4().Pz();
    s22 += mu.p4().Py()*mu.p4().Py();
    s23 += mu.p4().Py()*mu.p4().Pz();
    s33 += mu.p4().Pz()*mu.p4().Pz();
  }
  for(const Electron ele : *event.electrons){
    mag += (ele.p4().Px()*ele.p4().Px()+ele.p4().Py()*ele.p4().Py()+ele.p4().Pz()*ele.p4().Pz());
    s11 += ele.p4().Px()*ele.p4().Px();
    s12 += ele.p4().Px()*ele.p4().Py();
    s13 += ele.p4().Px()*ele.p4().Pz();
    s22 += ele.p4().Py()*ele.p4().Py();
    s23 += ele.p4().Py()*ele.p4().Pz();
    s33 += ele.p4().Pz()*ele.p4().Pz();
  }
  for(const Tau tau : *event.taus){
    mag += (tau.p4().Px()*tau.p4().Px()+tau.p4().Py()*tau.p4().Py()+tau.p4().Pz()*tau.p4().Pz());
    s11 += tau.p4().Px()*tau.p4().Px();
    s12 += tau.p4().Px()*tau.p4().Py();
    s13 += tau.p4().Px()*tau.p4().Pz();
    s22 += tau.p4().Py()*tau.p4().Py();
    s23 += tau.p4().Py()*tau.p4().Pz();
    s33 += tau.p4().Pz()*tau.p4().Pz();
  }

  s11 = s11 / mag;
  s12 = s12 / mag;
  s13 = s13 / mag;
  s22 = s22 / mag;
  s23 = s23 / mag;
  s33 = s33 / mag;

  vector<double> S_elements = {s11, s12, s13, s12, s22, s23, s13, s23, s33};
  TMatrixDSymEigen S = TMatrixDSymEigen(TMatrixDSym(3, &S_elements[0]));
  TVectorD S_eigenvalues = S.GetEigenValues();
  double aplanarity = -1.;
  if(S_eigenvalues[0] >= S_eigenvalues[1] && S_eigenvalues[0] >= S_eigenvalues[2]){
    aplanarity = 1.5*S_eigenvalues[0];
  }
  else if(S_eigenvalues[1] >= S_eigenvalues[0] && S_eigenvalues[1] >= S_eigenvalues[2]){
    aplanarity = 1.5*S_eigenvalues[1];
  }
  else if(S_eigenvalues[2] >= S_eigenvalues[0] && S_eigenvalues[2] >= S_eigenvalues[1]){
    aplanarity = 1.5*S_eigenvalues[2];
  }

  bool pass = (aplanarity >= 0) && (aplanarity >= min && (aplanarity < max || max < 0));

  return pass;
}
