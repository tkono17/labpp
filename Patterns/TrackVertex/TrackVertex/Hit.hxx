#ifndef __TrackVertex_Hit_hxx__
#define __TrackVertex_Hit_hxx__
/*
  Hit.hxx
*/
#include <cstdint>
#include "TObject.h"

#include "TrackVertex/Point.hxx"

class Hit : public TObject {
public:
  Hit();
  Hit(const Point& p, std::uint32_t detectorId);
  ~Hit();

  ClassDef(Hit, 1)
  
protected:
  Point mPosition;
  std::uint32_t mDetectorId;
};

#endif  // __TrackVertex_Hit_hxx__
