#ifndef __ds_SimVertex_hxx__
#define __ds_SimVertex_hxx__
/*
  ds::SimVertex.hxx
*/
#include "TObject.h"

namespace ds {

  class SimVertex {//: public TObject {
  public:
    SimVertex();
    SimVertex(float x, float y, float z, float t);
    ~SimVertex();

    //    ClassDef(ds::SimVertex, 1)
    
  protected:
    Float_t mX[4];
  };
}

#endif // __ds_SimVertex_hxx__
