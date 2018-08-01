/*
  Jet.cxx
*/
#include "McData/Jet.hxx"

ClassImp(Jet)

using namespace std;

Jet::Jet() : TObject() {
}

Jet::~Jet() {
}

void Jet::setMomentum(const TLorentzVector& v) {
  mP[0] = v.X();
  mP[1] = v.Y();
  mP[2] = v.Z();
  mP[3] = v.T();
}

TLorentzVector Jet::v4() const {
  return TLorentzVector(mP[0], mP[1], mP[2], mP[3]);
}

