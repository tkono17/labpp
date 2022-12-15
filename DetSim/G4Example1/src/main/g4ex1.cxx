/*
  g4ex1.cxx
*/
#include <iostream>
#include "G4Example1/ExG4DetectorConstruction01.hxx"
#include "G4Example1/ExG4PhysicsList01.hxx"
#include "G4Example1/ExG4PrimaryGeneratorAction01.hxx"
#include "G4Example1/CosmicEventAction.hxx"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "cstdlib"

using CLHEP::mm;
using CLHEP::cm;
using CLHEP::GeV;

int main(int argc, char* argv[]) {
  std::cout << "G4 example" << std::endl;

  int n = 1;
  std::string mOutFileName="out.root";
  std::string mDetAbsorberMaterial = "";
  float mDetAbsorberThickness = 0.0;
  float mDetAbsorberZ = 0.0;

  /**
     Usage: g4ex1.exe [n] [outfilename] [AbsorberMaterial] [AbsorberThickness(mm)] [AbsorberZ(mm)]
  */
  if (argc == 1) {
    std::cout << "Usage: g4ex1.exe [n] [outfilename] [AbsorberMaterial] [AbsorberThickness(mm)] [AbsorberZ(mm)]" << std::endl;
    exit(1);
  }
  if (argc>1){
    n=std::atoi(argv[1]);
  }
  if (argc>2) mOutFileName = argv[2];
  if (argc>3) mDetAbsorberMaterial = argv[3];
  if (argc>4) mDetAbsorberThickness = std::atof(argv[4]);
  if (argc>5) mDetAbsorberZ = std::atof(argv[5]);

  std::printf("g4ex1 arguments¥\n");
  std::printf("  Number of events: %d\n", n);
  std::printf("  Output filename: %s\n", mOutFileName.c_str());
  std::printf("  DetAbsorberMaterial: %s\n", mDetAbsorberMaterial.c_str());
  std::printf("  DetAbsorberThickness: %10.5f (mm)\n", mDetAbsorberThickness);
  std::printf("  DetAbsorberZ: %10.5f (mm)\n", mDetAbsorberZ);

  ds::ExG4PhysicsList01* physics_list = new ds::ExG4PhysicsList01();
  ds::ExG4DetectorConstruction01* detector_setup = 
    new ds::ExG4DetectorConstruction01();

  std::cout << "cm = " << cm << std::endl;
  std::cout << "mm = " << mm << std::endl;

  if (mDetAbsorberMaterial != "") {
    detector_setup->setUseAbsorber(true);
    detector_setup->setAbsorberMaterial(mDetAbsorberMaterial);
    detector_setup->setAbsorberThickness(mDetAbsorberThickness);
    detector_setup->setAbsorberZ(mDetAbsorberZ);
  } else {
    detector_setup->setUseAbsorber(false);
  }

  std::cout << "Set user initializations" << std::endl;
  G4RunManager* runMgr = new G4RunManager();
  runMgr->SetUserInitialization(detector_setup);
  runMgr->SetUserInitialization(physics_list);

  std::cout << "Set PrimaryGenratorAction" << std::endl;
  ds::ExG4PrimaryGeneratorAction01* action = 
    new ds::ExG4PrimaryGeneratorAction01();
  runMgr->SetUserAction(action);

  std::cout << "Set CosmicEventAction" << std::endl;
  ds::CosmicEventAction* event_action = new ds::CosmicEventAction();
  if (mOutFileName != "") {
    event_action->setOutFileName(mOutFileName);
  }
  event_action->setupTree();
  runMgr->SetUserAction(event_action);

  std::cout << "Initialize RunManager" << std::endl;
  runMgr->Initialize();

  std::cout << "UI manager" << std::endl;
  G4UImanager* ui = G4UImanager::GetUIpointer();
  ui->ApplyCommand("/run/verbose 1");
  //  ui->ApplyCommand("/event/verbose 1");
  //  ui->ApplyCommand("/tracking/verbose 1");

  std::cout << "BeamOn" << std::endl;
  int nevents=n;
  runMgr->BeamOn(nevents);

  if (runMgr) {
    delete runMgr;
    runMgr = 0;
  }

  return 0;
}
