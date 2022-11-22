#ifndef __ds_SimEvent_hxx__
#define __ds_SimEvent_hxx__
/*
  ds::SimEvent.hxx
*/
#include <iostream>
#include <vector>

#include "TTree.h"
#include "TLorentzVector.h"
#include "SimData/SCHitData.hxx"

namespace ds {

  class SCHitData;

  class SimEvent {// : public TObject {
  public:
    static SimEvent* get();
    
  private:
    static SimEvent* sEvent;

  public:
    SimEvent();
    ~SimEvent();

    void setTrackCharge(float q) { mTrackCharge = q; }
    void setTrackPosition(const TLorentzVector& v);
    void setTrackMomentum(const TLorentzVector& v);
    ds::SCHitData* addHit(int idet, const ds::SCHitData& hit);

    float trackCharge() const { return mTrackCharge; }
    const TLorentzVector& trackPosition() const { return *mTrackPosition; }
    const TLorentzVector& trackMomentum() const { return *mTrackMomentum; }
    int Nhits(int idet) const;
    ds::SCHitData getHit(int idet, int ihit) const;

    void initialize();

    void createBranches(TTree* t);
 
    void setBranchAddress(TTree *t);

    void clear();

    //    ClassDef(ds::SimEvent, 2)

  protected:
    float mTrackCharge;
    TLorentzVector* mTrackPosition;
    TLorentzVector* mTrackMomentum;
    //    TClonesArray* mSCHits[4];
    std::vector<SCHitData> mSCHits[4];
  };

}

#endif // __ds_SimEvent_hxx__
