#ifndef __ds_MuDetDetectorConstruction_hxx__
#define __ds_MuDetDetectorConstruction_hxx__
/*
  ds::MuDetDetectorConstruction.hxx
*/
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"

namespace ds {

  class MuDetDetectorConstruction : G4VUserDetectorConstruction {
  public:
    MuDetDetectorConstruction();
    ~MuDetDetectorConstruction();

    G4VPhysicalVolume* Construct();
    G4int ConstructSDandField();

  protected:
    G4VPhysicalVolume* mPV_World;
    G4VPhysicalVolume* mPV_Sci0;
    G4VPhysicalVolume* mPV_Sci1;
    G4VPhysicalVolume* mPV_Sci2;
    G4VPhysicalVolume* mPV_Sci3;
    //G4VPhysicalVolume* mPV_Cu;
  };

}

#endif // __ds_MuDetDetectorConstruction_hxx__
