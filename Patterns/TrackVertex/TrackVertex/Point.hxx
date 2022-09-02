#ifndef __TrackVertex_Point_hxx__
#define __TrackVertex_Point_hxx__
/*
  Point.hxx
*/
#include <cstdint>
#include <cmath>
#include "TObject.h"

class Point : public TObject {
public:
  Point(float x=0.0, float y=0.0);
  ~Point();

  void setData(float x, float y);

  float x() const { return mX[0]; }
  float y() const { return mX[1]; }

  Point operator+(const Point& p) const;
  Point operator-(const Point& p) const;

  float abs() const;
  float abs2() const;
  float length() const { return std::sqrt(mX[0]*mX[0] + mX[1]*mX[1]); }
  
  void clear();

  ClassDef(Point, 1)
  
protected:
  std::uint32_t mN; // ==2
  float mX[2];
};

#endif  // __TrackVertex_Point_hxx__
