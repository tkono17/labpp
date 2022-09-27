#ifndef __EventDisplay_hxx__
#define __EventDisplay_hxx__
/*
  EventDisplay.hxx
*/
#include <cstdint>

#include "TObject.h"
#include "TPad.h"
#include "TH1.h"

#include "TrackVertex/Event.hxx"
#include "TrackVertex/SimpleDetector.hxx"
#include "TrackVertex/Hit.hxx"
#include "TrackVertex/Track.hxx"

class EventDisplay : public TObject {
public:
  EventDisplay();
  EventDisplay(float halfwidth, float halfheight);
  ~EventDisplay();

  void setHalfWidth(float x) { mHalfWidth = x; }
  void setHalfHeight(float x) { mHalfHeight = x; }
  
  void setPad(TPad* pad) { mPad = pad; }

  void drawFrame();
  
  void drawEvent(const Event& event);
  
  void drawEvent(const Event& event, const SimpleDetector& det);

  void drawHit(const Hit& hit);
  
  void drawTrack(const Track& track, int color=2);

  void drawDetector(const SimpleDetector& det);
  
  void clear();

  ClassDef(EventDisplay, 1)
  
private:
  float mHalfWidth;
  float mHalfHeight;
  TPad* mPad;
  bool mFrameReady;
  TH1* mFrame;
};

#endif // __EventDisplay_hxx__
