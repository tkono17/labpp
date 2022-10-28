/*
  ds::ExG4PrimaryGeneratorAction01.cxx
*/
#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleTable.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "G4Example1/ExG4PrimaryGeneratorAction01.hxx"
#include "SimData/SimEvent.hxx"

#include "TMath.h"
#include "TH1.h"

#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using CLHEP::mm;
using CLHEP::cm;
using CLHEP::GeV;

namespace ds {

  ExG4PrimaryGeneratorAction01::ExG4PrimaryGeneratorAction01() : 
    G4VUserPrimaryGeneratorAction(), mParticleGun(0) {
    G4int nparticles = 1;
    mParticleGun = new G4ParticleGun(nparticles);
    mRandom.SetSeed(3457886);

  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  ExG4PrimaryGeneratorAction01::~ExG4PrimaryGeneratorAction01() {

    if (mParticleGun) {
      delete mParticleGun;
      mParticleGun = 0;
    }

  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void ExG4PrimaryGeneratorAction01::GeneratePrimaries(G4Event* event) {
    G4ParticleTable* ptable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* mu_minus = ptable->FindParticle("mu-");
    G4ParticleDefinition* mu_plus = ptable->FindParticle("mu+");
    Double_t mu = mRandom.Uniform(0.0, 1.0);
    G4ParticleDefinition* p = 0; 
    float track_charge=0.0;
    if(mu>0.5) {
      p = mu_minus;
      track_charge = -1;
    } else {
      p = mu_plus;
      track_charge = 1;
    }

    Double_t a = mRandom.Uniform(0.01, 10.0);
    Double_t b = log10(a);
    G4double energy = std::pow(10.0, b)*GeV;
    Double_t x = mRandom.Uniform(-40.0, 40.0);
    float y = mRandom.Uniform(-50.0, 50.0);
    float z = 50*cm;

    Double_t pi = TMath::Pi();
    Double_t c =  mRandom.Uniform(0.0, pi/6.);
    Double_t d = mRandom.Uniform(0.0, 1.);
    Double_t theta = 0.0;
    if (d<cos(c)*cos(c)) theta = c;
    G4ThreeVector pos(x, y, z);
    Double_t phi = mRandom.Uniform(0.0,2.0*pi);
    Double_t px = -sin(theta)*cos(phi);
    Double_t py = -sin(theta)*sin(phi);
    Double_t pz = -cos(theta);
    G4ThreeVector mom(px, py, pz);
    mParticleGun->SetParticleDefinition(p);
    mParticleGun->SetParticleEnergy(energy);
    mParticleGun->SetParticlePosition(pos);
    mParticleGun->SetParticleMomentumDirection(mom);
    mParticleGun->GeneratePrimaryVertex(event);
    TLorentzVector track_pos(pos.x(), pos.y(), pos.z(), 0.0);
    TLorentzVector track_mom;
    track_mom.SetPtEtaPhiE(mom.perp(), mom.eta(), mom.phi(), energy);
    
    ds::SimEvent* sevent = ds::SimEvent::get();
    //std::cout <<"track_charge pppp"<<std::endl;
    if (sevent) {
      sevent->setTrackCharge(track_charge);
      sevent->setTrackPosition(track_pos);
      sevent->setTrackMomentum(track_mom);
      // std::cout <<"track_charge "<< track_charge << ", " 
      // 		<< sevent->trackCharge() << std::endl;
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
