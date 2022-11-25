/*
  Vertex.cxx
*/
#include "TrackVertex/Vertex.hxx"

ClassImp(Vertex)

Vertex::Vertex() : TObject(), mPosition() {
}

Vertex::Vertex(double vx, double vy) : TObject(), mPosition(vx, vy) {
}

Vertex::~Vertex() {
}

