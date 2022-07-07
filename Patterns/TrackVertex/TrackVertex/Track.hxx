#ifndef __TrackVertex_Track_hxx__
#define __TrackVertex_Track_hxx__
/*
  Track.hxx
*/
#include <cstdint>
#include "TObject.h"

#include "TrackVertex/Point.hxx"

class Track : public TObject {
public:
  Track(float rho=0.0, float d0=0.0, float phi0=0.0);
  ~Track();

  void updateData(float rho, float d0, float phi0);

  ClassDef(Track, 1)
  
protected:
  std::uint32_t mNParameters;
  float mParameters[3]; // q/R, d0, phi0
  //
  float mCharge;
  float mPtAtDCA; // Pt at distance of closest appoach
  float mPhiAtDCA; // phi at distance of closest appoach
  Point mCircleCenter;
  float mCircleR;
  float mCircleStartPhi;
  bool mCircleClockwize;
};

#endif  // __TrackVertex_Track_hxx__
