/*
  Hit.cxx
*/
#include <cmath>

#include "TRandom3.h"

#include "TrackVertex/Hit.hxx"

ClassImp(Hit)

Hit::Hit() : mPosition(), mDetectorId(0) {
}

Hit::Hit(const Point& p, std::uint32_t detectorId) :
  mPosition(p), mDetectorId(detectorId) {
}

Hit::~Hit() {
}

float Hit::phi() const {
  return std::atan2(mPosition.y(), mPosition.x());
}

void Hit::smear(float sigma) {
  if (gRandom == nullptr) {
    TRandom3 r;
    r.Uniform(0.0, 1.0);
  }
  if (gRandom) {
    float dl = gRandom->Gaus(0.0, sigma);
    float x = mPosition.x();
    float y = mPosition.y();
    float r = std::sqrt(x*x + y*y);
    float phi = std::atan2(y, x);
    float dphi = dl/r;
    phi += dphi;
    mPosition.setData(r*std::cos(phi), r*std::sin(phi));
  }
}
