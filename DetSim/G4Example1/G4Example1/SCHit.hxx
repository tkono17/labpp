#ifndef __ds_SCHit_hxx__
#define __ds_SCHit_hxx__
/*
  ds::SCHit.hxx
*/
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

#include "SimData/SCHitData.hxx"

namespace ds {

  class SCHit : public G4VHit {
  public:
    SCHit();
    ~SCHit();

    const SCHit& operator=(const SCHit& x);
    G4int operator==(const SCHit& x) const;

    void* operator new(size_t);
    void operator delete(void* addr);

    SCHitData& hitData() { return mHitData; }
    const SCHitData& hitData() const { return mHitData; }

    void Draw();
    void Print();

 protected:
    SCHitData mHitData;

  };

  typedef G4THitsCollection<ds::SCHit> SCHitsCollection;
#ifndef __G4Allocator_ds_SCHit__defined__
  extern G4Allocator<ds::SCHit> SCHitAllocator;
#endif // __G4Allocator_ds_SCHit__defined__

}

#endif // __ds_SCHit_hxx__
