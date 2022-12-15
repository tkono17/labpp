/*
  ds::ExG4PhysicsList01.cxx
*/
#include "G4Example1/ExG4PhysicsList01.hxx"
#include "G4LeptonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4HadronElasticProcess.hh"
#include "G4HadronElastic.hh"
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
#include "G4MuMinusCapturePrecompound.hh"
#include "G4MuonMinusCapture.hh"
#include "G4MuonMinusAtomicCapture.hh"
#include "G4MuonMinusBoundDecay.hh"
#include "G4MuonicAtomDecay.hh"
#include "G4DecayTable.hh"
#include "G4DecayWithSpin.hh"
#include "G4ProcessTable.hh"
#include "G4IonTable.hh"
#include "G4GenericIon.hh"
#include "G4GenericMuonicAtom.hh"
#include "G4NistManager.hh"

namespace ds {

  ExG4PhysicsList01::ExG4PhysicsList01() : 
    G4VUserPhysicsList() {
  }

  ExG4PhysicsList01::~ExG4PhysicsList01() {
  }

  void ExG4PhysicsList01::ConstructParticle() {
    std::cout << "Construct particles" << std::endl;
    G4LeptonConstructor c_lepton;
    G4MesonConstructor mesonConstructor;
    G4BaryonConstructor baryonConstructor;
    c_lepton.ConstructParticle();
    mesonConstructor.ConstructParticle();
    baryonConstructor.ConstructParticle();
    
    G4Gamma::Definition();
    G4Proton::Definition();
    G4Neutron::Definition();

    G4GenericIon::Definition();
    G4Deuteron::Definition();
    G4Triton::Definition();
    G4Alpha::Definition();
    G4He3::Definition();
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

    AddMuonicAtom();
    ModifyMuonDecay();
    Check();
  }

  void ExG4PhysicsList01::AddMuonicAtom() {
    auto gion = G4Proton::Definition();
    G4ParticleTable::GetParticleTable()->SetGenericIon(gion);
    G4ParticleTable::GetParticleTable()->SetGenericMuonicAtom(gion);

    const G4ElementTable* eTable = G4Element::GetElementTable();
    for (auto e: *eTable) {
      std::cout << "Element: " << e->GetName()
		<< "  N isotopes: " << e->GetNumberOfIsotopes()
		<< std::endl;
      auto isotopes = e->GetIsotopeVector();
      for (auto iso: *isotopes) {
	std::cout << "  Isotope: " << iso->GetName()
		  << " Z=" << iso->GetZ() << " A=" << iso->GetA()
		  << " N=" << iso->GetN()
		  << std::endl;
      }
    }

    std::vector<G4ParticleDefinition*> muAtoms;
    std::cout << "ion table" << std::endl;
    G4IonTable* ionTable = G4IonTable::GetIonTable();
    if (ionTable) {
      std::cout << "create ion" << std::endl;
      //      G4ParticleDefinition* ion = nullptr;
      muAtoms.push_back(ionTable->GetMuonicAtom(29, 63));
      muAtoms.push_back(ionTable->GetMuonicAtom(29, 65));
    } else {
      std::cout << "No IonTable" << std::endl;
    }
    
    G4ProcessManager* pManager = nullptr;
    G4ParticleDefinition* particle=nullptr;
    G4ParticleTable* ptable = G4ParticleTable::GetParticleTable();
    std::cout << "Add mu- process" << std::endl;
    if (ptable) {
      std::cout << "Add process for mu-" << std::endl;
      particle = ptable->FindParticle("mu-");
      pManager = particle->GetProcessManager();
    }
    if (pManager) {
      std::cout << "Add atomic capture" << std::endl;
      auto capture = new G4MuonMinusCapture();
      pManager->AddRestProcess(capture);
      pManager->SetProcessOrderingToFirst(capture, idxAtRest);
    }

    for (auto muAtom: muAtoms) {
      std::cout << "Mu atom: " << muAtom->GetParticleName() << std::endl;
      pManager = nullptr;
      if (muAtom) {
	std::cout << "MuonicAtom process manager" << std::endl;
	pManager = muAtom->GetProcessManager();
      }
      if (pManager) {
	std::cout << "Add MuonicAtom decay2" << std::endl;
	G4MuonicAtomDecay* decay = new G4MuonicAtomDecay();
	std::cout << "created decay" << std::endl;
	pManager->AddProcess(decay);
	//pManager->SetProcessOrdering(decay, idxPostStep);
	//pManager->SetProcessOrdering(decay, idxAtRest);
      }
    }
    std::cout << "Muonic atom done" << std::endl;
  }

