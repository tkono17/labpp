/*
  SimpleDetector.cxx
*/
#include "TrackVertex/SimpleDetector.hxx"

ClassImp(SimpleDetector)

using namespace std;

SimpleDetector::SimpleDetector() :
  TNamed("detector", ""), 
  mNLayers(0), mRMin(0.0), mRMax(0.0) {
}

SimpleDetector::SimpleDetector(std::uint32_t nlayers, float rmin, float rmax) :
  TNamed("detector", ""), 
  mNLayers(nlayers), mRMin(rmin), mRMax(rmax) {
  if (nlayers > 0) {
    mRadiusList.assign(mNLayers, 0.0);
    float dr = (rmax - rmin)/(nlayers-1);
    float r = 0.0;
    for (std::uint32_t i=0; i<nlayers; ++i) {
      r = rmin + dr*i;
      mRadiusList[i] = r;
    }
  }
}

SimpleDetector::~SimpleDetector() {
}

float SimpleDetector::radiusOfLayer(std::uint32_t layer) const {
  float r = 0.0;
  if (layer < mRadiusList.size()) {
    r = mRadiusList[layer];
  }
  return r;
}

bool intersectionAtLayer(const Track& track, std::uint32_t layer) const {
  bool ok = false;
  float r = radiusOfLayer(layer);

  if (r > 0.0) {
    auto& c = track.circleCenter();
    auto r1 = track.circleRadius();
    if (c.length() < (r + r1)) {
    }
  }
  
  return ok;
}

