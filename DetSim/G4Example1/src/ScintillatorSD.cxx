/*
  ds::ScintillatorSD.cxx
*/#
#include "G4Example1/ScintillatorSD.hxx"
#include "G4Example1/SCHit.hxx"
#include "G4SDManager.hh"
#include "G4StepPoint.hh"
#include "G4ThreeVector.hh"

namespace ds {

  ScintillatorSD::ScintillatorSD(G4String name, int det_number) : 
    G4VSensitiveDetector(name) {
    mDetNumber = det_number;
    mHCID = -1;
    mHitsCollection = 0;
    char cname[100];
    sprintf(cname, "SCHitCollection_%d", det_number);
    collectionName.insert(cname);
  }

  ScintillatorSD::~ScintillatorSD() {
  }
  
  void ScintillatorSD::Initialize(G4HCofThisEvent* /*hce*/) {
    mHitsCollection = new ds::SCHitsCollection(SensitiveDetectorName, 
					       collectionName[0]);
  }

  G4bool ScintillatorSD::ProcessHits(G4Step* step, G4TouchableHistory* /*hist*/) {
    G4double edep = step->GetTotalEnergyDeposit();
    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    double global_time = preStepPoint->GetGlobalTime();
    const G4ThreeVector& v3 = preStepPoint->GetPosition();

    ds::SCHit* hit = new ds::SCHit();
    ds::SCHitData& hitdata = hit->hitData();
    hitdata.setTime(global_time);
    hitdata.setPosition(v3.x(), v3.y(), v3.z());
    hitdata.setEnergyDeposit(edep);

    const G4Track* track = step->GetTrack();
    if (track) {
      auto pdef = track->GetParticleDefinition();
      if (pdef) {
	auto pname = pdef->GetParticleName();
	hitdata.setParticleName(pname);
      }
    }
    //    auto secTracks = step->GetSecondaries();
    
    mHitsCollection->insert(hit);

    return true;
  }
  
  void ScintillatorSD::EndOfEvent(G4HCofThisEvent* hce) {
    // mHCID = -1;
    // if (mHCID < 0) {
    G4SDManager* sdmgr = G4SDManager::GetSDMpointer();
    mHCID = sdmgr->GetCollectionID(mHitsCollection);
    hce->AddHitsCollection(mHCID, mHitsCollection);
    // }
  }

}
