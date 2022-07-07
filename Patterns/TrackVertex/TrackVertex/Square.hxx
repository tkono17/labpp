#ifndef __TrackVertex_Square_hxx__
#define __TrackVertex_Square_hxx__
/*
  Square.hxx
*/
#include <cstdint>
#include "TrackVertex/Point.hxx"

class Square {
public:
  Square();
  Square(float lengths[2], float x, float y, float angle);
  ~Square();
  
protected:
  float mLengths[2];
  Point mCenter;
  float mAngle;
};

#endif  // __TrackVertex_Square_hxx__
