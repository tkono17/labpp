#ifndef __ds_SCWaveformData_hxx__
#define __ds_SCWaveformData_hxx__
/*
  ds::SCWaveformData.hxx
*/
#include "TObject.h"

namespace ds {

  class SCWaveformData : public TObject {
  public:
    SCWaveformData();
    ~SCWaveformData();

    void setTime(float x) { mTime = x; }
    void setPosition(float x, float y, float z);
    void setEnergyDeposit(float x) { mEnergyDeposit = x; }

    ClassDef(ds::SCWaveformData, 1)

  protected:
    float mTime;
    float mPosition[3];
    float mEnergyDeposit;

  };

}

#endif // __ds_SCWaveformData_hxx__
