#ifndef __ds_SCHitData_hxx__
#define __ds_SCHitData_hxx__
/*
  ds::SCHitData.hxx
*/
#include <string>
//#include "TObject.h"
#include "TVector3.h"

namespace ds {

  class SCHitData {//: public TObject {
  public:
    SCHitData();
    ~SCHitData();

    void setTime(float x) { mTime = x; }
    void setPosition(float x, float y, float z);
    void setEnergyDeposit(float x) { mEnergyDeposit = x; }
    void setParticleId(int id) { mParticleId = id; }
    void setParticleName(const std::string& pname) { mParticleName = pname; }
    
    float time() const { return mTime; }
    TVector3 position() const { return TVector3(mPosition[0], mPosition[1], mPosition[2]); }
    float energyDeposit() const { return mEnergyDeposit; }
    int particleId() const { return mParticleId; }
    const std::string& particleName() const { return mParticleName; }

    //    ClassDef(ds::SCHitData, 1)

  protected:
    float mTime;
    float mPosition[3];
    float mEnergyDeposit;
    int mParticleId;
    std::string mParticleName;

  };

}

#endif // __ds_SCHitData_hxx__
