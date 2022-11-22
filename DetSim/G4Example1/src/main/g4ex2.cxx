/*
  g4ex2.cxx
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
  std::cout << "G4 example: check muon decay" << std::endl;

  int n = 1;
  if (argc>1){
    n=std::atoi(argv[1]);
  }
  
  ds::ExG4PhysicsList01* physics_list = new ds::ExG4PhysicsList01();
  ds::ExG4DetectorConstruction01* detector_setup = 
    new ds::ExG4DetectorConstruction01();

  G4RunManager* runMgr = new G4RunManager();
  runMgr->SetUserInitialization(detector_setup);
  runMgr->SetUserInitialization(physics_list);

  ds::ExG4PrimaryGeneratorAction01* action = 
    new ds::ExG4PrimaryGeneratorAction01();
  runMgr->SetUserAction(action);

  ds::CosmicEventAction* event_action = new ds::CosmicEventAction();
  runMgr->SetUserAction(event_action);

  runMgr->Initialize();

  G4UImanager* ui = G4UImanager::GetUIpointer();
  ui->ApplyCommand("/run/verbose 1");
  //  ui->ApplyCommand("/event/verbose 1");
  //  ui->ApplyCommand("/tracking/verbose 1");

  int nevents=n;
  runMgr->BeamOn(nevents);

  if (runMgr) {
    delete runMgr;
    runMgr = 0;
  }

  return 0;
}
