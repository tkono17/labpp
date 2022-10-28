/*
  ds::SimEvent.cxx
*/
#include <cstdio>
#include <iostream>
#include "TDirectory.h"
#include "SimData/SimEvent.hxx"
#include "SimData/SCHitData.hxx"

//ClassImp(ds::SimEvent)

namespace ds {

  SimEvent* SimEvent::sEvent = 0;

  SimEvent* SimEvent::get() {
    return sEvent;
  }

  SimEvent::SimEvent() {//: TObject() {
    mTrackCharge = 0;
    mTrackPosition = 0;
    mTrackMomentum = 0;
    // for (int i=0; i<4; ++i) {
    //   mSCHits[i] = 0;
    // }
    if (sEvent == 0) sEvent = this;
  }

  SimEvent::~SimEvent() {
    if (mTrackPosition) {
      delete mTrackPosition;
      mTrackPosition = 0;
    }
    if (mTrackMomentum) {
      delete mTrackMomentum;
      mTrackMomentum = 0;
    }
    for (int i=0; i<4; ++i) {
      mSCHits[i].clear();
      // delete mSCHits[i];
      // mSCHits[i] = 0;
    }
    if (sEvent == this) {
      sEvent = 0;
    }
  }

  void SimEvent::setTrackPosition(const TLorentzVector& v) {
    if (mTrackPosition) {
      *mTrackPosition = v;
    }
  }

  void SimEvent::setTrackMomentum(const TLorentzVector& v) {
    if (mTrackMomentum) {
      *mTrackMomentum = v;
    }
  }

  ds::SCHitData* SimEvent::addHit(int idet, const ds::SCHitData& hit) {
    ds::SCHitData* x=0;
    if (idet >=0 & idet < 4) {
      mSCHits[idet].push_back(hit);
      // TClonesArray* ca = mSCHits[idet];
      // int n = ca->GetEntries();
      // x = new ( (*ca)[n]) ds::SCHitData(hit);
    }
    return nullptr;
    //    return x;
  }

  int SimEvent::Nhits(int idet) const {
    int n=0;
    if (idet >= 0 && idet < 4) {
      //      n = mSCHits[idet]->GetEntries();
      n = mSCHits[idet].size();
    }
    return n;
  }

  ds::SCHitData SimEvent::getHit(int idet, int ihit) const {
    ds::SCHitData x;
    if (idet >= 0 && idet < 4) {
      // TObject* obj = mSCHits[idet]->At(ihit);
      // x = dynamic_cast<const ds::SCHitData*>(obj);
      x = mSCHits[idet][ihit];
    }
    return x;
  }

  void SimEvent::initialize() {
    mTrackPosition = new TLorentzVector();
    mTrackMomentum = new TLorentzVector();
    for (int i=0; i<4; ++i) {
      //      mSCHits[i] = new TClonesArray("ds::SCHitData", 100);
    }
  }
  
  void SimEvent::createBranches(TTree* t) {
    char bname[100];
    t->Branch("track_charge", &mTrackCharge, "track_charge/F");
    t->Branch("track_pos", "TLorentzVector", &mTrackPosition);
    t->Branch("track_mom", "TLorentzVector", &mTrackMomentum);

    //    std::cout << mTrackCharge << std::endl;
    for (int i=0; i<4; ++i) {
      std::sprintf(bname, "schits%d", i);
      //      t->Branch(bname, "TClonesArray", &mSCHits[i]);
    }
  }
  
  void SimEvent::setBranchAddress(TTree *t) {
    char bname[100];

    t->SetBranchAddress("track_pos", &mTrackPosition);
    t->SetBranchAddress("track_mom", &mTrackMomentum);
    t->SetBranchAddress("track_charge", &mTrackCharge);
    t->Print();

    std::cout << "TreeName" << t->GetName() << std::endl;
    std::cout << "FileName" << t->GetDirectory()->GetName() << std::endl;
    std::cout << mTrackCharge << std::endl;

    for (int i=0; i<4; ++i) {
      std::sprintf(bname, "schits%d", i);
      //      t->SetBranchAddress(bname, &mSCHits[i]);
    }
  }

  void SimEvent::clear() {
    mTrackPosition->Clear();
    mTrackMomentum->Clear();
    mTrackCharge++;
    for (int i=0; i<4; ++i) {
      mSCHits[i].clear();
    }
  }  
}

