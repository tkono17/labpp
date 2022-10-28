#ifndef __ds_ExG4PrimaryGeneratorAction01_hxx__
#define __ds_ExG4PrimaryGeneratorAction01_hxx__
/*
  ds::ExG4PrimaryGeneratorAction01.hxx
*/
#include "G4VUserPrimaryGeneratorAction.hh"
#include "TRandom3.h"

class G4Event;
class G4ParticleGun;

namespace ds {

  class ExG4PrimaryGeneratorAction01 : public G4VUserPrimaryGeneratorAction {
  public:
    ExG4PrimaryGeneratorAction01();
    ~ExG4PrimaryGeneratorAction01();

    void GeneratePrimaries(G4Event* event);

  protected:
    G4ParticleGun* mParticleGun;
    TRandom3 mRandom;
  };

}

#endif // __ds_ExG4PrimaryGeneratorAction01_hxx__
