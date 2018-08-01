#ifndef __Jet_hxx__
#define __Jet_hxx__
/*
  Jet.hxx
*/
#include "TObject.h"
#include "TLorentzVector.h"

class Jet : public TObject {
public:
  Jet();
  ~Jet();

  void setPdgId(int id) { mPdgId = id; }
  void setStatus(int x) { mStatus = x; }
  void setMomentum(const TLorentzVector& v);
  void setNConstituents(int n) { mNConstituents = n; }
  void setFlavourTag(int n) { mFlavourTag = n; }
  
  int pdgId() const { return mPdgId; }
  int status() const { return mStatus; }
  int NConstituents() const { return mNConstituents; }
  int flavourTag() const { return mFlavourTag; }

  TLorentzVector v4() const;

  ClassDef(Jet, 1)

protected:
  Int_t mPdgId;
  Int_t mStatus;
  Float_t mP[4];
  Int_t mNConstituents;
  Int_t mFlavourTag; // =4(5) if the jet is c-tagged(b-tagged)

};

#endif // __Jet_hxx__
