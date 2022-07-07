/*
  Hit.cxx
*/
#include "TrackVertex/Hit.hxx"

ClassImp(Hit)

Hit::Hit() : mPosition(), mDetectorId(0) {
}

Hit::Hit(const Point& p, std::uint32_t detectorId) :
  mPosition(p), mDetectorId(detectorId) {
}

Hit::~Hit() {
}
