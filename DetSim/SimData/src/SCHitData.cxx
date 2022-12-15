/*
  ds::SCHitData.cxx
*/
#include "SimData/SCHitData.hxx"

//ClassImp(ds::SCHitData)

namespace ds {

  SCHitData::SCHitData() {// : TObject() {
    mTime = 0.0;
    for (int i=0; i<3; ++i) {
      mPosition[i] = 0.0;
    }
    mEnergyDeposit = 0.0;
    mParticleId = 0;
    mParticleName = "";
  }

  SCHitData::~SCHitData() {
  }

  void SCHitData::setPosition(float x, float y, float z) {
    mPosition[0] = x;
    mPosition[1] = y;
    mPosition[2] = z;
  }

}

