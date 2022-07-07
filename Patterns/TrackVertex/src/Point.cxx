/*
  Point.cxx
*/
#include "TrackVertex/Point.hxx"

ClassImp(Point)

Point::Point(float x, float y) : mN(2) {
  mX[0] = x;
  mX[1] = y;
}

Point::~Point() {
}

void Point::setData(float x, float y) {
  mX[0] = x;
  mX[1] = y;
}

void Point::clear() {
  setData(0.0, 0.0);
}

