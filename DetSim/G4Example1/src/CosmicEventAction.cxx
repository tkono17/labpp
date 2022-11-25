/*
  ds::CosmicEventAction.cxx
*/
#include <iostream>
#include "G4Example1/CosmicEventAction.hxx"
#include "G4Example1/SCHit.hxx"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4Event.hh"

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
    mTree->Fill();
    mEvent.clear();
  }

}

