#ifndef __McVertex_hxx__
#define __McVertex_hxx__
/*
  McVertex.hxx
*/
#include "TObject.h"
#include "TLorentzVector.h"
#include "TVector3.h"

class McVertex : public TObject {
public:
  McVertex();
  McVertex(float x, float y, float z, float t);
  ~McVertex();

  void setPosition(float x, float y, float z, float t);
  TLorentzVector position() const;

  ClassDef(McVertex, 1)

protected:
  Float_t mX;
  Float_t mY;
  Float_t mZ;
  Float_t mT;
};

#endif // __McVertex_hxx__
