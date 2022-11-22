#ifndef __ds_SimParticle_hxx__
#define __ds_SimParticle_hxx__
/*
  ds::SimParticle.hxx
*/
#include "TObject.h"

namespace ds {

  class SimParticle {//: public TObject {
  public:
    SimParticle();
    ~SimParticle();

    void setID(int id) { mID = id; }

    void setP(float px, float py, float pz, float e);
    
    void setVertexIndex(int i) { mVertexIndex = i; }

    //    ClassDef(ds::SimParticle, 1)

  protected:
    Int_t mID;
    Float_t mP[4];
    Int_t mVertexIndex;
  };

}

#endif // __ds_SimParticle_hxx__
