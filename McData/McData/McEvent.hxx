#ifndef __McEvent_hxx__
#define __McEvent_hxx__
/*
  McEvent.hxx
*/
#include "TObject.h"

class McEvent : public TObject {
public:
  McEvent();
  ~McEvent();

  void setSignalProcessID(int process_id) { mSignalProcessID = process_id; }
  void setEventNumber(int n) { mEventNumber = n; }
  void setWeights(float w1=1.0, float w2=1.0, float w3=1.0);
  void setPDFInfo(int i, int pdfset, int partonid, float x, float value);
  void setPDFscale(float q) { mPDFscale = q; }

  ClassDef(McEvent, 1)

protected:
  Int_t mSignalProcessID;
  Int_t mEventNumber;
  Float_t mWeights[3];
  /** PDF information */
  Int_t mPDFset1;
  Int_t mPDFset2;
  Int_t mPDFparton1;
  Int_t mPDFparton2;
  Float_t mPDFx1;
  Float_t mPDFx2;
  Float_t mPDFscale;
  Float_t mPDFvalue1;
  Float_t mPDFvalue2;
  
};

#endif // __McEvent_hxx__
