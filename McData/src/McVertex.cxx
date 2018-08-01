/*
  McVertex.cxx
*/
#include "McData/McVertex.hxx"

ClassImp(McVertex)

using namespace std;

McVertex::McVertex() : TObject() {
  setPosition(0.0, 0.0, 0.0, 0.0);
}

McVertex::McVertex(float x, float y, float z, float t) {
  setPosition(x, y, z, t);
}

McVertex::~McVertex() {
}

void McVertex::setPosition(float x, float y, float z, float t) {
  mX = x;
  mY = y;
  mZ = z;
  mT = t;
}

TLorentzVector McVertex::position() const {
  return TLorentzVector(mX, mY, mZ, mT);
}

