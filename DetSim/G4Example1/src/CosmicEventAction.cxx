/*
  ds::CosmicEventAction.cxx
*/
#include <iostream>
#include "G4Example1/CosmicEventAction.hxx"
#include "G4Example1/SCHit.hxx"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VTrajectory.hh"

namespace ds {

  CosmicEventAction::CosmicEventAction() : G4UserEventAction() {
    mEventManager = 0;
    mOutFileName = "out.root";
    mOutFile = 0;
    mHist_nhits = 0;
  }

  CosmicEventAction::~CosmicEventAction() {
    if (mOutFile) {
      mOutFile->Write();
      mOutFile->Close();
      delete mOutFile;
    }
  }

  void CosmicEventAction::setupTree() {
    mOutFile = TFile::Open(mOutFileName.c_str(), "RECREATE");
    mHist_nhits = new TH1F("h_nhits", "", 1000, 0, 1000);

    mTree = new TTree("t", "Muon decay measurement");
    mEvent.initialize();
    mEvent.createBranches(mTree);
  }
  
  void CosmicEventAction::BeginOfEventAction(const G4Event* /*event*/) {
  }

  void CosmicEventAction::EndOfEventAction(const G4Event* event) {
    char cname[100];

    G4SDManager* sdmgr = G4SDManager::GetSDMpointer();

    G4HCofThisEvent* event_hc = event->GetHCofThisEvent();

    for (int idet=0; idet<4; ++idet) {
      std::sprintf(cname, "SCHitCollection_%d", idet);
      G4int cid = sdmgr->GetCollectionID(cname);
      ds::SCHitsCollection* hc = 
	dynamic_cast<ds::SCHitsCollection*>(event_hc->GetHC(cid));
      if (hc) {
	int nhits = hc->GetSize();
	for (int ihit=0; ihit<nhits; ++ihit) {
	  ds::SCHit* hit = (*hc)[ihit];
	  if (hit) {
	    mEvent.addHit(idet, hit->hitData());
	  }
	}

      } else {
	std::cout << "Cannot find HitsCollection" << std::endl;
      }
    }

    auto trajectories = event->GetTrajectoryContainer();
    //    std::cout << "Trajectories: " << trajectories << std::endl;
    if (trajectories) {
      checkParticles(*trajectories);
    }
    
    mTree->Fill();
    mEvent.clear();
  }

  void CosmicEventAction::checkParticles(G4TrajectoryContainer& tc) {
    int nparticles = tc.entries();
    std::cout << "N trajectories: " << nparticles << std::endl;

    for (int i=0; i<nparticles; ++i) {
      auto trajectory = tc[i];
      if (trajectory) {
	//	auto point = trajectory->GetPoint();
	std::cout << "  particle=" << trajectory->GetParticleName()
		  << std::endl;
      }
    }
  }

}

