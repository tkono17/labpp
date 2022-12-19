#ifndef __TrackVertex_Track_hxx__
#define __TrackVertex_Track_hxx__
/*
  Track.hxx
*/
#include <cstdint>
#include "TObject.h"
#include "TPad.h"

#include "TrackVertex/Point.hxx"
#include "TrackVertex/Vertex.hxx"
#include "TrackVertex/Hit.hxx"

class Track : public TObject {
public:
  Track(float rho=0.0, float d0=0.0, float phi0=0.0);
  ~Track();

  float parameter(int i) const { return mParameters[i]; }
  void setParameter(int i, double p) { mParameters[i] = p; }
  const double* covarianceMatrix() const { return mCovarianceMatrix; }
  
  void updateData(float rho, float d0, float phi0);

  void setData(float rho, float d0, float phi0);
  
  void setDataPPhiXY(float p, float phi, const Point& xy,
		     float charge=1.0, float B=1.0);

  double rho() const { return mParameters[0]; }
  double d0() const { return mParameters[1]; }
  double phi0() const { return mParameters[2]; }
  
  double angleX0() const; // Angle to the point of closest approach

  double angleC() const; // Angle to the circle center
  
  Point x0() const;

  Point pointAt(double s) const;

  bool clockwize() const { return mParameters[0]>0; }
  bool turningLeftAtX0() const { return mParameters[1]>0; }
  
  float charge() const { return mCharge; }

  float radius() const { return mCircleR; }
  
  const Point& circleCenter() const { return mCircleCenter; }
  float circleR() const { return mCircleR; }
  float circleStartPhi() const { return mCircleStartPhi; }

  float angleAtPerigee() const;

  double distance(const Hit& hit) const;

  double distance(const Vertex& v) const;
  
  //  double distance(const Point& point) const;
  
  ClassDef(Track, 1)
  
protected:
  std::uint32_t mNParameters;
  float mParameters[3]; // q/R, d0, phi0
  double mCovarianceMatrix[9];
  //
  float mCharge;
  float mPtAtDCA; // Pt at distance of closest appoach
  float mPhiAtDCA; // phi at distance of closest appoach
  Point mCircleCenter;
  float mCircleR;
  float mCircleStartPhi;
  bool mCircleClockwize;
};

void drawTrack(TPad* pad, const Track& track);

#endif  // __TrackVertex_Track_hxx__
