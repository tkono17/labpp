#ifndef __Event_hxx__
#define __Event_hxx__
/*
  Event.hxx
*/
#include <cstdint>
#include <vector>
#include "TCanvas.h"
#include "TPad.h"

#include "TObject.h"
#include "TrackVertex/Point.hxx"
#include "TrackVertex/Track.hxx"
#include "TrackVertex/Hit.hxx"

class Event : public TObject {
public:
  typedef std::vector<std::uint32_t> IndexList;
  
public:
  Event();
  ~Event();

  void setGenerationPoint(const Point& p) { mGenerationPoint = p; }
  const Point& generationPoint() const { return mGenerationPoint; }
  
  void addTrack(const Track& track);
  void addTrack(Track* track);

  std::uint32_t trackIndex(const Track* track) const;
  bool trackIndexValid(std::uint32_t index) const;
  
  void addHitsOnTrack(const Track* track, std::vector<Hit*>& hits);
  void addHitsOnTrack(std::uint32_t itrack, std::vector<Hit*>& hits);
  
  void addHit(const Hit& hit);
  void addHit(Hit* hit);
  
  const std::vector<Hit*>& hits() const { return mHits; }
  
  const std::vector<Track*>& tracks() const { return mTracks; }
  
  void clear();
  
  ClassDef(Event, 1)
  
protected:
  Point mGenerationPoint;
  std::vector<Track*> mTracks;
  std::vector<Hit*> mHits;
  std::vector<IndexList> mTrackHits;
  
};

void drawEvent(TPad* pad, const Event& event);
  
#endif // __Event_hxx__
