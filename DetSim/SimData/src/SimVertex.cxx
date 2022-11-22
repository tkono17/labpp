/*
  ds::SimVertex.cxx
*/
#include "SimData/SimVertex.hxx"

//ClassImp(ds::SimVertex)

namespace ds {

  SimVertex::SimVertex() {
    for (int i=0; i<4; ++i) {
      mX[i] = 0.0;
    }
  }

  SimVertex::SimVertex(float x, float y, float z, float t) {
    mX[0] = x;
    mX[1] = y;
    mX[2] = z;
    mX[3] = t;
  }

  SimVertex::~SimVertex() {
  }

}
