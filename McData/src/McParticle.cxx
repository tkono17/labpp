/*
  McParticle.cxx
*/
#include "McData/McParticle.hxx"
#include <TRandom.h>

ClassImp(McParticle)

using namespace std;

McParticle::McParticle() {
  mPx = 0.0;
  mPy = 0.0;
  mPz = 0.0;
  mE = 0.0;
  mProdVertexIndex = -1;
  mEndVertexIndex = -1;
}

McParticle::McParticle(float px, float py, float pz, float e) {
  mPx = px;
  mPy = py;
  mPz = pz;
  mE = e;
  //setMomentum(px, py, pz, e);
  mProdVertexIndex = -1;
  mEndVertexIndex = -1;
}

McParticle::~McParticle() {
}


void McParticle::setMomentum(float px, float py, float pz, float e) {
  mPx = px;
  mPy = py;
  mPz = pz;
  mE = e;
}

TLorentzVector McParticle::v4() const {
  return TLorentzVector(mPx, mPy, mPz, mE);
}

