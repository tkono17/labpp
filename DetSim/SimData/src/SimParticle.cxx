/*
  ds::SimParticle.hxx
*/
#include "SimData/SimParticle.hxx"

//ClassImp(ds::SimParticle)

namespace ds {

  SimParticle::SimParticle() {//: TObject() {
  }

  SimParticle::~SimParticle() {
  }

  void SimParticle::setP(float px, float py, float pz, float e) {
    mP[0] = px;
    mP[1] = py;
    mP[2] = pz;
    mP[3] = e;
  }

}
