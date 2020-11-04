/*
  runPythiaJet.cxx
  ================
  Run Pythia8 and the jet algorithm 
  Input: LHE file (output of Madgraph5)
  Output: ROOT file with event strucutres from McData
*/
#include <iostream>
#include <string>
#include <vector>
#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC3.h"

#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/CDFMidPointPlugin.hh"
#include "fastjet/CDFJetCluPlugin.hh"

#include "HepMC3/GenEvent.h"

#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TApplication.h"
#include "TClonesArray.h"

#include "McData/FastjetTools.hxx"
#include "McData/HepmcTools.hxx"
#include "McData/McParticle.hxx"
#include "McData/McVertex.hxx"
#include "McData/Jet.hxx"
#include "McData/IndexPair.hxx"

int main(int argc, char* argv[]) {
  std::string pythiaCmdFile="";
  std::string inputLHEFile="";
  std::string outputRootFile="";

  if (argc == 4) {
    if (argc > 1) pythiaCmdFile = argv[1];
    if (argc > 2) inputLHEFile = argv[2];
    if (argc > 3) outputRootFile = argv[3];
  } else {
    std::cout << "Number of arguments: " << argc << std::endl;
    std::cout << "Usage: " << argv[0] << " <PythiaCmdFile> "
	      << " <InputLHEFile(.lhe|.lhe.gz)>" << " OutputRootFile" << std::endl;
    exit(1);
  }

  Pythia8::Pythia pythia;
  char pcmd[1000];
  HepMC3::Pythia8ToHepMC3 toHepMC;

  // int nEvent = pythia.mode("Main:numberOfEvents");
  // pythia.settings.forceParm("SpaceShower:pT0Ref",0.);
  pythia.readString("Beams:frameType = 4");
  std::sprintf(pcmd, "Beams:LHEF = %s", inputLHEFile.c_str());
  pythia.readString(pcmd);
  pythia.init();

  gROOT->SetBatch();
  char appName[] = "runPythiaJet.exe";
  int appargc = 1;
  char *appargv[] = {appName};
  TApplication app(appName, &appargc, appargv);

  TFile* fout = TFile::Open(outputRootFile.c_str(), "RECREATE");

  TClonesArray* ca_particles = new TClonesArray("McParticle", 10000);
  TClonesArray* ca_vertices = new TClonesArray("McVertex", 10000);
  TClonesArray* ca_jets = new TClonesArray("Jet", 1000);
  TClonesArray* ca_jet_matching = new TClonesArray("IndexPair", 10000);
  std::map<HepMC3::ConstGenVertexPtr, int> vtx_id_map;

  TTree* t = new TTree("t", "TTree for MC particles ans jets");
  t->Branch("Particle", "TClonesArray", &ca_particles);
  t->Branch("Vertex", "TClonesArray", &ca_vertices);
  t->Branch("Jet", "TClonesArray", &ca_jets);
  t->Branch("Jet_match", "TClonesArray", &ca_jet_matching);

  const fastjet::JetDefinition& jet_def = getJetDefinition("");
  std::vector<fastjet::PseudoJet>::const_iterator pj;
  std::vector<fastjet::PseudoJet> jets;
  //---------------------------------------------------------------

  int ievent=0;

  while (true) {
    if (!pythia.next()) {
      if (pythia.info.atEndOfFile()) {
	std::cout << "Pythia end-of-file reached, exit event loop" << std::endl;
	break;
      }
      break;
    }
    // Process event
    if (ievent > 0 && (ievent%1000) == 0) {
      std::cout << "Process event : " << ievent << std::endl;
    }

    HepMC3::GenEvent hepmcEvent;
    toHepMC.fill_next_event(pythia, &hepmcEvent);

    // Run jet algorithm
    t->Clear();
    ca_particles->Clear();
    ca_vertices->Clear();
    ca_jets->Clear();
    ca_jet_matching->Clear();
    fastjet::ClusterSequence* cs = runJetAlgorithm(hepmcEvent, jet_def, jets);

    convertVertexData(hepmcEvent, ca_vertices, vtx_id_map);
    convertParticleData(hepmcEvent, ca_particles, vtx_id_map);
    convertJetData(jets, ca_jets, ca_jet_matching);
    
    delete cs;
    t->Fill();

    if (ievent>0 && (ievent%1000)==0) {
      std::cout << "Event number : " << ievent << std::endl;
    }
    // post-processing
    ievent ++;
  }

  pythia.stat();
  t->Write("t");
  
  fout->Write();
  fout->Close();
  delete fout;

  return 0;
}
