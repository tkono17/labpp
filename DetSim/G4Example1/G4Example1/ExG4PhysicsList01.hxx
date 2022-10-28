#ifndef __ds_ExG4PhysicsList01_hxx__
#define __ds_ExG4PhysicsList01_hxx__
/*
  ds::ExG4PhysicsList01.hxx
*/
#include "G4VUserPhysicsList.hh"

namespace ds {

  class ExG4PhysicsList01 : public G4VUserPhysicsList {
  public:
    ExG4PhysicsList01();
    ~ExG4PhysicsList01();

    void ConstructParticle();

    void ConstructProcess();

    void ModifyMuonDecay();
  };

}

#endif // __ds_ExG4PhysicsList01_hxx__
