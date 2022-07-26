/*
  Track.cxx
*/
#include <cmath>
#include "TMath.h"
#include "TArc.h"

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

void Track::setDataPPhiXY(float p, float phi, const Point& xy) {
  float B = 1.0; // [T}
  float r = p/(0.3*B) * 1.0E+3; // [mm]
  float rho = 1/r;

  float px = std::cos(phi);
  float py = std::sin(phi);
  float qx = py;
  float qy = -px;
  float cx = xy.x() + r*qx;
  float cy = xy.y() + r*qy;
  float c = std::sqrt(cx*cx + cy*cy);
  float dx = (c - r)*cx;
  float dy = (c - r)*cy;
  
  float d0 = std::sqrt(dx*dx + dy*dy);
  float phi0 = std::atan2(dy, dx) + TMath::Pi()/2.0;

  mCircleCenter.setData(cx, cy);
  mCircleR = std::fabs(r);
  mCircleStartPhi = phi;
  
  setData(rho, d0, phi0);
}

void Track::updateData(float rho, float d0, float phi0) {
  // Need to implement the calculation
  // mCharge = rho/std::fabs(rho);
  // mPtAtDCA = ....;
  // mCircleCenter = Point(x, y);
  // .....
}

void drawTrack(TPad* pad, const Track& track) {
  float r = track.circleR();
  const auto c = track.circleCenter();
  TArc* arc = new TArc(c.x(), c.y(), r, 0.0, 360.0);
  arc->Draw();
}
