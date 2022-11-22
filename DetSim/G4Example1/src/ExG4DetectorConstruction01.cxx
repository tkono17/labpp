/*
  ExG4DetectorConstruction01.cxx
*/
#include "G4Example1/ExG4DetectorConstruction01.hxx"
#include "G4Example1/ScintillatorSD.hxx"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"

using CLHEP::mm;
using CLHEP::cm;

namespace ds {

  ExG4DetectorConstruction01::ExG4DetectorConstruction01() : 
    G4VUserDetectorConstruction() {
    mUseAbsorber = false;
    mAbsorberMaterial = "";
    mAbsorberThickness = 0.0;
    mAbsorberZ = 0.0;

    mPV_World = 0;
    mPV_Sci0 = 0;
    mPV_Sci0 = 0;
    mPV_Sci0 = 0;
    mPV_Sci0 = 0;
    mPV_Cu = 0;
  }

  ExG4DetectorConstruction01::~ExG4DetectorConstruction01() {
  }

  G4VPhysicalVolume* ExG4DetectorConstruction01::Construct() {
    constructVolumes();
    constructSDs();

    return mPV_World;
  }

  void ExG4DetectorConstruction01::constructVolumes() {
    // Materials
    G4NistManager* man = G4NistManager::Instance();
    G4Material* Air = man->FindOrBuildMaterial("G4_AIR");
    G4Material* plastic = man->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

    G4Box* s_world = new G4Box("World", 500.0*mm, 500.0*mm, 500.0*mm);
    G4LogicalVolume* lv_world = new G4LogicalVolume(s_world, Air, "World");
    mPV_World = new G4PVPlacement(0, 
				  G4ThreeVector(), 
				  lv_world, 
				  "World", 
				  0, 
				  false, 0);

    G4Box* s_sci0 = new G4Box("Sci0", 30.0*mm, 50.0*mm, 5.0*mm);
    G4LogicalVolume* lv_sci0 = new G4LogicalVolume(s_sci0, plastic, "Sci0");
    mPV_Sci0 = new G4PVPlacement(0, 
				 G4ThreeVector(0, 0, 180.0*mm), 
				 lv_sci0, 
				 "Sci0", 
				 lv_world, 
				 false, 0);

    G4Box* s_sci1 = new G4Box("Sci1", 30.0*mm, 50.0*mm, 5.0*mm);
    G4LogicalVolume* lv_sci1 = new G4LogicalVolume(s_sci1, plastic, "Sci1");
    mPV_Sci1 = new G4PVPlacement(0, 
				 G4ThreeVector(0, 0, 150.0*mm), 
				 lv_sci1, 
				 "Sci1", 
				 lv_world, 
				 false, 0);

    G4Box* s_sci2 = new G4Box("Sci2", 30.0*mm, 50.0*mm, 40.0*mm);
    G4LogicalVolume* lv_sci2 = new G4LogicalVolume(s_sci2, plastic, "Sci2");
    mPV_Sci2 = new G4PVPlacement(0, 
				 G4ThreeVector(0, 0, 6.5*cm), 
				 lv_sci2, 
				 "Sci2", 
				 lv_world, 
				 false, 0);

    G4Box* s_sci3 = new G4Box("Sci3", 3.0*cm, 5.0*cm, 0.5*cm);
    G4LogicalVolume* lv_sci3 = new G4LogicalVolume(s_sci3, plastic, "Sci3");
    mPV_Sci3 = new G4PVPlacement(0, 
				 G4ThreeVector(0, 0, -3.0*cm), 
				 lv_sci3, 
				 "Sci3", 
				 lv_world, 
				 false, 0);

    if (mUseAbsorber) {
      std::string g4mat = "G4_"+mAbsorberMaterial;
      G4Material* am = man->FindOrBuildMaterial(g4mat.c_str());
      if (am) {
	G4Box* s_Cu = new G4Box("Absorber", 5.0*cm, 7.5*cm, mAbsorberThickness/2.0);
	G4LogicalVolume* lv_Cu = new G4LogicalVolume(s_Cu, am, "Absorber");
	mPV_Cu = new G4PVPlacement(0, 
				   G4ThreeVector(0, 0, mAbsorberZ), 
				   lv_Cu, 
				   "Absorber", 
				   lv_world, 
				   false, 0);
      }
    }
  }

  void ExG4DetectorConstruction01::constructSDs() {
    G4SDManager* sdMgr = G4SDManager::GetSDMpointer();

    int idet=0;
    ds::ScintillatorSD* sd=0;
    char sdname[100];
    G4VPhysicalVolume* dets[4] = { mPV_Sci0, mPV_Sci1, mPV_Sci2, mPV_Sci3};
    G4VPhysicalVolume* det=0;

    for (idet=0; idet<4; ++idet) {
      std::sprintf(sdname, "ScintillatorSD_%d", idet);
      sd = new ds::ScintillatorSD(sdname, idet);

      det = dets[idet];
      det->SetCopyNo(idet);
      det->GetLogicalVolume()->SetSensitiveDetector(sd);
      sdMgr->AddNewDetector(sd);
    }

  }
    
}
