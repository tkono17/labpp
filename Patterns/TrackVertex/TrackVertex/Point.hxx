#ifndef __TrackVertex_Point_hxx__
#define __TrackVertex_Point_hxx__
/*
  Point.hxx
*/
#include <cstdint>
#include "TObject.h"

class Point : public TObject {
public:
  Point(float x=0.0, float y=0.0);
  ~Point();

  void setData(float x, float y);

  void clear();

  ClassDef(Point, 1)
  
protected:
  std::uint32_t mN; // ==2
  float mX[2];
};

#endif  // __TrackVertex_Point_hxx__
