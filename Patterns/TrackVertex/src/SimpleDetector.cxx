/*
  SimpleDetector.cxx
*/
#include <cmath>

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
    
    if ( (l < r1 && l > dr) || (l < (r1 + r2) ) ) {
      ok = true;
      float x = 0.0;
      float y = 0.0;

      Point dc = c2 - c1;
      float a = 2.0*dc.x();
      float b = 2.0*dc.y();
      float c = r1*r1 - r2*r2 - (c1.abs2() - c2.abs2());
      if (std::fabs(a) >= std::fabs(b)) {
	float A1 = -b/a;
	float B1 = c/a + c1.x();
	float A = A1*A1 + 1;
	float B = -A1*B1 - c1.y();
	float C = c1.y()*c1.y() + B1*B1 - r1*r1;
	if (ny > 0.0) {
	   y = (-B + std::sqrt(B*B - A*C) )/A;
	} else {
	   y = (-B - std::sqrt(B*B - A*C) )/A;
	}
	x = -(b*y + c)/a;
      } else {
	float A1 = -a/b;
	float B1 = c/b + c1.y();
	float A = A1*A1 + 1;
	float B = -A1*B1 - c1.x();
	float C = c1.x()*c1.x() + B1*B1 - r1*r1;
	if (nx > 0.0) {
	  x = (-B + std::sqrt(B*B - A*C) )/A;
	} else {
	  x = (-B - std::sqrt(B*B - A*C) )/A;
	}
	y = -(a*x + c)/b;
      }
      point.setData(x, y);
    }
  }
  
  return ok;
}

