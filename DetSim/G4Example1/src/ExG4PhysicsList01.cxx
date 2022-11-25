/*
  ds::ExG4PhysicsList01.cxx
*/
#include "G4Example1/ExG4PhysicsList01.hxx"
#include "G4LeptonConstructor.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4PhysicsListHelper.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4MuonDecayChannel.hh"
#include "G4ParticleTable.hh"
#include "G4MuonDecayChannelWithSpin.hh"
#include "G4MuonRadiativeDecayChannelWithSpin.hh"
//#include "G4MuonMinusCaptureAtRest.hh"
#include "G4MuonMinusAtomicCapture.hh"
#include "G4MuonicAtomDecay.hh"
#include "G4DecayTable.hh"
#include "G4DecayWithSpin.hh"
#include "G4ProcessTable.hh"

namespace ds {

  ExG4PhysicsList01::ExG4PhysicsList01() : 
    G4VUserPhysicsList() {
  }

  ExG4PhysicsList01::~ExG4PhysicsList01() {
  }

  void ExG4PhysicsList01::ConstructParticle() {
    G4LeptonConstructor c_lepton;
    c_lepton.ConstructParticle();
    G4Electron::Definition();
    G4Positron::Definition();
    G4Gamma::Definition();
    G4Proton::ProtonDefinition();
    G4Neutron::NeutronDefinition();
  }

  void ExG4PhysicsList01::ConstructProcess() {
    AddTransportation();
    
    G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
    G4ParticleTable* ptable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle=0;

    std::cout << "Add process for gamma" << std::endl;    
    particle = ptable->FindParticle("gamma");
    ph->RegisterProcess(new G4PhotoElectricEffect(), particle);
    ph->RegisterProcess(new G4ComptonScattering(), particle);
    ph->RegisterProcess(new G4GammaConversion(), particle);
    
    std::cout << "Add process for electron" << std::endl;    
    particle = ptable->FindParticle("e-");
    ph->RegisterProcess(new G4eMultipleScattering(), particle);
    ph->RegisterProcess(new G4eIonisation(), particle);
    ph->RegisterProcess(new G4eBremsstrahlung(), particle);
    
    std::cout << "Add process for positron" << std::endl;    
    particle = ptable->FindParticle("e+");
    ph->RegisterProcess(new G4eMultipleScattering(), particle);
    ph->RegisterProcess(new G4eIonisation(), particle);
    ph->RegisterProcess(new G4eBremsstrahlung(), particle);
    ph->RegisterProcess(new G4eplusAnnihilation(), particle);

    particle = ptable->FindParticle("mu-");
    ph->RegisterProcess(new G4MuMultipleScattering(), particle);
    ph->RegisterProcess(new G4MuIonisation(), particle);
    ph->RegisterProcess(new G4MuBremsstrahlung(), particle);
    ph->RegisterProcess(new G4MuPairProduction(), particle);
    //    ph->RegisterProcess(new G4MuonDecayChannel(), particle);

    particle = ptable->FindParticle("mu+");
    ph->RegisterProcess(new G4MuMultipleScattering(), particle);
    ph->RegisterProcess(new G4MuIonisation(), particle);
    ph->RegisterProcess(new G4MuBremsstrahlung(), particle);
    ph->RegisterProcess(new G4MuPairProduction(), particle);
    //    ph->RegisterProcess(new G4MuonDecayChannel(), particle);

    ModifyMuonDecay();
  }

  void ExG4PhysicsList01::ModifyMuonDecay() {
    std::cout << "Modify muon decay configuration" << std::endl;
    G4DecayTable* mup_dt = new G4DecayTable();
    mup_dt->Insert(new G4MuonDecayChannelWithSpin("mu+", 0.986));
    mup_dt->Insert(new G4MuonRadiativeDecayChannelWithSpin("mu+", 0.014));
    G4MuonPlus::MuonPlusDefinition()->SetDecayTable(mup_dt);

    G4DecayTable* mum_dt = new G4DecayTable();
    mum_dt->Insert(new G4MuonDecayChannelWithSpin("mu-", 0.986));
    mum_dt->Insert(new G4MuonRadiativeDecayChannelWithSpin("mu-", 0.014));
    G4MuonMinus::MuonMinusDefinition()->SetDecayTable(mum_dt);

    G4DecayWithSpin* decayWithSpin = new G4DecayWithSpin();
    G4ProcessTable* processTable = G4ProcessTable::GetProcessTable();
    G4VProcess* decay;
    G4ProcessManager* pManager;

    decay = processTable->FindProcess("Decay", G4MuonPlus::MuonPlus());
    pManager = G4MuonPlus::MuonPlus()->GetProcessManager();
    if (pManager) {
      if (decay) pManager->RemoveProcess(decay);
      pManager->AddProcess(decayWithSpin);
      pManager->SetProcessOrdering(decayWithSpin, idxPostStep);
      pManager->SetProcessOrdering(decayWithSpin, idxAtRest);
    }
    
    decay = processTable->FindProcess("Decay", G4MuonMinus::MuonMinus());
    pManager = G4MuonMinus::MuonMinus()->GetProcessManager();
    G4VProcess* capture_process = new G4MuonMinusAtomicCapture();
    if (pManager) {
      if (decay) pManager->RemoveProcess(decay);
      pManager->AddProcess(decayWithSpin);
      pManager->SetProcessOrdering(decayWithSpin, idxPostStep);
      pManager->SetProcessOrdering(decayWithSpin, idxAtRest);
      pManager->AddProcess(capture_process);
    }

    // pManager = G4MuonicAtom::MuonicAtom()->GetProcessManager();
    // decay = new G4MuonicAtomDecay();
    // if (pManager) {
    //   pManager->AddProcess(decay);
    // }
    
  }

}
