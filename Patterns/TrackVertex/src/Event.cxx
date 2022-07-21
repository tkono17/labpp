/*
  Event.cxx
*/
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

void Event::clear() {
  for (auto t: mTracks) {
    if (t) {
      delete t;
      t = nullptr;
    }
    mTracks.clear();
  }
}

