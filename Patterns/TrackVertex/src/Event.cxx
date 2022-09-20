/*
  Event.cxx
*/
#include <iostream>
#include "TEllipse.h"
#include "TH1.h"

#include "TrackVertex/Event.hxx"

ClassImp(Event)

Event::Event() : TObject(), mTracks(), mHits(), mTrackHits() {
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

std::uint32_t Event::trackIndex(const Track* track) const {
  std::uint32_t i=0;
  for (i=0; i<mTracks.size(); ++i) {
    if (mTracks[i] == track) {
      std::cout << "Found matching track " << i << std::endl;
      break;
    }
  }
  return i;
}

bool Event::trackIndexValid(std::uint32_t index) const {
  return (index < mTracks.size());
}

void Event::addHitsOnTrack(std::uint32_t itrack, std::vector<Hit*>& hits) {
  IndexList hitList;
  if (mTrackHits.size() == 0) mTrackHits.assign(mTracks.size(), hitList);

  //  std::cout << "itrack " << itrack << "/" << mTracks.size()<< std::endl;
  if (trackIndexValid(itrack)) {
    if (mTrackHits.size() < mTracks.size()) {
      mTrackHits.resize(mTracks.size());
    }
    std::uint32_t ihit = mHits.size();
    hitList.clear();
    for (auto& hit: hits) {
      hitList.push_back(ihit);
      addHit(hit);
      ++ihit;
    }
    mTrackHits[itrack] = hitList;
  }
}

void Event::addHitsOnTrack(const Track* track, std::vector<Hit*>& hits) {
  auto itrack = trackIndex(track);
  addHitsOnTrack(itrack, hits);
}

Event::IndexList Event::hitsOnTrack(std::uint32_t itrack) const {
  IndexList v;
  if (itrack < mTrackHits.size()) {
    v = mTrackHits[itrack];
  }
  return v;
}

std::vector<Hit*> Event::getHits(const IndexList& hitIndices) const {
  std::vector<Hit*> hits;
  for (auto i: hitIndices) {
    std::uint32_t nhits = mHits.size();
    if (i < nhits) {
      hits.push_back(mHits[i]);
    }
  }
  return hits;
}

void Event::addHit(const Hit& hit) {
  mHits.push_back(new Hit(hit));
}

void Event::addHit(Hit* hit) {
  mHits.push_back(hit);
}

void Event::clear() {
  for (auto t: mTracks) {
    if (t) {
      delete t;
      t = nullptr;
    }
  }
  mTracks.clear();
  
  for (auto x: mHits) {
    if (x) {
      delete x;
      x = nullptr;
    }
  }
  mHits.clear();
  mTrackHits.clear();
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
