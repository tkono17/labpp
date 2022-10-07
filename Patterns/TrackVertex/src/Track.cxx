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
  setData(rho, d0, phi0);
}

Track::~Track() {
}

void Track::setData(float rho, float d0, float phi0) {
  mParameters[0] = rho;
  mParameters[1] = d0;
  mParameters[2] = phi0;
  updateData(rho, d0, phi0);
}

void Track::setDataPPhiXY(float p, float phi, const Point& xy,
			  float charge, float B) {
  float r = p/(0.3*B) * 1.0E+3; // [mm]
  float rho = charge/r;
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
  float phi0 = std::atan2(cy, cx);

  if (charge > 0.0 && c >= r) {
    phi0 += TMath::Pi()/2.0;
  } else if (charge > 0.0 && c < r) {
    d0 -= -1.0;
    phi0 -= TMath::Pi()/2.0;
  } else if (charge < 0.0 && c >= r) {
    phi0 -= TMath::Pi()/2.0;
  } else if (charge < 0.0 && c < r) {
    d0 -= -1.0;
    phi0 += TMath::Pi()/2.0;
  }

  // std::cout << "Track p=" << p << ", phi=" << phi << std::endl;
  // std::cout << "Track phi0: " << phi0
  // 	    << " u=(" << ucx << ", " << ucy << ")" 
  // 	    << " dx=(" << dx << ", " << dy << ")" 
  // 	    << std::endl;
  setData(rho, d0, phi0);
  // std::cout << "Track cx,cy=(" << cx << ", " << cy << ")" << std::endl;
}

void Track::updateData(float rho, float d0, float phi0) {
  // Need to implement the calculation
  // mCharge = rho/std::fabs(rho);
  // mPtAtDCA = ....;
  // mCircleCenter = Point(x, y);
  // .....
  if (std::fabs(rho) < 1.0E-10) {
    if (rho >= 0.0) {
      rho = 1.0E-7;
    } else {
      rho = -1.0E-10;
    }
  }
  double R = std::fabs(1.0/rho);
  double a = R + d0;
  if (rho < 0.0) a = R - d0;
  
  double beta = phi0 - TMath::Pi()/2.0;
  if (rho < 0.0) beta = phi0 + TMath::Pi()/2.0;
  double cx = a*std::cos(beta);
  double cy = a*std::sin(beta);
  //  std::cout << "beta = " << beta << std::endl;
  mCharge = rho/std::fabs(rho);
  mCircleCenter = Point(cx, cy);
  mCircleR = R;
  mCircleStartPhi = std::atan2(-cy, -cx);
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
  //  arc->Draw("only");
}

double Track::distance(const Hit& hit) const {
  auto p = hit.position();
  double d = p.distance(mCircleCenter) - mCircleR;
  return d;
}
