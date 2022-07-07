#ifndef __TrackVertex_Square_hxx__
#define __TrackVertex_Square_hxx__
/*
  Square.hxx
*/
#include <cstdint>
#include "TObject.h"

#include "TrackVertex/Point.hxx"

class Square : public TObject {
public:
  Square();
  Square(float lengths[2], float x, float y, float angle);
  ~Square();

  ClassDef(Square, 1)
  
protected:
  float mLengths[2];
  Point mCenter;
  float mAngle;
};

#endif  // __TrackVertex_Square_hxx__
