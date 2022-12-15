#ifndef __ds_CosmicEventAction_hxx__
#define __ds_CosmicEventAction_hxx__
/*
  ds::CosmicEventAction.hxx
*/
#include "TH1.h"
#include "TFile.h"
#include "TTree.h"

#include "G4UserEventAction.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VTrajectory.hh"

#include "SimData/SimEvent.hxx"

namespace ds {

  class CosmicEventAction : public G4UserEventAction {
  public:
    CosmicEventAction();
    ~CosmicEventAction();

    void setupTree();

    void BeginOfEventAction(const G4Event* event);
    void EndOfEventAction(const G4Event* event);

    void setOutFileName(const std::string& s) { mOutFileName = s; }

    void checkParticles(G4TrajectoryContainer& tc);
    
  protected:
    G4EventManager* mEventManager;

    ds::SimEvent mEvent;
    std::string mOutFileName;
    TFile* mOutFile;
    TTree* mTree;
    TH1* mHist_nhits;
  };

}

#endif // __ds_CosmicEventAction_hxx__
