/*
  test_simdata1.cxx
*/
#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"

#include "SimData/SimParticle.hxx"
#include "SimData/SimVertex.hxx"


int main(int argc, char* argv[]) {
  std::cout << "Start test_simdata1.exe" << std::endl;

  Int_t n_particles;
  Int_t n_vertices;
  auto v_particles = new std::vector<ds::SimParticle>();
  auto v_vertices = new std::vector<ds::SimVertex>();

  TFile* f = TFile::Open("simdata1.root", "RECREATE");
  TTree* t = new TTree("t", "");
  t->Branch("n_particles", &n_particles, "n_particles/I");
  t->Branch("particles.", "std::vector<ds::SimParticle>", &v_particles);
  t->Branch("n_vertices", &n_vertices, "n_vertices/I");
  t->Branch("vertices.", "std::vector<ds::SimVertex>", &v_vertices);

  ds::SimParticle* particle=0;
  for (int i=0; i<10; i++) {
    n_particles = 0;
    n_vertices = 0;
    v_particles->clear();
    v_vertices->clear();
    for (int j=0; j<100; j++) {
      v_particles->push_back(ds::SimParticle());
    }
    t->Fill();
  }

  f->Write();
  delete t;
  t = 0;

  f->Close();
  delete f;
  f = 0;

  return 0;
}
