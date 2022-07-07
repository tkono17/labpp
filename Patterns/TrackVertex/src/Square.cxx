/*
  Square.cxx
*/
#include "TrackVertex/Square.hxx"

Square::Square() {
  mLengths[0] = 0.0;
  mLengths[1] = 0.0;
  mCenter.clear();
  mAngle = 0.0;
}

Square::Square(float lengths[2], float x, float y, float angle) {
  mLengths[0] = lengths[0];
  mLengths[1] = lengths[1];
  mCenter.setData(x, y);
  mAngle = angle;
}

Square::~Square() {
}

