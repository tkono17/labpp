#ifndef __LargeRJet_hxx__
#define __LargeRJet_hxx__
/*
  LargeRJet.hxx
*/
#include "Jet.hxx"

class LargeRJet : public Jet {
public:
  LargeRJet();
  ~LargeRJet();

  void setD2(float d2) { mD2 = d2; }
  float d2() const { return mD2; }

  ClassDef(LargeRJet, 1)

protected:
  Float_t mD2;
};

#endif // __LargeRJet_hxx__
