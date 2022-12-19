#ifndef __TrackVertex_Vector_hxx__
#define __TrackVertex_Vector_hxx__
/*
  Vector.hxx
*/
#include <cstdint>
#include <cmath>
#include "TObject.h"

class Vector : public TObject {
public:
  Vector(float x=0.0, float y=0.0);
  ~Vector();

  void setData(float x, float y);

  float x() const { return mX[0]; }
  float y() const { return mX[1]; }

  Vector operator*(double s) const;
  Vector operator+(const Vector& p) const;
  Vector operator-(const Vector& p) const;

  float abs() const;
  float abs2() const;
  float length() const { return std::sqrt(mX[0]*mX[0] + mX[1]*mX[1]); }

  void clear();

  ClassDef(Vector, 1)
  
protected:
  std::uint32_t mN; // ==2
  float mX[2];
};

Vector operator*(double s, const Vector& v);

#endif  // __TrackVertex_Vector_hxx__
