/*
  Track.cxx
*/
#include <iostream>
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

void Track::setDataPPhiXY(float p, float phi, const Point& xy, float charge) {
  float B = 1.0; // [T}
  float r = p/(0.3*B) * 1.0E+3; // [mm]
  float rho = 1/r;
  mCharge = charge;

  float px = std::cos(phi);
  float py = std::sin(phi);
  float qx = py;
  float qy = -px;
  if (charge > 0.0) {
    qx = py;
    qy = -px;
  } else {
    qx = -py;
    qy = px;
  }
  float cx = xy.x() + r*qx;
  float cy = xy.y() + r*qy;
  float c = std::sqrt(cx*cx + cy*cy);
  float ucx = cx/c;
  float ucy = cy/c;
  float dx = (c - r)*ucx;
  float dy = (c - r)*ucy;
  float d0 = std::sqrt(dx*dx + dy*dy);
  float phi0 = std::atan2(dy, dx) + TMath::Pi()/2.0;
  if (charge > 0.0 && c >= r) {
    dx = (c - r)*ucx;
    dy = (c - r)*ucy;
    d0 = std::sqrt(dx*dx + dy*dy);
    phi0 = std::atan2(dy, dx) + TMath::Pi()/2.0;
  } else if (charge > 0.0 && c < r) {
    dx = (c - r)*ucx;
    dy = (c - r)*ucy;
    d0 = -std::sqrt(dx*dx + dy*dy);
    phi0 = std::atan2(dy, dx) - TMath::Pi()/2.0;
  } else if (charge < 0.0 && c >= r) {
    dx = (c - r)*ucx;
    dy = (c - r)*ucy;
    d0 = +std::sqrt(dx*dx + dy*dy);
    phi0 = std::atan2(dy, dx) - TMath::Pi()/2.0;
  } else if (charge < 0.0 && c < r) {
    dx = (c - r)*ucx;
    dy = (c - r)*ucy;
    d0 = -std::sqrt(dx*dx + dy*dy);
    phi0 = std::atan2(dy, dx) + TMath::Pi()/2.0;
  }

  mCircleCenter.setData(cx, cy);
  mCircleR = std::fabs(r);
  mCircleStartPhi = std::atan2(-qy, -qx);
  
  setData(rho, d0, phi0);
}

void Track::updateData(float rho, float d0, float phi0) {
  // Need to implement the calculation
  // mCharge = rho/std::fabs(rho);
  // mPtAtDCA = ....;
  // mCircleCenter = Point(x, y);
  // .....
}

float Track::angleAtPerigee() const {
  // float qOverR = mParameters[0];
  // float d0 = mParameters[1];
  float phi0 = mParameters[2];
  // float phi = 0.0;

  return phi0;
}

void drawTrack(TPad* pad, const Track& track) {
  float r = track.circleR();
  auto& c = track.circleCenter();
  float phi1 = track.circleStartPhi();
  float phi2 = track.circleStartPhi();
  auto charge = track.charge();
  float l = 2000.0;
  float dphi = l/r;
  if (charge > 0.0) {
    phi2 = phi1 - dphi;
  } else {
    phi2 = phi1 + dphi;
  }

  const float deg = 180.0/TMath::Pi();
  
  TArc* arc = new TArc(c.x(), c.y(), r, phi1*deg, phi2*deg);
  arc->SetLineColor(kBlue-2);
  arc->SetLineWidth(1);
  arc->SetFillStyle(0);
  arc->Draw("only");
}
