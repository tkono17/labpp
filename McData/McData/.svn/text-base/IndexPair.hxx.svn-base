#ifndef __IndexPair_hxx__
#define __IndexPair_hxx__
/*
  IndexPair.hxx
*/
#include "TObject.h"

class IndexPair : public TObject {
public:
  IndexPair(int i1=0, int i2=0);
  ~IndexPair();

  void set(int i1, int i2);
  void setFirst(int i) { mFirst = i; }
  void setSecond(int i) { mSecond = i; }

  int first() const { return mFirst; }
  int second() const { return mSecond; }

  ClassDef(IndexPair, 1)

private:
  int mFirst;
  int mSecond;
};

#endif // __IndexPair_hxx__
