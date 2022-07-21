#ifndef __Event_hxx__
#define __Event_hxx__
/*
  Event.hxx
*/
#include <vector>
#include "TObject.h"
#include "TrackVertex/Point.hxx"
#include "TrackVertex/Track.hxx"

class Event : public TObject {
public:
  Event();
  ~Event();

  void setGenerationPoint(const Point& p) { mGenerationPoint = p; }

  void addTrack(Track* track);

  void clear();
  
  ClassDef(Event, 1)
  
protected:
  Point mGenerationPoint;
  std::vector<Track*> mTracks;
  
};

#endif // __Event_hxx__
