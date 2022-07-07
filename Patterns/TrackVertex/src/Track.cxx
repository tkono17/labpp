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

void Track::updateDate(float rho, float d0, float phi0) {
  // Need to implement the calculation
}

  
