#pragma once
#include <TLorentzVector.h>
#include <vector>
#include "TVector3.h"
#include <iostream>



inline TVector3 toVector(const TLorentzVector & v4){
    return TVector3(v4.X(),v4.Y(),v4.Z());
}

std::vector<TLorentzVector> NeutrinoReconstruction(const TLorentzVector & lepton, const TLorentzVector & met);
