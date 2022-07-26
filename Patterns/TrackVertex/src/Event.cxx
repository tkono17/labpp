/*
  Event.cxx
*/
#include "TEllipse.h"
#include "TH1.h"

#include "TrackVertex/Event.hxx"

ClassImp(Event)

Event::Event() : TObject() {
}

Event::~Event() {
  clear();
}

void Event::addTrack(Track* track) {
  mTracks.push_back(track);
}

void Event::addTrack(const Track& track) {
  mTracks.push_back(new Track(track));
}

void Event::clear() {
  for (auto t: mTracks) {
    if (t) {
      delete t;
      t = nullptr;
    }
    mTracks.clear();
  }
}

void drawEvent(TPad* pad, const Event& event) {
  pad->Clear();
  TH1* hframe = pad->DrawFrame(-2.0E+3, -2.0E+3, 2.0E+3, 2.0E+3);
  auto point = event.generationPoint();
  float r = 1.0;
  TEllipse* c = new TEllipse(point.x(), point.y(), r);
  c->Draw();
  for (auto track: event.tracks()) {
    drawTrack(pad, *track);
  }
}
