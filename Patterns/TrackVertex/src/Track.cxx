/*
  Track.cxx
*/
#include "TrackVertex/Track.hxx"

ClassImp(Track)

Track::Track(float rho, float d0, float phi0) {
  mNParameters = 3;
  mParameters[0] = rho;
  mParameters[1] = d0;
  mParameters[2] = phi0;
}

Track::~Track() {
}

void Track::setData(float rho, float d0, float phi0) {
  mParameters[0] = rho;
  mParameters[1] = d0;
  mParameters[2] = phi0;
  updateData(rho, d0, phi0);
}

void Track::setDataFrom(float p, float phi, const Point& xy) {
  // float rho = ...;
  // float d0 = ...;
  // float phi0 = ...;
  setData(rho, d0, phi0);
}

void Track::updateData(float rho, float d0, float phi0) {
  // Need to implement the calculation
  // mCharge = rho/std::fabs(rho);
  // mPtAtDCA = ....;
  // mCircleCenter = Point(x, y);
  // .....
}

  
