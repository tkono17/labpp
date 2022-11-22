/*
  ds::SCHit.cxx
*/
#define __G4Allocator_ds_SCHit__defined__
#include "G4Example1/SCHit.hxx"

namespace ds {
  
  G4Allocator<ds::SCHit> SCHitAllocator;

  SCHit::SCHit() : G4VHit() {
  }

  SCHit::~SCHit() {
  }

  const SCHit& SCHit::operator=(const SCHit& x) {
    mHitData = x.hitData();
    return *this;
  }

  G4int SCHit::operator==(const SCHit& x) const {
    return (this == &x) ? 1 : 0;
  }

  void* SCHit::operator new(size_t) {
    SCHit* x = ds::SCHitAllocator.MallocSingle();
    return x;
  }

  void SCHit::operator delete(void* addr) {
    ds::SCHit* x = reinterpret_cast<SCHit*>(addr);
    if (x) {
      ds::SCHitAllocator.FreeSingle(x);
    }
  }

  void SCHit::Draw() {
  }

  void SCHit::Print() {
  }

}
