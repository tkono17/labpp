/*
  Track.cxx
*/
#include <iostream>
#include <cmath>
#include "TMath.h"
#include "TArc.h"

#include "TrackVertex/Track.hxx"
#include "TrackVertex/Vertex.hxx"

ClassImp(Track)

Track::Track(double rho, double d0, double phi0) {
  int i;

  mNParameters = 3;
  setData(rho, d0, phi0);
  
  mChi2 = 0.0;
  for (i=0; i<9; ++i) {
    mCovarianceMatrix[i] = 0.0;
  }
}

Track::~Track() {
}

void Track::setData(double rho, double d0, double phi0) {
  mParameters[0] = rho;
  mParameters[1] = d0;
  mParameters[2] = phi0;
  updateData(rho, d0, phi0);
}

void Track::setParameters(const double* p) {
  setData(p[0], p[1], p[2]);
}

void Track::setCovMatrix(const double* cov) {
  int i;
  for (i=0; i<9; ++i) {
    mCovarianceMatrix[i] = cov[i];
  }
}

void Track::setDataPPhiXY(double p, double phi, const Point& xy,
			  double charge, double B) {
  double r = p/(0.3*B) * 1.0E+3; // [mm]
  double rho = charge/r;
  mCharge = charge;

  double px = std::cos(phi);
  double py = std::sin(phi);
  double qx = py;
  double qy = -px;
  if (charge > 0.0) {
    qx = py;
    qy = -px;
  } else {
    qx = -py;
    qy = px;
  }
  double cx = xy.x() + r*qx;
  double cy = xy.y() + r*qy;
  double c = std::sqrt(cx*cx + cy*cy);
  double ucx = cx/c;
  double ucy = cy/c;
  double dx = (c - r)*ucx;
  double dy = (c - r)*ucy;
  double d0 = std::sqrt(dx*dx + dy*dy);
  double phi0 = std::atan2(cy, cx);

  if (charge > 0.0 && c >= r) {
    phi0 += TMath::Pi()/2.0;
  } else if (charge > 0.0 && c < r) {
    d0 *= -1.0;
    phi0 += TMath::Pi()/2.0;
  } else if (charge < 0.0 && c >= r) {
    d0 *= -1.0;
    phi0 -= TMath::Pi()/2.0;
  } else if (charge < 0.0 && c < r) {
    phi0 -= TMath::Pi()/2.0;
  }

  // std::cout << "Track p=" << p << ", phi=" << phi << std::endl;
  // std::cout << "Track phi0: " << phi0
  // 	    << " u=(" << ucx << ", " << ucy << ")" 
  // 	    << " dx=(" << dx << ", " << dy << ")" 
  // 	    << std::endl;
  setData(rho, d0, phi0);
  // std::cout << "Track cx,cy=(" << cx << ", " << cy << ")" << std::endl;
}

void Track::updateData(double rho, double d0, double phi0) {
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

double Track::angleAtPerigee() const {
  // double qOverR = mParameters[0];
  // double d0 = mParameters[1];
  double phi0 = mParameters[2];
  // double phi = 0.0;

  return phi0;
}

void drawTrack(TPad* pad, const Track& track) {
  double r = track.circleR();
  auto& c = track.circleCenter();
  double phi1 = track.circleStartPhi();
  double phi2 = track.circleStartPhi();
  auto charge = track.charge();
  double l = 2000.0;
  double dphi = l/r;
  if (charge > 0.0) {
    phi2 = phi1 - dphi;
  } else {
    phi2 = phi1 + dphi;
  }

  const double deg = 180.0/TMath::Pi();
  
  TArc* arc = new TArc(c.x(), c.y(), r, phi1*deg, phi2*deg);
  arc->SetLineColor(kBlue-2);
  arc->SetLineWidth(1);
  arc->SetFillStyle(0);
  //  arc->Draw("only");
}

Point Track::x0() const {
  double pi = TMath::Pi();
  double d = std::fabs(mParameters[1]);
  double alpha = angleX0();
  return Point(d*std::cos(alpha), d*std::sin(alpha));
}

Point Track::pointAt(double s) const {
  double pi = TMath::Pi();
  
  s = std::fabs(s);
  if (clockwize()) s *= -1.0;
  double r = radius();
  double theta = s/r;
  double u = r*std::sin(theta);
  double v = r*(1.0 - std::cos(theta));
  double uangle = phi0();
  if (clockwize()) {
    uangle += pi;
  }
  double ca = std::cos(uangle);
  double sa = std::sin(uangle);
  double x = ca*u + -sa*v;
  double y = sa*u + ca*v;

  // std::cout << "pars=" << mParameters[0] << ", " << mParameters[1]
  // 	    << ", " << mParameters[2] << std::endl;
  // std::cout << "r=" << r << ", theta=" << theta*180.0/pi << std::endl;
  // std::cout << "u=" << u << ", v=" << v << std::endl;
  // std::cout << "uangle=" << uangle << ", x=" << x << ", y=" << y << std::endl;
  
  return x0() + Point(x, y);
}

Vector Track::directionAt(double s) const {
  double phi0 = mParameters[2];
  Vector v(std::cos(phi0), std::sin(phi0));
  return v;
}
double Track::angleX0() const {
  double pi = TMath::Pi();
  double alpha = mParameters[2] + pi/2.0;
  if (turningLeftAtX0()) {
    alpha = mParameters[2] - pi/2.0;
  }
  //  std::cout << "  alpha: " << alpha*180.0/pi << std::endl;
  return alpha;
}

double Track::angleC() const {
  double pi = TMath::Pi();
  double beta = mParameters[2] + pi/2.0;
  if (turningLeftAtX0() && clockwize() || !turningLeftAtX0() && !clockwize()) {
    beta = mParameters[2] - pi/2.0;
  }
  //  std::cout << "  beta: " << beta*180.0/pi << std::endl;
  return beta;
}

double Track::distance(const Hit& hit) const {
  auto p0 = hit.position();
  double d = p0.distance(mCircleCenter) - mCircleR;
  // std::cout << "   distance: p=(" << p0.x() << ", " << p0.y()
  // 	    << ", R=" << mCircleR << ", d=" << d << std::endl;
  return d;
}

double Track::distance(const Vertex& v) const {
  double d = std::fabs(v.position().distance(mCircleCenter) - mCircleR);
  return d;
}

// double Track::distance(const Point& point) const {
//   double d = std::fabs(point.distance(mCircleCenter) - mCircleR);
//   return d;
// }
