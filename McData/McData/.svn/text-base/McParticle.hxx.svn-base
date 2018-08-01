#ifndef __McParticle_hxx__
#define __McParticle_hxx__
/*
  McParticle.hxx
*/
#include "TLorentzVector.h"
class McParticle : public TObject {
public:
  McParticle();
  McParticle(float px, float py, float pz, float e);
  ~McParticle();

  void setPDGID(int id) { mPDGID = id; }
  void setProdVertexIndex(int i) { mProdVertexIndex = i; }
  void setEndVertexIndex(int i) { mEndVertexIndex = i; }
  void setMomentum(float px, float py, float pz, float e);
  void setStatus(int i){ mStatus = i; };

  int pdgID() const { return mPDGID; }
  int prodVertexIndex() const { return mProdVertexIndex; }
  int endVertexIndex() const { return mEndVertexIndex; }
  int status() const {return mStatus;}


  TLorentzVector v4() const;

  ClassDef(McParticle, 1)

protected:
  Int_t mPDGID;
  Float_t mPx;
  Float_t mPy;
  Float_t mPz;
  Float_t mE;
  Int_t mProdVertexIndex;
  Int_t mEndVertexIndex;
  Int_t mStatus;
};

#endif // __McParticle_hxx__
