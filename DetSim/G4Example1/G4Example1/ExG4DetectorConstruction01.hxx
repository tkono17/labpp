#ifndef __ds_ExG4DetectorConstruction01_hxx__
#define __ds_ExG4DetectorConstruction01_hxx__
/*
  ExG4DetectorConstruction01.hxx
*/
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"


namespace ds {

  class ScintillatorSD;
  
  class ExG4DetectorConstruction01 : public G4VUserDetectorConstruction {
  public:
    ExG4DetectorConstruction01();
    ~ExG4DetectorConstruction01();

    G4VPhysicalVolume* Construct();

    void setUseAbsorber(bool x) { mUseAbsorber = x; }
    void setAbsorberMaterial(const std::string& m) { mAbsorberMaterial = m; }
    void setAbsorberThickness(float x) { mAbsorberThickness = x; }
    void setAbsorberZ(float z) { mAbsorberZ = z; }

  protected:
    void constructVolumes();
    void constructSDs();

  protected:
    bool mUseAbsorber;
    std::string mAbsorberMaterial;
    float mAbsorberThickness;
    float mAbsorberZ;

    // Volumes
    G4VPhysicalVolume* mPV_World;
    G4VPhysicalVolume* mPV_Sci0;
    G4VPhysicalVolume* mPV_Sci1;
    G4VPhysicalVolume* mPV_Sci2;
    G4VPhysicalVolume* mPV_Sci3;
    G4VPhysicalVolume* mPV_Cu;

    // SensitiveDetectors
    ds::ScintillatorSD* mScintillatorSD;
  };

}

#endif // __DS_ExG4DetectorConstruction01_hxx__
