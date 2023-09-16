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

  void setE1(double e1) { mE1 = e1; }
  double e1() const { return mE1; }

  void setE2(double e2) { mE2 = e2; }
  double e2() const { return mE2; }

  void setE3(double e3) { mE3 = e3; }
  double e3() const { return mE3; }

  void setD2(double d2) { mD2 = d2; }
  double d2() const { return mD2; }

  ClassDef(LargeRJet, 1)

protected:
  Double_t mE1;
  Double_t mE2;
  Double_t mE3;
  Double_t mD2;
};

#endif // __LargeRJet_hxx__
