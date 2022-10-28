#ifndef __ds_ScintillatorSD_hxx__
#define __ds_ScintillatorSD_hxx__
/*
  ds::ScintillatorSD.hxx
*/
#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4String.hh"
#include "G4HCofThisEvent.hh"
#include "G4Example1/ExG4DetectorConstruction01.hxx"
#include "G4Example1/SCHit.hxx"

namespace ds {

  class ScintillatorSD : public G4VSensitiveDetector {
  public:
    ScintillatorSD(G4String name, int eet_number);
    ~ScintillatorSD();

    void Initialize(G4HCofThisEvent* hce);

    G4bool ProcessHits(G4Step* step, G4TouchableHistory* hist);

    void EndOfEvent(G4HCofThisEvent* hce);

  protected:
    int mDetNumber;
    int mHCID;
    ds::SCHitsCollection* mHitsCollection;
  };

}

#endif // __ds_ScintillatorSD_hxx__
