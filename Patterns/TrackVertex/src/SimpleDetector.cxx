/*
  SimpleDetector.cxx
*/
#include <cmath>
#include <iostream>

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

bool SimpleDetector::intersectionAtLayer(const Track& track,
					 std::uint32_t layer,
					 Point& point) const {
  bool ok = false;
  float r1 = radiusOfLayer(layer);
  Point c1(0.0, 0.0);

  if (r1 > 0.0) {
    auto& c2 = track.circleCenter();
    auto r2 = track.circleR();
    auto phi2 = track.circleStartPhi();
    float dr = std::fabs(r2 - r1);
    float l = c2.length();
    float phi0 = track.angleAtPerigee();
    float nx = std::cos(phi0);
    float ny = std::sin(phi0);

    // std::cout << "phi0 = " << phi0 << std::endl;
    // std::cout << "Intersection: r1=" << r1 <<
    //   ", (" << c1.x() << ", " << c1.y() << ")" << std::endl;
    // std::cout << "   r2=" << r2 <<
    //   ", (" << c2.x() << ", " << c2.y() << ")" << std::endl;
    
    if ( (l < r1 && l > dr) || (l < (r1 + r2) ) ) {
      ok = true;
      float x = 0.0;
      float y = 0.0;

      Point dc = c2 - c1;
      float cx = dc.x();
      float cy = dc.y();
      float c = dc.length();
      float ux = cx/c;
      float uy = cy/c;
      float e = (r1*r1 - r2*r2 + l*l)/(2.0*l);
      float cs = e/r1;
      float t = r1*std::sqrt(1.0 - cs*cs);
      float ax = c1.x() + e*ux;
      float ay = c1.y() + e*uy;
      // std::cout << "  c=" << c << ", u=(" << ux << ", " << uy << ")" << std::endl;
      // std::cout << "  t = " << t << std::endl;
      // std::cout << "  a = (" << ax << ", " << ay << ")" << std::endl;
      x = ax + t*nx;
      y = ay + t*ny;
      point.setData(x, y);
    }
  }
  
  return ok;
}

Hit SimpleDetector::smearHitAtLayer(const Hit& hit,
				    std::uint32_t /*layer*/, float resolution) {
  Hit hit1(hit);
  hit1.smear(resolution);
  return hit1;
}

