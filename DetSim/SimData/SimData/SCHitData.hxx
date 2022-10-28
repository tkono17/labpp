#ifndef __ds_SCHitData_hxx__
#define __ds_SCHitData_hxx__
/*
  ds::SCHitData.hxx
*/
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

    float time() const { return mTime; }
    TVector3 position() const { return TVector3(mPosition[0], mPosition[1], mPosition[2]); }
    float energyDeposit() const { return mEnergyDeposit; }

    //    ClassDef(ds::SCHitData, 1)

  protected:
    float mTime;
    float mPosition[3];
    float mEnergyDeposit;

  };

}

#endif // __ds_SCHitData_hxx__
