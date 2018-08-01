/*
  McEvent.cxx
*/
#include "McData/McEvent.hxx"

ClassImp(McEvent)

using namespace std;

McEvent::McEvent() : TObject() {
}

McEvent::~McEvent() {
}

void McEvent::setWeights(float w1, float w2, float w3) {
  mWeights[0] = w1;
  mWeights[0] = w2;
  mWeights[0] = w3;
}

void McEvent::setPDFInfo(int i, int pdfset, int partonid, float x, 
			 float value) {
  if (i == 0) {
    mPDFset1 = pdfset;
    mPDFparton1 = partonid;
    mPDFx1 = x;
    mPDFvalue1 = value;
  } else if (i == 1) {
    mPDFset2 = pdfset;
    mPDFparton2 = partonid;
    mPDFx2 = x;
    mPDFvalue2 = value;
  }
}
