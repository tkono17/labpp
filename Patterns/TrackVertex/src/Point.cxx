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

float Point::abs() const {
  return length();
}

float Point::abs2() const {
  return (mX[0]*mX[0] + mX[1]*mX[1]);
}

Point Point::operator+(const Vector& v) const {
  float x1 = x() + v.x();
  float y1 = y() + v.y();
  return Point(x1, y1);
}

Point Point::operator+(const Point& p) const {
  float x1 = x() + p.x();
  float y1 = y() + p.y();
  return Point(x1, y1);
}

Point Point::operator-(const Vector& v) const {
  float x1 = x() - v.x();
  float y1 = y() - v.y();
  return Point(x1, y1);
}

Point Point::operator-(const Point& p) const {
  float x1 = x() - p.x();
  float y1 = y() - p.y();
  return Point(x1, y1);
}

float Point::distance(const Point& p) const {
  Point dp = (*this) - p;
  return dp.length();
}

void Point::clear() {
  setData(0.0, 0.0);
}

