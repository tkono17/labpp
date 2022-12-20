#ifndef __TrackVertex_Track_hxx__
#define __TrackVertex_Track_hxx__
/*
  Track.hxx
*/
#include <cstdint>
#include "TObject.h"
#include "TPad.h"

#include "TrackVertex/Point.hxx"
#include "TrackVertex/Vector.hxx"
#include "TrackVertex/Vertex.hxx"
#include "TrackVertex/Hit.hxx"

class Track : public TObject {
public:
  Track(double rho=0.0, double d0=0.0, double phi0=0.0);
  ~Track();

  const double* parameters() const { return mParameters; }
  double parameter(int i) const { return mParameters[i]; }
  void setParameter(int i, double p) { mParameters[i] = p; }
  void setParameters(const double* p);

  void setCovMatrix(const double* cov);
  const double* covarianceMatrix() const { return mCovarianceMatrix; }

  void setChi2(double x) { mChi2 = x; }
  double chi2() const { return mChi2; }
  
  void updateData(double rho, double d0, double phi0);

  void setData(double rho, double d0, double phi0);
  
  void setDataPPhiXY(double p, double phi, const Point& xy,
		     double charge=1.0, double B=1.0);

  double rho() const { return mParameters[0]; }
  double d0() const { return mParameters[1]; }
  double phi0() const { return mParameters[2]; }
  
  double angleX0() const; // Angle to the point of closest approach

  double angleC() const; // Angle to the circle center
  
  Point x0() const;

  Vector directionAt(double s) const;
  Point pointAt(double s) const;

  bool clockwize() const { return mParameters[0]>0; }
  bool turningLeftAtX0() const { return mParameters[1]>0; }
  
  double charge() const { return mCharge; }

  double radius() const { return mCircleR; }
  
  const Point& circleCenter() const { return mCircleCenter; }
  double circleR() const { return mCircleR; }
  double circleStartPhi() const { return mCircleStartPhi; }

  double angleAtPerigee() const;

  double distance(const Hit& hit) const;

  double distance(const Vertex& v) const;
  
  //  double distance(const Point& point) const;
  
  ClassDef(Track, 1)
  
protected:
  std::uint32_t mNParameters;
  double mParameters[3]; // q/R, d0, phi0
  double mCovarianceMatrix[9];
  double mChi2;
  //
  double mCharge;
  double mPtAtDCA; // Pt at distance of closest appoach
  double mPhiAtDCA; // phi at distance of closest appoach
  Point mCircleCenter;
  double mCircleR;
  double mCircleStartPhi;
  bool mCircleClockwize;
};

void drawTrack(TPad* pad, const Track& track);

#endif  // __TrackVertex_Track_hxx__
