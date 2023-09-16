/*
  LargeRJet.cxx
*/
#include "McData/LargeRJet.hxx"

ClassImp(LargeRJet)

using namespace std;

LargeRJet::LargeRJet() : Jet() {
  mE1 = 0.0;
  mE2 = 0.0;
  mE3 = 0.0;
  mD2 = 0.0;
}

LargeRJet::~LargeRJet() {
}

