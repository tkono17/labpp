/*
  Vector.cxx
*/
#include "TrackVertex/Vector.hxx"

ClassImp(Vector)

Vector::Vector(float x, float y) : mN(2) {
  mX[0] = x;
  mX[1] = y;
}

Vector::~Vector() {
}

void Vector::setData(float x, float y) {
  mX[0] = x;
  mX[1] = y;
}

float Vector::abs() const {
  return length();
}

float Vector::abs2() const {
  return (mX[0]*mX[0] + mX[1]*mX[1]);
}

Vector Vector::operator*(double s) const {
  Vector v(s*x(), s*y());
  return v;
}

Vector Vector::operator+(const Vector& p) const {
  float x1 = x() + p.x();
  float y1 = y() + p.y();
  return Vector(x1, y1);
}

Vector Vector::operator-(const Vector& p) const {
  float x1 = x() - p.x();
  float y1 = y() - p.y();
  return Vector(x1, y1);
}

void Vector::clear() {
  setData(0.0, 0.0);
}

Vector operator*(double s, const Vector& v) {
  Vector v1(s*v.x(), s*v.y());
  return v1;
}
