/*
  EventDisplay.cxx
*/
#include <iostream>

#include "TH1.h"
#include "TArc.h"
#include "TEllipse.h"
#include "TMath.h"

#include "TrackVertex/EventDisplay.hxx"

ClassImp(EventDisplay)

EventDisplay::EventDisplay() : TObject(),
  mHalfWidth(0), mHalfHeight(0) {
  setPad(nullptr);
  mFrameReady = false;
  mFrame = nullptr;
}

EventDisplay::EventDisplay(float halfwidth, float halfheight) :
  TObject() {
  mHalfWidth = halfwidth;
  mHalfHeight = halfheight;
  mPad = nullptr;
  mFrameReady = false;
  mFrame = nullptr;
}

EventDisplay::~EventDisplay() {
}
  
void EventDisplay::drawFrame() {
  if (!mFrameReady && mPad) {
    float w = mHalfWidth;
    float h = mHalfHeight;
    mFrame = mPad->DrawFrame(-w, -h, w, h);
    mFrameReady = true;
  }
}

void EventDisplay::drawEvent(const Event& event) {
  if (mPad == nullptr) {
    std::cout << "No pad is set" << std::endl;
    return;
  }
  drawFrame();
  
  auto point = event.generationPoint();
  float r = 1.0;
  TEllipse* c = new TEllipse(point.x(), point.y(), r);
  c->Draw();

  for (auto track: event.tracks()) {
    drawTrack(*track, kBlue);
  }

  for (auto hit: event.hits()) {
    drawHit(*hit);
  }
}

void EventDisplay::drawEvent(const Event& event,
			     const SimpleDetector& det) {
  if (mPad == nullptr) {
    std::cout << "No pad is set" << std::endl;
    return;
  }
  drawFrame();
  drawEvent(event);
  drawDetector(det);
}

void EventDisplay::drawHit(const Hit& hit) {
  if (mPad) {
    float r = 5.0;
    auto& p = hit.position();
    TEllipse* c = new TEllipse(p.x(), p.y(), r);
    c->SetLineColor(kRed);
    c->SetFillStyle(0);
    c->Draw();
  }
}

void EventDisplay::drawTrack(const Track& track, int color) {
  if (mPad) {
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
    float cx = c.x();
    float cy = c.y();

    const float deg = 180.0/TMath::Pi();

    // std::cout << "Track rho: " << track.parameter(0) << std::endl;
    // std::cout << "Track d0: " << track.parameter(1) << std::endl;
    // std::cout << "Track phi0: " << track.parameter(2)*deg << std::endl;
    // std::cout << "Eventdisplay draw track" << std::endl;
    // std::cout << "  r = " << r << ", startPhi: " << phi1 << " - " << phi2 
    //  	      << ", cx=" << cx << ", cy=" << cy << std::endl;
    
    TArc* arc = new TArc(c.x(), c.y(), r, phi1*deg, phi2*deg);
    arc->SetLineColor(color);
    arc->SetLineWidth(1);
    arc->SetFillStyle(0);
    arc->Draw("only");
    // std::cout << "arc: c=(" << c.x() << ", " << c.y()
    // 	    << " r=" << r
    // 	    << " phi1=" << phi1*deg
    // 	    << " phi2=" << phi2*deg
    // 	    << std::endl;

  }
}

void EventDisplay::drawDetector(const SimpleDetector& det) {
  if (mPad) {
    auto n = det.nLayers();
    float r = 0.0;
    for (std::uint32_t i=0; i<n; ++i) {
      r = det.radiusOfLayer(i);
      TEllipse* c = new TEllipse(0.0, 0.0, r);
      c->SetLineStyle(2);
      c->SetFillStyle(0);
      c->Draw();
    }
  }
}

void EventDisplay::clear() {
  if (mPad) {
    mPad->Clear();
    mFrameReady = false;
    mFrame = nullptr;
  }
}