  void ExG4PhysicsList01::ModifyMuonDecay() {
    std::cout << "Modify muon decay configuration" << std::endl;

    // G4DecayTable* mup_dt = new G4DecayTable();
    // mup_dt->Insert(new G4MuonDecayChannelWithSpin("mu+", 0.986));
    // mup_dt->Insert(new G4MuonRadiativeDecayChannelWithSpin("mu+", 0.014));
    // G4MuonPlus::MuonPlusDefinition()->SetDecayTable(mup_dt);

    // G4DecayTable* mum_dt = new G4DecayTable();
    // mum_dt->Insert(new G4MuonDecayChannelWithSpin("mu-", 0.986));
    // mum_dt->Insert(new G4MuonRadiativeDecayChannelWithSpin("mu-", 0.014));
    // G4MuonMinus::MuonMinusDefinition()->SetDecayTable(mum_dt);

    G4DecayWithSpin* decayWithSpin = new G4DecayWithSpin();
    G4ProcessTable* processTable = G4ProcessTable::GetProcessTable();
    G4VProcess* decay;
    G4ProcessManager* pManager;

    decay = processTable->FindProcess("Decay", G4MuonPlus::MuonPlus());
    std::cout << "Muondecay: " << decay << std::endl;
    pManager = G4MuonPlus::MuonPlus()->GetProcessManager();
    if (pManager) {
      if (decay) pManager->RemoveProcess(decay);
      pManager->AddProcess(decayWithSpin);
      pManager->SetProcessOrdering(decayWithSpin, idxAtRest);
      pManager->SetProcessOrdering(decayWithSpin, idxPostStep);
    }
    
    decay = processTable->FindProcess("Decay", G4MuonMinus::MuonMinus());
    pManager = G4MuonMinus::MuonMinus()->GetProcessManager();
    if (pManager) {
      if (decay) pManager->RemoveProcess(decay);
      pManager->AddProcess(decayWithSpin);
      pManager->SetProcessOrdering(decayWithSpin, idxAtRest);
      pManager->SetProcessOrdering(decayWithSpin, idxPostStep);
    }
    
  }

  void ExG4PhysicsList01::Check() {
    G4bool isElectron         = false;
    G4bool isPositron         = false;
    G4bool isGamma            = false;
    G4bool isProton           = false;
    G4bool isGenericIon       = false;
    G4bool isAnyIon           = false;
    G4bool isAnyChargedBaryon = false;
    G4bool isEmProc           = false;

    auto pTable = G4ParticleTable::GetParticleTable();
    for (int i=0; i<pTable->size(); ++i) {
      G4ParticleDefinition* particle = pTable->GetParticle(i);
      auto name = particle->GetParticleName();
      std::cout << "particle name: " << name << std::endl;

      if(name == "e-")
	isElectron = true;
      else if(name == "e+")
	isPositron = true;
      else if(name == "gamma")
	isGamma = true;
      else if(name == "GenericIon")
	isGenericIon = true;
      else if(name == "proton")
	isProton = true;
      else if(particle->GetParticleType() == "nucleus")
	isAnyIon = true;
      else if(particle->GetParticleType() == "baryon")
	{
	  if(particle->GetPDGCharge() != 0.0)
	    isAnyChargedBaryon = true;
	}

      auto pManager = particle->GetProcessManager();
      std::cout << "  process manager: " << pManager << std::endl;
      if (pManager) {
	std::cout << "  process manager is there" << std::endl;
      } else {
	continue;
      }
      G4ProcessVector* list = pManager->GetProcessList();
      std::cout << "  list=" << list << std::endl;
      if (list) {
	std::cout << "  list size: " << list->size() << std::endl;
      } else {
	continue;
      }
      for(std::size_t idx = 0; idx < list->size(); ++idx)
	{
	  std::cout << "  process name: "
		    << (*list)[idx]->GetProcessName() << std::endl;
	  if (!isEmProc) {
	    isEmProc = ((*list)[idx])->GetProcessType() == fElectromagnetic;
	  }
	}

      auto decayTable = particle->GetDecayTable();
      std::cout << "  decayTable: " << decayTable << std::endl;
      if (decayTable) {
	int ndecays = decayTable->entries();
	std::cout << "  N decay channels: " << ndecays
		  << " lifetime=" << particle->GetPDGLifeTime()
		  << std::endl;
	for (int idecay=0; idecay<ndecays; ++idecay) {
	  auto ch = decayTable->GetDecayChannel(idecay);
	  if (ch) {
	    std::cout << "  decay " << ch->GetKinematicsName()
		      << " BR=" << ch->GetBR()
		      << " daughters=" << ch->GetNumberOfDaughters()
		      << std::endl;
	  }
	}
      }
    }
    std::cout << "isElectron = " << isElectron << std::endl;
    std::cout << "isPositron = " << isPositron << std::endl;
    std::cout << "isGamma = " << isGamma << std::endl;
    std::cout << "isGenericIon = " << isGenericIon << std::endl;
    std::cout << "isProton = " << isProton << std::endl;
    std::cout << "isAnyIon = " << isAnyIon << std::endl;
    std::cout << "isAnyChargedBaryon = " << isAnyChargedBaryon << std::endl;
    std::cout << "isEmProc = " << isEmProc << std::endl;
  }
  
}
