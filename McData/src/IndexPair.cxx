/*
  IndexPair.cxx
*/
#include "McData/IndexPair.hxx"

ClassImp(IndexPair)

IndexPair::IndexPair(int i1, int i2) : TObject() {
  mFirst = i1;
  mSecond = i2;
}

IndexPair::~IndexPair() {
}

void IndexPair::set(int i1, int i2) {
  setFirst(i1);
  setSecond(i2);
}



