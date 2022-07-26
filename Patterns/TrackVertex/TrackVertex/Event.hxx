#ifndef __Event_hxx__
#define __Event_hxx__
/*
  Event.hxx
*/
#include <vector>
#include "TCanvas.h"
#include "TPad.h"

#include "TObject.h"
#include "TrackVertex/Point.hxx"
#include "TrackVertex/Track.hxx"

class Event : public TObject {
public:
  Event();
  ~Event();

  void setGenerationPoint(const Point& p) { mGenerationPoint = p; }
  const Point& generationPoint() const { return mGenerationPoint; }
  
  void addTrack(const Track& track);
  void addTrack(Track* track);

  const std::vector<Track*>& tracks() const { return mTracks; }
  
  void clear();
  
  ClassDef(Event, 1)
  
protected:
  Point mGenerationPoint;
  std::vector<Track*> mTracks;
  
};

void drawEvent(TPad* pad, const Event& event);
  
#endif // __Event_hxx__
